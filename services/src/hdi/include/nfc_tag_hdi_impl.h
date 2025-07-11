/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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
#ifndef OHOS_NFC_TAG_HDI_IMPL_H
#define OHOS_NFC_TAG_HDI_IMPL_H

#include <string>
#include <vector>
#include <cstdint>
#include <mutex>

#include "infc_tag_callback.h"
#include "v1_1/iconnected_nfc_tag.h"
#include "iservstat_listener_hdi.h"
#include "nfc_tag_errcode.h"
#include "system_ability_listener.h"

namespace OHOS {
namespace NFC {

class NfcTagDriverStatusListener : public OHOS::HDI::ServiceManager::V1_0::ServStatListenerStub {
public:
    NfcTagDriverStatusListener() = default;
    ~NfcTagDriverStatusListener() = default;
    void OnReceive(const OHOS::HDI::ServiceManager::V1_0::ServiceStatus &status) override;
};

class NfcTagHdiCallBack : public HDI::ConnectedNfcTag::V1_1::IConnectedNfcTagCallback {
public:
    NfcTagHdiCallBack() = default;
    ~NfcTagHdiCallBack();
    ErrCode RegisterUpperCallBack(sptr<INfcTagCallback> listener);
    ErrCode UnRegisterUpperCallBack(sptr<INfcTagCallback> listener);
    int32_t OnChipEvent(HDI::ConnectedNfcTag::V1_1::ConnectedNfcTagEvent event,
        const std::vector<uint8_t>& message) override;
private:
    std::mutex callbackMutex_;
    sptr<INfcTagCallback> upperCallBack_{nullptr};
};

class NfcTagHdiImpl {
public:
    static NfcTagHdiImpl &GetInstance();
    NfcTagHdiImpl();
    ~NfcTagHdiImpl();
    ErrCode Init();
    ErrCode Uninit();
    ErrCode ReadNdefTag(std::string &data);
    ErrCode WriteNdefTag(const std::string &data);
    ErrCode ReadNdefData(std::vector<uint8_t> &data);
    ErrCode WriteNdefData(const std::vector<uint8_t> &data);
    ErrCode RegisterCallBack(sptr<INfcTagCallback> listener);
    ErrCode UnRegisterCallBack(sptr<INfcTagCallback> listener);
    ErrCode InitDriver();
    void OnNfcTagDriverStart();
    void OnNfcTagDriverStop();
private:
    sptr<OHOS::HDI::ConnectedNfcTag::V1_1::IConnectedNfcTag> GetProxy();
    sptr<OHOS::HDI::ConnectedNfcTag::V1_1::IConnectedNfcTag> nfcTagProxy_;
    std::mutex driverMutex_;
private:
    void OnDriverManagerAdd();
    void OnDriverManagerRemove();
    sptr<SystemAbilityListener> driverManagerStatusListener_ = nullptr;
    sptr<OHOS::HDI::ServiceManager::V1_0::ServStatListenerStub> nfcTagDriverStatusListener_ = nullptr;
};
}  // namespace NFC
}  // namespace OHOS
#endif
