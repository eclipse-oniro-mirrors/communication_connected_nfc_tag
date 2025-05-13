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

#define LOG_TAG "NFCTAG_CB_PROXY"
#include "nfc_tag_callback_proxy.h"
#include "nfc_tag_log.h"
#include "nfc_tag_errcode.h"

namespace OHOS {
namespace NFC {
NfcTagCallBackProxy::NfcTagCallBackProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<INfcTagCallback>(impl)
{}

ErrCode NfcTagCallBackProxy::OnNotify(int nfcRfState)
{
    HILOGD("OnNotify");
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOGE("WriteInterfaceToken failed!");
        return NFC_IPC_WRITETOKE_FAILED;
    }
    if (!data.WriteInt32(nfcRfState)) {
        HILOGE("WriteInt32 failed!");
        return NFC_IPC_WRITE_FAILED;
    }

    int error = Remote()->SendRequest(CMD_ON_NFC_TAG_NOTIFY, data, reply, option);
    if (error != ERR_NONE) {
        HILOGE("Set Attr(%{public}d) failed,error code is %{public}d", CMD_ON_NFC_TAG_NOTIFY, error);
        return NFC_IPC_SEND_FAILED;
    }
    int exception;
    if (!reply.ReadInt32(exception)) {
        HILOGE("notify CMD_ON_NFC_TAG_NOTIFY state change failed!, exception: %{public}d", exception);
        return NFC_IPC_READ_FAILED;
    }
    return NFC_SUCCESS;
}
}  // namespace NFC
}  // namespace OHOS