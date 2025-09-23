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
#ifndef NFC_NAPI_EVENT_H_
#define NFC_NAPI_EVENT_H_
#include <string>
#include <set>
#include <map>
#include <vector>
#include <shared_mutex>
#include "nfc_tag_errcode.h"
#include "infc_tag_callback.h"
#include "napi/native_api.h"

namespace OHOS {
namespace NFC {
class AsyncEventData {
public:
    napi_env env_;
    napi_ref callbackRef_;
    int32_t jsEvent_;

    explicit AsyncEventData(napi_env e, napi_ref r, int32_t v)
    {
        env_ = e;
        callbackRef_ = r;
        jsEvent_ = v;
    }

    AsyncEventData() = delete;

    virtual ~AsyncEventData() {
    }
};

class RegObj {
public:
    RegObj() : regEnv_(0), regHanderRef_(nullptr) {
    }
    explicit RegObj(const napi_env& env, const napi_ref& ref)
    {
        regEnv_ = env;
        regHanderRef_ = ref;
    }

    ~RegObj() {
    }

    napi_env regEnv_;
    napi_ref regHanderRef_;
};

class NapiEvent {
public:
    bool CheckIsRegister(const std::string& type);
    void EventNotify(AsyncEventData *asyncEvent);
    void CheckAndNotify(const std::string& type, int32_t value);
};

class NfcListenerEvent : public INfcTagCallback, public NapiEvent {
public:
    NfcListenerEvent() {}

    virtual ~NfcListenerEvent() {}

public:
    ErrCode OnNotify(int nfcRfState) override;

    OHOS::sptr<OHOS::IRemoteObject> AsObject() override
    {
        return nullptr;
    }
};

class EventRegister {
public:
    EventRegister() {
    }
    ~EventRegister() {
    }

    static EventRegister& GetInstance();

    void Register(const napi_env& env, const std::string& type, napi_value handler);
    void Unregister(const napi_env& env, const std::string& type, napi_value handler);

private:
    ErrCode RegisterNfcEvents();
    ErrCode UnRegisterNfcEvents();
    bool IsEventSupport(const std::string& type);
    void DeleteRegisterObj(std::vector<RegObj>& vecRegObjs, napi_value& handler);
    void DeleteAllRegisterObj(std::vector<RegObj>& vecRegObjs);

    sptr<NfcListenerEvent> nfcListenerEvent_ = nullptr;
};

napi_value On(napi_env env, napi_callback_info cbinfo);
napi_value Off(napi_env env, napi_callback_info cbinfo);
}  // namespace NFC
}  // namespace OHOS

#endif
