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

#include "nfc_tag_callback_stub.h"
#include "common_mock.h"

namespace OHOS {
namespace NFC {

NfcTagCallbackStub::NfcTagCallbackStub()
{}

NfcTagCallbackStub::~NfcTagCallbackStub()
{}

ErrCode NfcTagCallbackStub::RegisterUserCallBack(const sptr<INfcTagCallback> &callBack)
{
    return NFC_SUCCESS;
}

ErrCode NfcTagCallbackStub::UnRegisterUserCallBack(const sptr<INfcTagCallback> &callBack)
{
    return NFC_SUCCESS;
}

ErrCode NfcTagCallbackStub::OnNotify(int nfcRfState)
{
    return NFC_SUCCESS;
}

bool NfcTagCallbackStub::IsUserCallbackEmpty()
{
    return true;
}

int NfcTagCallbackStub::RemoteOnNotify(MessageParcel &data, MessageParcel &reply)
{
    return 0;
}

int NfcTagCallbackStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    return IPCObjectStub::onRemoteRequestRetMock_;
}

} // namespace NFC
} // namespace OHOS