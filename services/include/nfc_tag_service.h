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

#ifndef OHOS_NFC_CONNECTED_TAG_SERVICE_H
#define OHOS_NFC_CONNECTED_TAG_SERVICE_H

#include <mutex>
#include <map>
#include "system_ability.h"
#include "nfc_tag_stub.h"
#include "iremote_object.h"
#include "nfc_tag_errcode.h"
#include "nfc_tag_hdi_adapter.h"

namespace OHOS {
namespace NFC {
enum ServiceRunningState {
    STATE_NOT_START,
    STATE_RUNNING
};

class NfcTagCallBackManager : public OHOS::NFC::INfcTagCallback {
public:
    NfcTagCallBackManager() {}
    virtual ~NfcTagCallBackManager() {}
    ErrCode RegisterListener(sptr<INfcTagCallback> listener);
    ErrCode UnRegisterListener(sptr<INfcTagCallback> listener);

public:
    ErrCode OnNotify(int nfcRfState) override;
    OHOS::sptr<OHOS::IRemoteObject> AsObject() override
    {
        return nullptr;
    }
private:
    std::mutex listenerMapLock_;
    std::map<uint64_t, sptr<INfcTagCallback>> listenerMap_;
};

class NfcTagService : public SystemAbility, public NfcTagStub {
    DECLARE_SYSTEM_ABILITY(NfcTagService);
public:
    NfcTagService(int32_t saId, bool runOnCreate);
    ~NfcTagService();

    void OnStart() override;
    void OnStop() override;

    ErrCode Init() override;

    ErrCode Uninit() override;

    ErrCode ReadNdefTag(std::string &response) override;

    ErrCode WriteNdefTag(const std::string &data) override;

    ErrCode ReadNdefData(std::vector<uint8_t> &data) override;

    ErrCode WriteNdefData(const std::vector<uint8_t> &data) override;

    ErrCode RegListener(const sptr<INfcTagCallback> &callback) override;

    ErrCode UnregListener(const sptr<INfcTagCallback> &callback) override;
private:
    bool ServiceInit();
    ErrCode VerifyPermissionsBeforeEntry();
private:
    NfcTagHdiAdapter hdiAdapter_;
    bool published_;
    ServiceRunningState state_;
    sptr<NfcTagCallBackManager> callbackManager_;
    std::mutex callbackMutex_;
};
}  // namespace NFC
}  // namespace OHOS
#endif