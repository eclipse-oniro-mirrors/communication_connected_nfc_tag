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

#define LOG_TAG "NFCTAG_FWK_STUB"
#include "nfc_tag_callback_stub.h"
#include "nfc_tag_log.h"
#include "nfc_tag_errcode.h"

namespace OHOS {
namespace NFC {
static constexpr size_t MAX_CALLBACK_NUM = 30;

NfcTagCallbackStub::NfcTagCallbackStub()
{}

NfcTagCallbackStub::~NfcTagCallbackStub()
{}

bool NfcTagCallbackStub::IsUserCallbackEmpty()
{
    std::shared_lock<std::shared_mutex> guard(callbackMutex_);
    return callbackStore_.empty();
}

ErrCode NfcTagCallbackStub::OnNotify(int nfcRfState)
{
    HILOGI("nfcRfState: %{public}d", nfcRfState);
    std::shared_lock<std::shared_mutex> guard(callbackMutex_);
    for (auto &item : callbackStore_) {
        if (item) {
            item->OnNotify(nfcRfState);
        }
    }
    return NFC_SUCCESS;
}

int NfcTagCallbackStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    HILOGD("enter");
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        HILOGE("invalid interface token");
        return static_cast<int>(NFC_INVALID_TOKEN);
    }

    int ret = static_cast<int>(NFC_SUCCESS);
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
    if (callBack == nullptr) {
        HILOGW("callBack is nullptr!");
        return NFC_NO_CALLBACK;
    }
    std::unique_lock<std::shared_mutex> guard(callbackMutex_);
    if (std::find(callbackStore_.begin(), callbackStore_.end(), callBack) != callbackStore_.end()) {
        HILOGW("this callback has been registered!");
        return NFC_CALLBACK_REGISTERED;
    }
    if (callbackStore_.size() >= MAX_CALLBACK_NUM) {
        HILOGE("too many callback, %{public}lu", callbackStore_.size());
        return NFC_TOO_MANY_CALLBACK;
    }
    callbackStore_.emplace_back(callBack);
    return NFC_SUCCESS;
}

ErrCode NfcTagCallbackStub::UnRegisterUserCallBack(const sptr<INfcTagCallback> &callBack)
{
    if (callBack == nullptr) {
        HILOGW("callBack is nullptr!");
        return NFC_NO_CALLBACK;
    }
    std::unique_lock<std::shared_mutex> guard(callbackMutex_);
    auto item = std::find(callbackStore_.begin(), callbackStore_.end(), callBack);
    if (item != callbackStore_.end()) {
        HILOGW("will remove this callback!");
        callbackStore_.erase(item);
        return NFC_SUCCESS;
    }
    HILOGW("this callback hasn't been registered!");
    return NFC_CALLBACK_NOT_REGISTERED;
}

int NfcTagCallbackStub::RemoteOnNotify(MessageParcel &data, MessageParcel &reply)
{
    int state = data.ReadInt32();
    ErrCode ret = OnNotify(state);
    reply.WriteInt32(static_cast<int32_t>(ret)); /* Reply 0 to indicate that no exception occurs. */
    return static_cast<int>(ret);
}
}  // namespace NFC
}  // namespace OHOS