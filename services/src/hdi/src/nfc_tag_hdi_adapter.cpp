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
#include "nfc_tag_hdi_adapter.h"
#include <mutex>
#include "nfc_tag_log.h"
#include "nfc_tag_errcode.h"
#include "v1_1/iconnected_nfc_tag.h"

using namespace OHOS::HDI::ConnectedNfcTag;
using IConnectedNfcTagV1_1 = OHOS::HDI::ConnectedNfcTag::V1_1::IConnectedNfcTag;

namespace OHOS {
namespace NFC {

class NfcTagHdiAdapter::Impl : public HDI::ConnectedNfcTag::V1_1::IConnectedNfcTagCallback {
public:
    Impl();
    ~Impl();
    ErrCode Init();
    ErrCode Uninit();
    ErrCode ReadNdefTag(std::string &data);
    ErrCode WriteNdefTag(const std::string &data);
    ErrCode ReadNdefData(std::vector<uint8_t> &data);
    ErrCode WriteNdefData(const std::vector<uint8_t> &data);
    ErrCode RegisterCallBack(sptr<INfcTagCallback> listener, sptr<Impl> impl);
    ErrCode UnRegisterCallBack(sptr<INfcTagCallback> listener, sptr<Impl> impl);
    int32_t OnChipEvent(HDI::ConnectedNfcTag::V1_1::ConnectedNfcTagEvent event,
        const std::vector<uint8_t>& message) override;
private:
    sptr<IConnectedNfcTagV1_1> GetProxy();
    std::mutex proxyMutex_;
    sptr<IConnectedNfcTagV1_1> nfcTagProxy_;
    sptr<INfcTagCallback> upperCallBack_;
    std::mutex callbackMutex_;
    bool inited;
};

NfcTagHdiAdapter::Impl::Impl(): inited(false)
{
    HILOGI("enter");
}

NfcTagHdiAdapter::Impl::~Impl()
{
    HILOGI("destroy called.");
    {
        std::lock_guard<std::mutex> lock(callbackMutex_);
        upperCallBack_ = nullptr;
    }
    if (inited) {
        Uninit();
    }
    std::lock_guard<std::mutex> lock(proxyMutex_);
    nfcTagProxy_ = nullptr;
}

sptr<IConnectedNfcTagV1_1> NfcTagHdiAdapter::Impl::GetProxy()
{
    std::lock_guard<std::mutex> lock(proxyMutex_);
    if (nfcTagProxy_ == nullptr) {
        nfcTagProxy_ = IConnectedNfcTagV1_1::Get();
    }
    return nfcTagProxy_;
}

ErrCode NfcTagHdiAdapter::Impl::Init()
{
    HILOGI("enter");
    sptr<IConnectedNfcTagV1_1> nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    int32_t ret = nfcTagProxy->Init();
    if (ret != 0) {
        HILOGE("nfcTagProxy->Init %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    inited = true;
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiAdapter::Impl::Uninit()
{
    HILOGI("enter");
    sptr<IConnectedNfcTagV1_1> nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    int32_t ret = nfcTagProxy->Uninit();
    inited = false;
    if (ret != 0) {
        HILOGE("nfcTagProxy->Uninit %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiAdapter::Impl::ReadNdefTag(std::string &data)
{
    HILOGI("enter");
    sptr<IConnectedNfcTagV1_1> nfcTagProxy = GetProxy();
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

ErrCode NfcTagHdiAdapter::Impl::WriteNdefTag(const std::string &data)
{
    HILOGI("enter");
    sptr<IConnectedNfcTagV1_1> nfcTagProxy = GetProxy();
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

ErrCode NfcTagHdiAdapter::Impl::ReadNdefData(std::vector<uint8_t> &data)
{
    HILOGI("enter");
    sptr<IConnectedNfcTagV1_1> nfcTagProxy = GetProxy();
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

ErrCode NfcTagHdiAdapter::Impl::WriteNdefData(const std::vector<uint8_t> &data)
{
    HILOGI("enter");
    sptr<IConnectedNfcTagV1_1> nfcTagProxy = GetProxy();
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

ErrCode NfcTagHdiAdapter::Impl::RegisterCallBack(sptr<INfcTagCallback> listener, sptr<Impl> impl)
{
    HILOGI("enter");
    sptr<IConnectedNfcTagV1_1> nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    {
        std::lock_guard<std::mutex> lock(callbackMutex_);
        upperCallBack_ = listener;
    }
    int32_t ret = nfcTagProxy->RegisterCallBack(impl);
    if (ret != 0) {
        HILOGE("nfcTagProxy->RegisterCallBack %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    return NFC_SUCCESS;
}

ErrCode NfcTagHdiAdapter::Impl::UnRegisterCallBack(sptr<INfcTagCallback> listener, sptr<Impl> impl)
{
    HILOGI("enter");
    sptr<IConnectedNfcTagV1_1> nfcTagProxy = GetProxy();
    if (nfcTagProxy == nullptr) {
        HILOGE("nfcTagProxy == nullptr");
        return NFC_NO_HDI_PROXY;
    }
    {
        std::lock_guard<std::mutex> lock(callbackMutex_);
        if (upperCallBack_ == listener) {
            upperCallBack_ = nullptr;
        } else {
            HILOGE("listener not equal the regisetered");
            return NFC_NO_CALLBACK;
        }
    }
    int32_t ret = nfcTagProxy->RegisterCallBack(nullptr);
    if (ret != 0) {
        HILOGE("UnRegisterCallBack %{public}d", ret);
        return NFC_HDI_REMOTE_FAILED;
    }
    return NFC_SUCCESS;
}

int32_t NfcTagHdiAdapter::Impl::OnChipEvent(HDI::ConnectedNfcTag::V1_1::ConnectedNfcTagEvent event,
    const std::vector<uint8_t>& message)
{
    HILOGI("event: %d", static_cast<int>(event));
    std::lock_guard<std::mutex> lock(callbackMutex_);
    if (upperCallBack_ != nullptr) {
        upperCallBack_->OnNotify(static_cast<int>(event));
    }
    return 0;
}

NfcTagHdiAdapter::NfcTagHdiAdapter(): pimpl_(sptr<Impl>::MakeSptr())
{
    HILOGI("enter");
}

NfcTagHdiAdapter::~NfcTagHdiAdapter()
{
    HILOGI("enter");
}

NfcTagHdiAdapter &NfcTagHdiAdapter::GetInstance()
{
    static NfcTagHdiAdapter sNfcTagHdiAdapter;
    return sNfcTagHdiAdapter;
}

ErrCode NfcTagHdiAdapter::Init()
{
    return pimpl_->Init();
}
ErrCode NfcTagHdiAdapter::Uninit()
{
    return pimpl_->Uninit();
}
ErrCode NfcTagHdiAdapter::ReadNdefTag(std::string &data)
{
    return pimpl_->ReadNdefTag(data);
}
ErrCode NfcTagHdiAdapter::WriteNdefTag(const std::string &data)
{
    return pimpl_->WriteNdefTag(data);
}

ErrCode NfcTagHdiAdapter::ReadNdefData(std::vector<uint8_t> &data)
{
    return pimpl_->ReadNdefData(data);
}

ErrCode NfcTagHdiAdapter::WriteNdefData(const std::vector<uint8_t> &data)
{
    return pimpl_->WriteNdefData(data);
}

ErrCode NfcTagHdiAdapter::RegisterCallBack(sptr<INfcTagCallback> listener)
{
    return pimpl_->RegisterCallBack(listener, pimpl_);
}

ErrCode NfcTagHdiAdapter::UnRegisterCallBack(sptr<INfcTagCallback> listener)
{
    return pimpl_->UnRegisterCallBack(listener, pimpl_);
}

}  // namespace NFC
}  // namespace OHOS
