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

#define LOG_TAG "NFCTAG_FWK_CLIENT"
#include "nfc_tag_client.h"
#include "nfc_tag_log.h"
#include "nfc_tag_errcode.h"
#include "iservice_registry.h"

namespace OHOS {
namespace NFC {
using ErrCodeMap = struct {
    ErrCode code;
    const char* str;
};
static const ErrCodeMap g_errCodeMap[] = {
    {NFC_SUCCESS, "success"},
    {NFC_REMOTE_DIED, "remote died"},
    {NFC_IPC_WRITETOKE_FAILED, "ipc write token failed"},
    {NFC_INVALID_TOKEN, "invalid token"},
    {NFC_IPC_SEND_FAILED, "send ipc failed"},
    {NFC_IPC_WRITE_FAILED, "write ipc failed"},
    {NFC_IPC_READ_FAILED, "read ipc failed"},
    {NFC_NO_CALLBACK, "null callback"},
    {NFC_NO_REMOTE, "null remote"},
    {NFC_NO_OBJECT, "null object"},
    {NFC_NO_HDI_PROXY, "null hdi proxy"},
    {NFC_NO_HDI_IMPL, "null hdi impl"},
    {NFC_GRANT_FAILED, "grant failed"},
    {NFC_SYS_PERM_FAILED, "check system permission failed"},
    {NFC_INVALID_PROXY, "invalid proxy"},
    {NFC_INVALID_CALLBACKSTUB, "invalid callback stub"},
    {NFC_INVALID_CALLBACK, "invalid callback"},
    {NFC_INVALID_PARAMETER, "invalid parameter"},
    {NFC_CALLBACK_REGISTERED, "callback been registered"},
    {NFC_CALLBACK_NOT_REGISTERED, "callback not been registered"},
    {NFC_CALLBACK_NOT_EQUAL, "callback not the registered one"},
    {NFC_TOO_MANY_CALLBACK, "too many callback"},
    {NFC_HDI_REMOTE_FAILED, "hdi remote failed"},
    {NFC_INVALID_STATE, "invalid state"},
    {NFC_ERR_MAX, "err max"},
};

NfcTagClient::NfcTagClient()
{
}
NfcTagClient::~NfcTagClient()
{
    {
        std::lock_guard<std::mutex> lock(callbackLock_);
        callbackStub_ = nullptr;
    }
    {
        std::lock_guard<std::mutex> lock(proxyLock_);
        serviceProxy_ = nullptr;
    }
}

const char* NfcTagClient::GetErrCodeString(ErrCode code)
{
    if ((code >= 0) && (code <= NFC_ERR_MAX)) {
        for (int i = 0; i < static_cast<int>(sizeof(g_errCodeMap)/sizeof(ErrCodeMap)); ++i) {
            if (g_errCodeMap[i].code == code) {
                return g_errCodeMap[i].str;
            }
        }
    }
    return "unknown error code";
}

sptr<INfcTagService> NfcTagClient::GetService()
{
    std::lock_guard<std::mutex> lock(proxyLock_);
    if (serviceProxy_ != nullptr) {
        return serviceProxy_;
    }
    sptr<ISystemAbilityManager> samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    NFC_CHECK_RETURN_RET(samgr != nullptr, nullptr, "Get samgr failed");
    sptr<IRemoteObject> object = samgr->CheckSystemAbility(NFC_CONNECTED_TAG_ABILITY_ID);
    NFC_CHECK_RETURN_RET(object != nullptr, nullptr, "object is nullptr");
    serviceProxy_ = iface_cast<INfcTagService>(object);
    return serviceProxy_;
}

ErrCode NfcTagClient::Init()
{
    HILOGI("enter");
    sptr<INfcTagService> serviceProxy = GetService();
    NFC_CHECK_RETURN_RET(serviceProxy != nullptr, NFC_INVALID_PROXY, "serviceProxy is nullptr");
    ErrCode code = serviceProxy->Init();
    if (code != NFC_SUCCESS) {
        HILOGE("code: %{public}d, [%{public}s]", code, NfcTagClient::GetErrCodeString(code));
    }
    return code;
}

ErrCode NfcTagClient::Uninit()
{
    HILOGI("enter");
    sptr<INfcTagService> serviceProxy = GetService();
    NFC_CHECK_RETURN_RET(serviceProxy != nullptr, NFC_INVALID_PROXY, "serviceProxy is nullptr");
    ErrCode code = serviceProxy->Uninit();
    if (code != NFC_SUCCESS) {
        HILOGE("code: %{public}d, [%{public}s]", code, NfcTagClient::GetErrCodeString(code));
    }
    return code;
}
ErrCode NfcTagClient::ReadNdefTag(std::string &response)
{
    HILOGI("enter");
    sptr<INfcTagService> serviceProxy = GetService();
    NFC_CHECK_RETURN_RET(serviceProxy != nullptr, NFC_INVALID_PROXY, "serviceProxy is nullptr");
    ErrCode code = serviceProxy->ReadNdefTag(response);
    if (code != NFC_SUCCESS) {
        HILOGE("code: %{public}d, [%{public}s]", code, NfcTagClient::GetErrCodeString(code));
    }
    return code;
}
ErrCode NfcTagClient::WriteNdefTag(const std::string &data)
{
    HILOGI("enter");
    sptr<INfcTagService> serviceProxy = GetService();
    NFC_CHECK_RETURN_RET(serviceProxy != nullptr, NFC_INVALID_PROXY, "serviceProxy is nullptr");
    ErrCode code = serviceProxy->WriteNdefTag(data);
    if (code != NFC_SUCCESS) {
        HILOGE("code: %{public}d, [%{public}s]", code, NfcTagClient::GetErrCodeString(code));
    }
    return code;
}

ErrCode NfcTagClient::ReadNdefData(std::vector<uint8_t> &data)
{
    HILOGI("enter");
    sptr<INfcTagService> serviceProxy = GetService();
    NFC_CHECK_RETURN_RET(serviceProxy != nullptr, NFC_INVALID_PROXY, "serviceProxy is nullptr");
    ErrCode code = serviceProxy->ReadNdefData(data);
    if (code != NFC_SUCCESS) {
        HILOGE("code: %{public}d, [%{public}s]", code, NfcTagClient::GetErrCodeString(code));
    }
    return code;
}

ErrCode NfcTagClient::WriteNdefData(const std::vector<uint8_t> &data)
{
    HILOGI("enter");
    sptr<INfcTagService> serviceProxy = GetService();
    NFC_CHECK_RETURN_RET(serviceProxy != nullptr, NFC_INVALID_PROXY, "serviceProxy is nullptr");
    ErrCode code = serviceProxy->WriteNdefData(data);
    if (code != NFC_SUCCESS) {
        HILOGE("code: %{public}d, [%{public}s]", code, NfcTagClient::GetErrCodeString(code));
    }
    return code;
}

ErrCode NfcTagClient::RegListener(const sptr<INfcTagCallback> &callback)
{
    HILOGI("enter");
    std::lock_guard<std::mutex> callbackLock(callbackLock_);
    if (callbackStub_ == nullptr) {
        callbackStub_ = sptr<NfcTagCallbackStub>::MakeSptr();
    }
    NFC_CHECK_RETURN_RET(callbackStub_ != nullptr, NFC_INVALID_CALLBACKSTUB, "callbackStub_ is nullptr");
    sptr<INfcTagService> serviceProxy = GetService();
    NFC_CHECK_RETURN_RET(serviceProxy != nullptr, NFC_INVALID_PROXY, "serviceProxy is nullptr");
    callbackStub_->RegisterUserCallBack(callback);
    ErrCode code = serviceProxy->RegListener(callbackStub_);
    if (code != NFC_SUCCESS) {
        HILOGE("code: %{public}d, [%{public}s]", code, NfcTagClient::GetErrCodeString(code));
        callbackStub_->UnRegisterUserCallBack(callback);
        if (callbackStub_->IsUserCallbackEmpty()) {
            ErrCode code2 = serviceProxy->UnregListener(callbackStub_);
            if (code2 != NFC_SUCCESS) {
                HILOGE("code2: %{public}d, [%{public}s]", code2, NfcTagClient::GetErrCodeString(code2));
                return code;
            }
        }
    }
    return code;
}
ErrCode NfcTagClient::UnregListener(const sptr<INfcTagCallback> &callback)
{
    HILOGI("enter");
    std::lock_guard<std::mutex> callbackLock(callbackLock_);
    NFC_CHECK_RETURN_RET(callbackStub_ != nullptr, NFC_INVALID_CALLBACKSTUB, "callbackStub_ is nullptr");
    sptr<INfcTagService> serviceProxy = GetService();
    NFC_CHECK_RETURN_RET(serviceProxy != nullptr, NFC_INVALID_PROXY, "serviceProxy is nullptr");
    callbackStub_->UnRegisterUserCallBack(callback);
    if (callbackStub_->IsUserCallbackEmpty()) {
        ErrCode code = serviceProxy->UnregListener(callbackStub_);
        if (code != NFC_SUCCESS) {
            HILOGE("code: %{public}d, [%{public}s]", code, NfcTagClient::GetErrCodeString(code));
            return code;
        }
    }
    return NFC_SUCCESS;
}
}  // namespace NFC
}  // namespace OHOS