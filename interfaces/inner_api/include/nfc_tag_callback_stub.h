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
#ifndef OHOS_NFC_TAG_CALLBACK_STUB_H
#define OHOS_NFC_TAG_CALLBACK_STUB_H

#include <shared_mutex>
#include "define.h"
#include "infc_tag_callback.h"
#include "iremote_object.h"
#include "iremote_stub.h"

namespace OHOS {
namespace NFC {
class NfcTagCallbackStub : public IRemoteStub<INfcTagCallback> {
public:
    NfcTagCallbackStub();
    virtual ~NfcTagCallbackStub();
    static NfcTagCallbackStub& GetInstance();
    ErrCode RegisterUserCallBack(const sptr<INfcTagCallback> &callBack);

    virtual int OnRemoteRequest(
        uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) override;

    void OnNotify(int nfcRfState) override;

private:
    int RemoteOnNotify(MessageParcel &data, MessageParcel &reply);
    sptr<INfcTagCallback> callback_;
    std::shared_mutex callbackMutex;
    bool mRemoteDied;
};
}  // namespace NFC
}  // namespace OHOS
#endif