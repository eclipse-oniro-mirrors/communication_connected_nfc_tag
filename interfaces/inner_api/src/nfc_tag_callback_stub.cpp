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
#include "nfc_tag_callback_stub.h"
#include "define.h"
#include "log.h"

namespace OHOS {
namespace NFC {
NfcTagCallbackStub::NfcTagCallbackStub() : callback_(nullptr), mRemoteDied(false)
{}

NfcTagCallbackStub::~NfcTagCallbackStub()
{}

NfcTagCallbackStub& NfcTagCallbackStub::GetInstance()
{
    static NfcTagCallbackStub sConnectedTagCallBackStub;
    return sConnectedTagCallBackStub;
}

void NfcTagCallbackStub::OnNotify(int nfcRfState)
{
    HILOGD("NfcTagCallbackStub::OnNotify");
    if (callback_) {
        callback_->OnNotify(nfcRfState);
    }
}

int NfcTagCallbackStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    HILOGD("NfcTagCallbackStub::OnRemoteRequest!");
    if (mRemoteDied) {
        HILOGE("Failed to `%{public}s`,Remote service is died!", __func__);
        return NFC_FAILED;
    }
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        HILOGE("NfcTagCallbackStub::OnRemoteRequest GetDescriptor error.");
        return NFC_FAILED;
    }
    int exception = data.ReadInt32();
    if (exception) {
        HILOGE("NfcTagCallbackStub::OnRemoteRequest, got exception: %{public}d!", exception);
        return NFC_FAILED;
    }
    int ret = NFC_FAILED;
    switch (code) {
        case CMD_ON_NFC_TAG_NOTIFY: {
            ret = RemoteOnNotify(data, reply);
            break;
        }
        default: {
            ret = IPCObjectStub::OnRemoteRequest(code, data, reply, option);
            break;
        }
    }
    return ret;
}

ErrCode NfcTagCallbackStub::RegisterUserCallBack(const sptr<INfcTagCallback> &callBack)
{
    std::shared_lock<std::shared_mutex> guard(callbackMutex);
    if (callBack == nullptr) {
        HILOGW("RegisterUserCallBack:callBack is nullptr!");
    }
    callback_ = callBack;
    return NFC_SUCCESS;
}

int NfcTagCallbackStub::RemoteOnNotify(MessageParcel &data, MessageParcel &reply)
{
    HILOGD("run %{public}s datasize %{public}zu", __func__, data.GetRawDataSize());
    int state = data.ReadInt32();
    std::shared_lock<std::shared_mutex> guard(callbackMutex);
    if (callback_) {
        callback_->OnNotify(state);
    }
    reply.WriteInt32(NFC_SUCCESS); /* Reply 0 to indicate that no exception occurs. */
    return NFC_SUCCESS;
}
}  // namespace NFC
}  // namespace OHOS