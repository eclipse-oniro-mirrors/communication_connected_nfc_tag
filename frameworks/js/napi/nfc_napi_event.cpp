/*
 * Copyright (C) 2022-2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "NFCTAG_FWK_NAPI"
#include "nfc_napi_event.h"
#include <uv.h>
#include "nfc_tag_log.h"
#include "nfc_napi_utils.h"
#include "infc_tag_callback.h"
#include "nfc_tag_client.h"

namespace OHOS {
namespace NFC {

namespace {
const std::string EVENT_NOTIFY = "notify";
const std::set<std::string> SUPPORT_EVENT_LIST = {
    EVENT_NOTIFY,
};
constexpr int NOTIFY_TYPE_LEN = 64;
constexpr int NFC_ENTER = 0;
constexpr int NFC_LEAVE = 1;
constexpr size_t ARGC_ONE = 1;
constexpr size_t ARGC_TWO = 2;
std::shared_mutex g_regInfoMutex;
std::map<std::string, std::vector<RegObj>> g_eventRegisterInfo;
}

class NapiEvent {
public:
    bool CheckIsRegister(const std::string& type);
    void EventNotify(AsyncEventData *asyncEvent);

    template<typename T>
    void CheckAndNotify(const std::string& type, const T& obj)
    {
        std::shared_lock<std::shared_mutex> guard(NFC::g_regInfoMutex);
        if (!CheckIsRegister(type)) {
            return;
        }

        std::vector<RegObj>& vecObj = NFC::g_eventRegisterInfo[type];
        for (auto& each : vecObj) {
            napi_value result;
            napi_create_int32(each.regEnv_, obj, &result);
            AsyncEventData *asyncEvent = new AsyncEventData(each.regEnv_, each.regHanderRef_, result);
            if (asyncEvent == nullptr) {
                return;
            }
            EventNotify(asyncEvent);
        }
    }
};

bool EventRegister::isEventRegistered = false;

void NapiEvent::EventNotify(AsyncEventData *asyncEvent)
{
    HILOGI("Enter nfc event notify");
    if (asyncEvent == nullptr) {
        HILOGE("asyncEvent is null.");
        return;
    }
    uv_loop_s* loop = nullptr;
    napi_get_uv_event_loop(asyncEvent->env_, &loop);

    uv_work_t* work = new uv_work_t;
    if (work == nullptr) {
        HILOGE("uv_work_t work is null.");
        delete asyncEvent;
        asyncEvent = nullptr;
        return;
    }

    work->data = asyncEvent;
    uv_queue_work(
        loop, work, [](uv_work_t* work) {},
        [](uv_work_t* work, int status) {
            AsyncEventData *asyncData = static_cast<AsyncEventData*>(work->data);
            if (asyncData == nullptr) {
                HILOGE("asyncData is null.");
                return;
            }
            HILOGI("Napi event uv_queue_work, status: %{public}d", status);
            napi_handle_scope scope = nullptr;
            napi_open_handle_scope(asyncData->env_, &scope);
            if (scope == nullptr) {
                HILOGE("scope is nullptr");
                napi_close_handle_scope(asyncData->env_, scope);
                return;
            }
            napi_value undefine;
            napi_get_undefined(asyncData->env_, &undefine);
            napi_value handler = nullptr;
            napi_get_reference_value(asyncData->env_, asyncData->callbackRef_, &handler);

            if (napi_call_function(asyncData->env_, nullptr, handler, 1, &asyncData->jsEvent_, &undefine) != napi_ok) {
                HILOGE("Report event to Js failed");
            }
            napi_close_handle_scope(asyncData->env_, scope);
            if (asyncData != nullptr) {
                delete asyncData;
                asyncData = nullptr;
            }
            delete work;
            work = nullptr;
        });
}

bool NapiEvent::CheckIsRegister(const std::string& type)
{
    return NFC::g_eventRegisterInfo.find(type) != NFC::g_eventRegisterInfo.end();
}

class NfcListenerEvent : public INfcTagCallback, public NapiEvent {
public:
    NfcListenerEvent() {}

    virtual ~NfcListenerEvent() {}

public:
    ErrCode OnNotify(int nfcRfState) override
    {
        HILOGI("OnNotify rcvd nfcRfState: %{public}d", nfcRfState);
        if (nfcRfState == NFC_ENTER || nfcRfState == NFC_LEAVE) {
            CheckAndNotify(EVENT_NOTIFY, nfcRfState);
        }
        return NFC_SUCCESS;
    }

    OHOS::sptr<OHOS::IRemoteObject> AsObject() override
    {
        return nullptr;
    }
};

sptr<NfcListenerEvent> nfcListenerEvent =
    sptr<NfcListenerEvent>(new (std::nothrow) NfcListenerEvent());

napi_value On(napi_env env, napi_callback_info cbinfo)
{
    size_t argc = ARGC_TWO;
    napi_value argv[ARGC_TWO] = {0};
    napi_value thisVar = 0;
    napi_get_cb_info(env, cbinfo, &argc, argv, &thisVar, nullptr);
    napi_valuetype eventName = napi_undefined;
    napi_typeof(env, argv[0], &eventName);
    napi_valuetype handler = napi_undefined;
    napi_typeof(env, argv[1], &handler);

    if (argc != ARGC_TWO || eventName != napi_string || handler != napi_function) {
        HILOGE("On args invalid, failed!");
        napi_value result;
        napi_get_boolean(env, false, &result);
        return result;
    }

    char type[NOTIFY_TYPE_LEN] = {0};
    size_t typeLen = 0;
    napi_get_value_string_utf8(env, argv[0], type, sizeof(type), &typeLen);
    EventRegister::GetInstance().Register(env, type, argv[1]);
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
    return result;
}

napi_value Off(napi_env env, napi_callback_info cbinfo)
{
    size_t argc = ARGC_TWO;
    napi_value argv[ARGC_TWO] = {0};
    napi_value thisVar = 0;
    napi_get_cb_info(env, cbinfo, &argc, argv, &thisVar, nullptr);
    napi_valuetype eventName = napi_undefined;
    napi_typeof(env, argv[0], &eventName);

    if ((argc != ARGC_ONE && argc != ARGC_TWO) || eventName != napi_string) {
        HILOGE("On args invalid, failed!");
        napi_value result;
        napi_get_boolean(env, false, &result);
        return result;
    }

    if (argc == ARGC_TWO) {
        napi_valuetype handler = napi_undefined;
        napi_typeof(env, argv[1], &handler);
        if (handler == napi_null || handler == napi_undefined) {
            HILOGD("argv[1] is null or undefined, handle as no argv[1] input");
            argc -= 1;
        } else if (handler != napi_function) {
            HILOGE("On args invalid napi_function, failed!");
            napi_value result;
            napi_get_boolean(env, false, &result);
            return result;
        }
    }

    char type[NOTIFY_TYPE_LEN] = {0};
    size_t typeLen = 0;
    napi_get_value_string_utf8(env, argv[0], type, sizeof(type), &typeLen);
    EventRegister::GetInstance().Unregister(env, type, argc >= ARGC_TWO ? argv[1] : nullptr);
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
    return result;
}

ErrCode EventRegister::RegisterNfcEvents()
{
    ErrCode ret = DelayedRefSingleton<NfcTagClient>::GetInstance().RegListener(nfcListenerEvent);
    if (ret != NFC_SUCCESS) {
        HILOGE("RegisterNfcEvents nfcListenerEvent failed!");
        return ret;
    }
    return ret;
}

EventRegister& EventRegister::GetInstance()
{
    static EventRegister inst;
    return inst;
}

bool EventRegister::IsEventSupport(const std::string& type)
{
    return SUPPORT_EVENT_LIST.find(type) != SUPPORT_EVENT_LIST.end();
}

void EventRegister::Register(const napi_env& env, const std::string& type, napi_value handler)
{
    HILOGI("enter");

    if (!IsEventSupport(type)) {
        HILOGE("Register type error or not support!");
        return;
    }
    std::unique_lock<std::shared_mutex> guard(NFC::g_regInfoMutex);
    if (!isEventRegistered) {
        if (RegisterNfcEvents() != NFC_SUCCESS) {
            return;
        }
        isEventRegistered = true;
    }
    napi_ref handlerRef = nullptr;
    napi_create_reference(env, handler, 1, &handlerRef);
    RegObj regObj(env, handlerRef);
    auto iter = NFC::g_eventRegisterInfo.find(type);
    if (iter == NFC::g_eventRegisterInfo.end()) {
        NFC::g_eventRegisterInfo[type] = std::vector<RegObj> {regObj};
    } else {
        iter->second.emplace_back(regObj);
    }
}

void EventRegister::DeleteRegisterObj(std::vector<RegObj>& vecRegObjs, napi_value& handler)
{
    auto iter = vecRegObjs.begin();
    for (;iter != vecRegObjs.end();) {
        napi_value handlerTemp = nullptr;
        napi_get_reference_value(iter->regEnv_, iter->regHanderRef_, &handlerTemp);
        bool isEqual = false;
        napi_strict_equals(iter->regEnv_, handlerTemp, handler, &isEqual);
        if (isEqual) {
            napi_delete_reference(iter->regEnv_, iter->regHanderRef_);
            HILOGI("Delete register object ref.");
            iter = vecRegObjs.erase(iter);
        } else {
            ++iter;
        }
    }
}

void EventRegister::DeleteAllRegisterObj(std::vector<RegObj>& vecRegObjs)
{
    for (auto& each : vecRegObjs) {
        napi_delete_reference(each.regEnv_, each.regHanderRef_);
    }
    vecRegObjs.clear();
}

void EventRegister::Unregister(const napi_env& env, const std::string& type, napi_value handler)
{
    HILOGI("enter");

    if (!IsEventSupport(type)) {
        HILOGE("Unregister type error or not support!");
        return;
    }

    std::unique_lock<std::shared_mutex> guard(NFC::g_regInfoMutex);
    auto iter = NFC::g_eventRegisterInfo.find(type);
    if (iter == NFC::g_eventRegisterInfo.end()) {
        HILOGE("Unregister type not registered!");
        return;
    }
    if (handler != nullptr) {
        DeleteRegisterObj(iter->second, handler);
    } else {
        HILOGW("All callback is unsubscribe for event: %{public}s", type.c_str());
        DeleteAllRegisterObj(iter->second);
    }
    if (iter->second.empty()) {
        NFC::g_eventRegisterInfo.erase(iter);
    }
}
}  // namespace NFC
}  // namespace OHOS
