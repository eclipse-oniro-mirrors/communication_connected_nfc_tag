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
#define LOG_TAG "NFCTAG_SVC_HDI"
#include "nfc_tag_hdi_impl.h"
#include "nfc_tag_log.h"
#include "nfc_tag_errcode.h"
#include "system_ability_definition.h"
#include "iservice_registry.h"
#include "iservmgr_hdi.h"

namespace OHOS {
namespace NFC {

namespace {
constexpr const char *SERVICE_NAME = "connected_nfc_tag_service";
};

void NfcTagDriverStatusListener::OnReceive(const OHOS::HDI::ServiceManager::V1_0::ServiceStatus &status)
{
    if (status.serviceName != SERVICE_NAME) {
        return;
    }

    HILOGI("receive service %{public}s status %{public}d", status.serviceName.c_str(), status.status);
    if (status.status == OHOS::HDI::ServiceManager::V1_0::SERVIE_STATUS_START) {
        NfcTagHdiImpl::GetInstance().OnNfcTagDriverStart();
    } else if (status.status == OHOS::HDI::ServiceManager::V1_0::SERVIE_STATUS_STOP) {
        NfcTagHdiImpl::GetInstance().OnNfcTagDriverStop();
    }
}

NfcTagHdiCallBack::~NfcTagHdiCallBack()
{
    std::lock_guard<std::mutex> lock(callbackMutex_);
    upperCallBack_ = nullptr;
}

ErrCode NfcTagHdiCallBack::RegisterUpperCallBack(sptr<INfcTagCallback> listener)
{
    std::lock_guard<std::mutex> lock(callbackMutex_);
    if (listener == nullptr) {
        HILOGE("listener == nullptr");
        return NFC_NO_CALLBACK;
    }
    if (upperCallBack_ != nullptr) {
        HILOGE("listener been registered");
        return NFC_CALLBACK_REGISTERED;
    }
    upperCallBack_ = listener;
    return NFC_SUCCESS;
}
ErrCode NfcTagHdiCallBack::UnRegisterUpperCallBack(sptr<INfcTagCallback> listener)
{
    std::lock_guard<std::mutex> lock(callbackMutex_);
    if (upperCallBack_ != listener || listener == nullptr) {
        HILOGE("listener not equal the registered");
        return NFC_INVALID_CALLBACK;
    }
    upperCallBack_ = nullptr;
    return NFC_SUCCESS;
}
int32_t NfcTagHdiCallBack::OnChipEvent(HDI::ConnectedNfcTag::V1_1::ConnectedNfcTagEvent event,
    const std::vector<uint8_t>& message)
{
    HILOGI("event: %d", static_cast<int>(event));
    std::lock_guard<std::mutex> lock(callbackMutex_);
    if (upperCallBack_ != nullptr) {
        upperCallBack_->OnNotify(static_cast<int>(event));
    }
    return 0;
}

sptr<NfcTagHdiCallBack> g_nfcTagHdiCallBack = sptr<NfcTagHdiCallBack>::MakeSptr();

NfcTagHdiImpl &NfcTagHdiImpl::GetInstance()
{
    static NfcTagHdiImpl sNfcTagHdiImpl;
    return sNfcTagHdiImpl;
}

NfcTagHdiImpl::NfcTagHdiImpl()
{
    HILOGI("enter");
    nfcTagDriverStatusListener_ = sptr<NfcTagDriverStatusListener>::MakeSptr();
}

NfcTagHdiImpl::~NfcTagHdiImpl()
{
    HILOGI("destroy called.");
    Uninit();

    if (driverManagerStatusListener_) {
        SystemAbilityListener::UnSubscribe(DEVICE_SERVICE_MANAGER_SA_ID, driverManagerStatusListener_);
    }

    std::lock_guard<std::mutex> lock(driverMutex_);
    nfcTagProxy_ = nullptr;
}

sptr<OHOS::HDI::ConnectedNfcTag::V1_1::IConnectedNfcTag> NfcTagHdiImpl::GetProxy()
{
    if (nfcTagProxy_ == nullptr) {
        nfcTagProxy_ = OHOS::HDI::ConnectedNfcTag::V1_1::IConnectedNfcTag::Get();
    }
    return nfcTagProxy_;
}

ErrCode NfcTagHdiImpl::Init()
{
    HILOGI("enter");
    std::lock_guard<std::mutex> lock(driverMutex_);
    auto nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    int32_t ret = nfcTagProxy->Init();
    if (ret != 0) {
        HILOGE("nfcTagProxy->Init %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiImpl::Uninit()
{
    HILOGI("enter");
    std::lock_guard<std::mutex> lock(driverMutex_);
    auto nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    int32_t ret = nfcTagProxy->Uninit();
    if (ret != 0) {
        HILOGE("nfcTagProxy->Uninit %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiImpl::ReadNdefTag(std::string &data)
{
    HILOGI("enter");
    std::lock_guard<std::mutex> lock(driverMutex_);
    auto nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    int32_t ret = nfcTagProxy->ReadNdefTag(data);
    if (ret != 0) {
        HILOGE("nfcTagProxy->ReadNdefTag %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiImpl::WriteNdefTag(const std::string &data)
{
    HILOGI("enter");
    std::lock_guard<std::mutex> lock(driverMutex_);
    auto nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    int32_t ret = nfcTagProxy->WriteNdefTag(data);
    if (ret != 0) {
        HILOGE("nfcTagProxy->WriteNdefTag %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiImpl::ReadNdefData(std::vector<uint8_t> &data)
{
    HILOGI("enter");
    std::lock_guard<std::mutex> lock(driverMutex_);
    auto nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    int32_t ret = nfcTagProxy->ReadNdefData(data);
    if (ret != 0) {
        HILOGE("nfcTagProxy->ReadNdefData %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiImpl::WriteNdefData(const std::vector<uint8_t> &data)
{
    HILOGI("enter");
    std::lock_guard<std::mutex> lock(driverMutex_);
    auto nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    int32_t ret = nfcTagProxy->WriteNdefData(data);
    if (ret != 0) {
        HILOGE("nfcTagProxy->WriteNdefData %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiImpl::RegisterCallBack(sptr<INfcTagCallback> listener)
{
    HILOGI("enter");
    std::lock_guard<std::mutex> lock(driverMutex_);
    auto nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    ErrCode errCode = g_nfcTagHdiCallBack->RegisterUpperCallBack(listener);
    if (errCode != NFC_SUCCESS) {
        return errCode;
    }
    int32_t ret = nfcTagProxy->RegisterCallBack(g_nfcTagHdiCallBack);
    if (ret != 0) {
        g_nfcTagHdiCallBack->UnRegisterUpperCallBack(listener);
        HILOGE("nfcTagProxy->RegisterCallBack %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiImpl::UnRegisterCallBack(sptr<INfcTagCallback> listener)
{
    HILOGI("enter");
    std::lock_guard<std::mutex> lock(driverMutex_);
    auto nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    ErrCode errCode = g_nfcTagHdiCallBack->UnRegisterUpperCallBack(listener);
    if (errCode != NFC_SUCCESS) {
        return errCode;
    }
    int32_t ret = nfcTagProxy->RegisterCallBack(nullptr);
    if (ret != 0) {
        HILOGE("UnRegisterCallBack %{public}d", ret);
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiImpl::InitDriver()
{
    if (driverManagerStatusListener_ != nullptr) {
        HILOGE("driverManagerStatusListener_ != nullptr");
        return NFC_SUCCESS;
    }
    driverManagerStatusListener_ = SystemAbilityListener::Subscribe(
        "DriverManager", DEVICE_SERVICE_MANAGER_SA_ID,
        [this]() { OnDriverManagerAdd(); },
        [this]() { OnDriverManagerRemove(); });
    return NFC_SUCCESS;
}

void NfcTagHdiImpl::OnDriverManagerAdd()
{
    HILOGI("enter");
    auto servMgr = OHOS::HDI::ServiceManager::V1_0::IServiceManager::Get();
    if (servMgr == nullptr) {
        HILOGE("servMgr == nullptr");
        return;
    }
    if (servMgr->RegisterServiceStatusListener(nfcTagDriverStatusListener_, DEVICE_CLASS_DEFAULT) != 0) {
        HILOGE("RegisterServiceStatusListener failed");
        return;
    }
    auto service = servMgr->GetService(SERVICE_NAME);
    if (service != nullptr) {
        OnNfcTagDriverStart();
    } else {
        OnNfcTagDriverStop();
    }
}

void NfcTagHdiImpl::OnDriverManagerRemove()
{
    HILOGI("enter");
    OnNfcTagDriverStop();
}

void NfcTagHdiImpl::OnNfcTagDriverStart()
{
    HILOGI("enter");
    Init();
}

void NfcTagHdiImpl::OnNfcTagDriverStop()
{
    HILOGI("enter");
    Uninit();
}

}  // namespace NFC
}  // namespace OHOS
