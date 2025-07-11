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

#define LOG_TAG "NFCTAG_SERVICE"
#include "nfc_tag_service.h"
#include "nfc_tag_log.h"
#include "nfc_tag_errcode.h"
#include "nfc_tag_hdi_adapter.h"
#include "nfc_tag_utils.h"
#include "accesstoken_kit.h"
#include "ipc_skeleton.h"
#include "tokenid_kit.h"

using namespace OHOS::Security::AccessToken;

namespace OHOS {
namespace NFC {
REGISTER_SYSTEM_ABILITY_BY_ID(NfcTagService, NFC_CONNECTED_TAG_ABILITY_ID, true);

static constexpr size_t MAX_LISTENER_NUM = 30;

__attribute__((weak)) ErrCode NfcTagServiceVerifyPermissionsHook(void)
{
    AccessTokenID callerToken = IPCSkeleton::GetCallingTokenID();
    ATokenTypeEnum tokenType = AccessTokenKit::GetTokenTypeFlag(callerToken);
    HILOGI("tokenType: %{public}d", static_cast<int>(tokenType));
    if (tokenType == ATokenTypeEnum::TOKEN_INVALID) {
        return NFC_SYS_PERM_FAILED;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagCallBackManager::RegisterListener(sptr<INfcTagCallback> listener)
{
    uint64_t tokenId = IPCSkeleton::GetCallingFullTokenID();
    HILOGI("enter, token id: %{public}lu", tokenId);
    if (listener == nullptr) {
        HILOGE("listener == nullptr");
        return NFC_INVALID_CALLBACK;
    }
    std::lock_guard<std::mutex> lock(listenerMapLock_);
    // one process, one callback
    if (listenerMap_.count(tokenId) != 0) {
        HILOGE("token id: %{public}lu has registered callback", tokenId);
        return NFC_CALLBACK_REGISTERED;
    }
    if (listenerMap_.size() >= MAX_LISTENER_NUM) {
        HILOGE("token id: %{public}lu, too many listeners > %{public}lu", tokenId, MAX_LISTENER_NUM);
        return NFC_TOO_MANY_CALLBACK;
    }
    listenerMap_[tokenId] = listener;
    return NFC_SUCCESS;
}

ErrCode NfcTagCallBackManager::UnRegisterListener(sptr<INfcTagCallback> listener)
{
    uint64_t tokenId = IPCSkeleton::GetCallingFullTokenID();
    HILOGI("enter, token id: %{public}lu", tokenId);
    std::lock_guard<std::mutex> lock(listenerMapLock_);
    if (listenerMap_.count(tokenId) == 0) {
        HILOGE("token id: %{public}lu hasn't registered callback", tokenId);
        return NFC_CALLBACK_NOT_REGISTERED;
    }
    if (listenerMap_[tokenId] != listener) {
        HILOGW("token id: %{public}lu listener not the registered one", tokenId);
        return NFC_CALLBACK_NOT_EQUAL;
    }
    listenerMap_.erase(tokenId);
    return NFC_SUCCESS;
}

ErrCode NfcTagCallBackManager::OnNotify(int nfcRfState)
{
    std::lock_guard<std::mutex> lock(listenerMapLock_);
    for (auto &iter : listenerMap_) {
        if (iter.second != nullptr) {
            iter.second->OnNotify(nfcRfState);
        }
    }
    return NFC_SUCCESS;
}

NfcTagService::NfcTagService(int32_t saId, bool runOnCreate)
    : SystemAbility(saId, runOnCreate), published_(false),
    state_(ServiceRunningState::STATE_NOT_START), callbackManager_(nullptr)
{
}

NfcTagService::~NfcTagService()
{
    if (callbackManager_ != nullptr) {
        callbackManager_ = nullptr;
    }
}

void NfcTagService::OnStart()
{
    HILOGI("enter");
    if (state_ == ServiceRunningState::STATE_RUNNING) {
        HILOGI("Service has already started.");
        return;
    }
    ErrCode errCode = hdiAdapter_.InitDriver();
    HILOGI("hdiAdapter_.InitDriver: %{public}d", errCode);
    if (!ServiceInit()) {
        HILOGE("Failed to init service");
        OnStop();
        return;
    }
    state_ = ServiceRunningState::STATE_RUNNING;
    HILOGI("Start service!");
}

void NfcTagService::OnStop()
{
    HILOGI("enter");
    state_ = ServiceRunningState::STATE_NOT_START;
    published_ = false;
    HILOGI("Stop service!");
}

bool NfcTagService::ServiceInit()
{
    HILOGI("enter");
    if (!published_) {
        bool ret = Publish(this);
        if (!ret) {
            HILOGE("Failed to publish service!");
            return false;
        }
        published_ = true;
    }
    return true;
}

ErrCode NfcTagService::VerifyPermissionsBeforeEntry()
{
    if (!Utils::IsGranted(OHOS::NFC::TAG_PERMISSION)) {
        HILOGE("IsGranted failed!");
        return NFC_GRANT_FAILED;
    }
    std::string callingName = Utils::GetCallingName();
    HILOGI("callingName: %{public}s", callingName.c_str());
    ErrCode ret = NfcTagServiceVerifyPermissionsHook();
    if (ret != NFC_SUCCESS) {
        HILOGE("NfcTagServiceVerifyPermissionsHook failed, %{public}d", ret);
        return ret;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagService::Init()
{
    HILOGI("enter");
    ErrCode ret = VerifyPermissionsBeforeEntry();
    if (ret != NFC_SUCCESS) {
        return ret;
    }
    return hdiAdapter_.Init();
}

ErrCode NfcTagService::Uninit()
{
    ErrCode ret = VerifyPermissionsBeforeEntry();
    if (ret != NFC_SUCCESS) {
        return ret;
    }
    return hdiAdapter_.Uninit();
}

ErrCode NfcTagService::ReadNdefTag(std::string &response)
{
    ErrCode ret = VerifyPermissionsBeforeEntry();
    if (ret != NFC_SUCCESS) {
        return ret;
    }
    return hdiAdapter_.ReadNdefTag(response);
}

ErrCode NfcTagService::WriteNdefTag(const std::string &data)
{
    ErrCode ret = VerifyPermissionsBeforeEntry();
    if (ret != NFC_SUCCESS) {
        return ret;
    }
    return hdiAdapter_.WriteNdefTag(data);
}

ErrCode NfcTagService::ReadNdefData(std::vector<uint8_t> &data)
{
    ErrCode ret = VerifyPermissionsBeforeEntry();
    if (ret != NFC_SUCCESS) {
        return ret;
    }
    return hdiAdapter_.ReadNdefData(data);
}

ErrCode NfcTagService::WriteNdefData(const std::vector<uint8_t> &data)
{
    ErrCode ret = VerifyPermissionsBeforeEntry();
    if (ret != NFC_SUCCESS) {
        return ret;
    }
    return hdiAdapter_.WriteNdefData(data);
}

ErrCode NfcTagService::RegListener(const sptr<INfcTagCallback> &callback)
{
    ErrCode ret = VerifyPermissionsBeforeEntry();
    if (ret != NFC_SUCCESS) {
        return ret;
    }
    std::lock_guard<std::mutex> lock(callbackMutex_);
    if (callbackManager_ == nullptr) {
        callbackManager_ = sptr<NfcTagCallBackManager>::MakeSptr();
    }
    if (callbackManager_ == nullptr) {
        HILOGE("callbackManager_ == nullptr");
        return NFC_INVALID_CALLBACK;
    }
    ErrCode errCode = callbackManager_->RegisterListener(callback);
    if (errCode == NFC_SUCCESS) {
        ErrCode hdiCode = hdiAdapter_.RegisterCallBack(callbackManager_);
        if (hdiCode != NFC_SUCCESS) {
            callbackManager_->UnRegisterListener(callback);
        }
        return hdiCode;
    }
    return errCode;
}

ErrCode NfcTagService::UnregListener(const sptr<INfcTagCallback> &callback)
{
    ErrCode ret = VerifyPermissionsBeforeEntry();
    if (ret != NFC_SUCCESS) {
        return ret;
    }
    std::lock_guard<std::mutex> lock(callbackMutex_);
    if (callbackManager_ == nullptr) {
        callbackManager_ = sptr<NfcTagCallBackManager>::MakeSptr();
    }
    if (callbackManager_ == nullptr) {
        HILOGE("callbackManager_ == nullptr");
        return NFC_INVALID_CALLBACK;
    }
    ErrCode errCode = callbackManager_->UnRegisterListener(callback);
    if (errCode == NFC_SUCCESS) {
        return hdiAdapter_.UnRegisterCallBack(callbackManager_);
    }
    return errCode;
}

}  // namespace NFC
}  // namespace OHOS