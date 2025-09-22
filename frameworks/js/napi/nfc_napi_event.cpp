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
constexpr int32_t NFC_JS_LEAVE = 0;
constexpr int32_t NFC_JS_ENTER = 1;
constexpr size_t ARGC_ONE = 1;
constexpr size_t ARGC_TWO = 2;
std::shared_mutex g_regInfoMutex;
std::map<std::string, std::vector<RegObj>> g_eventRegisterInfo;
}

void NapiEvent::EventNotify(AsyncEventData *asyncEvent)
{
    HILOGI("Enter nfc event notify");
    if (asyncEvent == nullptr) {
        HILOGE("asyncEvent is null.");
        return;
    }

    auto task = [asyncEvent]() {
        napi_handle_scope scope = nullptr;
        napi_open_handle_scope(asyncEvent->env_, &scope);
        if (scope == nullptr) {
            HILOGE("scope is nullptr");
            napi_close_handle_scope(asyncEvent->env_, scope);
            return;
        }
        napi_value undefine;
        napi_get_undefined(asyncEvent->env_, &undefine);
        napi_value result;
        napi_create_int32(asyncEvent->env_, asyncEvent->jsEvent_, &result);
        napi_value handler = nullptr;
        napi_get_reference_value(asyncEvent->env_, asyncEvent->callbackRef_, &handler);
        if (napi_call_function(asyncEvent->env_, nullptr, handler, 1, &result, &undefine) != napi_ok) {
            HILOGE("Report event to Js failed");
        }
        napi_close_handle_scope(asyncEvent->env_, scope);
        delete asyncEvent;
    };

    napi_status sendRet = napi_send_event(asyncEvent->env_, task, napi_eprio_high);
    if (sendRet != napi_ok) {
        HILOGE("napi_send_event failed, ret:%{public}d", sendRet);
        delete asyncEvent;
    }
}

bool NapiEvent::CheckIsRegister(const std::string& type)
{
    return NFC::g_eventRegisterInfo.find(type) != NFC::g_eventRegisterInfo.end();
}

void NapiEvent::CheckAndNotify(const std::string& type, int32_t value)
{
    std::shared_lock<std::shared_mutex> guard(NFC::g_regInfoMutex);
    if (!CheckIsRegister(type)) {
        return;
    }

    std::vector<RegObj>& vecObj = NFC::g_eventRegisterInfo[type];
    for (auto& each : vecObj) {
        AsyncEventData *asyncEvent = new AsyncEventData(each.regEnv_, each.regHanderRef_, value);
        if (asyncEvent == nullptr) {
            return;
        }
        EventNotify(asyncEvent);
    }
}

ErrCode NfcListenerEvent::OnNotify(int nfcRfState)
{
    HILOGI("OnNotify rcvd nfcRfState: %{public}d", nfcRfState);
    if (nfcRfState == NFC_ENTER) {
        CheckAndNotify(EVENT_NOTIFY, NFC_JS_ENTER);
    } else if (nfcRfState == NFC_LEAVE) {
        CheckAndNotify(EVENT_NOTIFY, NFC_JS_LEAVE);
    }
    return NFC_SUCCESS;
}

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
    if (nfcListenerEvent_) {
        return NFC_SUCCESS;
    }
    nfcListenerEvent_ = sptr<NfcListenerEvent>::MakeSptr();
    ErrCode ret = DelayedRefSingleton<NfcTagClient>::GetInstance().RegListener(nfcListenerEvent_);
    if (ret != NFC_SUCCESS) {
        HILOGE("RegListener failed!");
        nfcListenerEvent_ = nullptr;
        return ret;
    }
    return NFC_SUCCESS;
}

ErrCode EventRegister::UnRegisterNfcEvents()
{
    if (nfcListenerEvent_ == nullptr) {
        return NFC_SUCCESS;
    }
    ErrCode ret = DelayedRefSingleton<NfcTagClient>::GetInstance().UnregListener(nfcListenerEvent_);
    if (ret != NFC_SUCCESS) {
        HILOGE("UnregListener failed!");
        return ret;
    }
    nfcListenerEvent_ = nullptr;
    return NFC_SUCCESS;
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
    RegisterNfcEvents();
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
        UnRegisterNfcEvents();
    }
    if (iter->second.empty()) {
        NFC::g_eventRegisterInfo.erase(iter);
    }
}
}  // namespace NFC
}  // namespace OHOS
