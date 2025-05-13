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
#ifndef OHOS_NFC_TAG_STUB_H
#define OHOS_NFC_TAG_STUB_H

#include <map>
#include <stdint.h>
#include <string>
#include "infc_tag_service.h"
#include "nfc_tag_errcode.h"
#include "iremote_stub.h"

namespace OHOS {
namespace NFC {
class NfcTagStub : public IRemoteStub<INfcTagService> {
public:
    NfcTagStub();
    virtual ~NfcTagStub();

    using handleFunc = ErrCode (NfcTagStub::*)(uint32_t code, MessageParcel &data, MessageParcel &reply);
    using HandleFuncMap = std::map<int, handleFunc>;

    virtual int OnRemoteRequest(
        uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) override;
private:
    bool InitHandleMap();
    ErrCode OnInit(uint32_t code, MessageParcel &data, MessageParcel &reply);
    ErrCode OnUninit(uint32_t code, MessageParcel &data, MessageParcel &reply);
    ErrCode OnReadNdefTag(uint32_t code, MessageParcel &data, MessageParcel &reply);
    ErrCode OnWriteNdefTag(uint32_t code, MessageParcel &data, MessageParcel &reply);
    ErrCode OnReadNdefData(uint32_t code, MessageParcel &data, MessageParcel &reply);
    ErrCode OnWriteNdefData(uint32_t code, MessageParcel &data, MessageParcel &reply);
    ErrCode OnRegisterCallback(uint32_t code, MessageParcel &data, MessageParcel &reply);
    ErrCode OnUnRegisterCallback(uint32_t code, MessageParcel &data, MessageParcel &reply);
private:
    HandleFuncMap handleFuncMap;
};
}  // namespace NFC
}  // namespace OHOS
#endif
