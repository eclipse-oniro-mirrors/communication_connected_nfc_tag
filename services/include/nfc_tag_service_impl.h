/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#ifndef OHOS_NFC_CONNECTED_TAG_SERVICE_IMPL_H
#define OHOS_NFC_CONNECTED_TAG_SERVICE_IMPL_H

#include "system_ability.h"
#include "nfc_tag_session_stub.h"
#include "iremote_object.h"
#include "define.h"

namespace OHOS {
namespace NFC {
enum ServiceRunningState {
    STATE_NOT_START,
    STATE_RUNNING
};
class NfcTagServiceImpl : public SystemAbility, public NfcTagSessionStub {
DECLARE_SYSTEM_ABILITY(NfcTagServiceImpl);
public:
    NfcTagServiceImpl();
    virtual ~NfcTagServiceImpl();

    static sptr<NfcTagServiceImpl> GetInstance();

    void OnStart() override;
    void OnStop() override;

    ErrCode Init() override;

    ErrCode Uninit() override;

    ErrCode ReadNdefTag(std::string &response) override;

    ErrCode WriteNdefTag(std::string data) override;

    ErrCode RegListener(const sptr<INfcTagCallback> &callback) override;

    ErrCode UnregListener(const sptr<INfcTagCallback> &callback) override;
private:
    bool ServiceInit();

private:
    bool mPublishFlag;
    ServiceRunningState mState;
};
}  // namespace NFC
}  // namespace OHOS
#endif