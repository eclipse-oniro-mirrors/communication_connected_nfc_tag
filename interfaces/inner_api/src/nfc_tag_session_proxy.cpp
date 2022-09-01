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
#include "nfc_tag_session_proxy.h"
#include "define.h"
#include "log.h"
#include "nfc_tag_callback_stub.h"

namespace OHOS {
namespace NFC {
NfcTagSessionProxy::NfcTagSessionProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<INfcTagSession>(impl), mRemoteDied(false)
{
    HILOGI("NfcTagSessionProxy() in!");
    if (impl) {
        if ((impl->IsProxyObject()) && (!impl->AddDeathRecipient(this))) {
            HILOGD("AddDeathRecipient!");
        } else {
            HILOGW("no recipient!");
        }
    }
}
NfcTagSessionProxy::~NfcTagSessionProxy()
{
}

ErrCode NfcTagSessionProxy::Init()
{
    if (mRemoteDied) {
        HILOGE("failed to `%{public}s`,remote service is died!", __func__);
        return NFC_FAILED;
    }
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        return NFC_FAILED;
    }
    data.WriteInt32(NFC_SUCCESS);

    int error = Remote()->SendRequest(NFC_SVR_CMD_INIT, data, reply, option);
    if (error != ERR_NONE) {
        HILOGE("Init failed, error code is %{public}d", error);
        return NFC_FAILED;
    }

    int exception = reply.ReadInt32();
    if (exception) {
        return NFC_FAILED;
    }
    return ErrCode(reply.ReadInt32());
}
ErrCode NfcTagSessionProxy::Uninit()
{
    if (mRemoteDied) {
        HILOGE("failed to `%{public}s`,remote service is died!", __func__);
        return NFC_FAILED;
    }
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        return NFC_FAILED;
    }
    data.WriteInt32(NFC_SUCCESS);

    int error = Remote()->SendRequest(NFC_SVR_CMD_UNINIT, data, reply, option);
    if (error != ERR_NONE) {
        HILOGE("Uninit failed, error code is %{public}d", error);
        return NFC_FAILED;
    }

    int exception = reply.ReadInt32();
    if (exception) {
        return NFC_FAILED;
    }
    return ErrCode(reply.ReadInt32());
}
ErrCode NfcTagSessionProxy::ReadNdefTag(std::string &response)
{
    if (mRemoteDied) {
        HILOGE("failed to `%{public}s`,remote service is died!", __func__);
        return NFC_FAILED;
    }
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        return NFC_FAILED;
    }
    data.WriteInt32(NFC_SUCCESS);

    int error = Remote()->SendRequest(NFC_SVR_CMD_READ_NDEF_TAG, data, reply, option);
    if (error != ERR_NONE) {
        HILOGE("ReadNdefTag failed, error code is %{public}d", error);
        return NFC_FAILED;
    }

    int exception = reply.ReadInt32();
    if (exception) {
        return NFC_FAILED;
    }
    response = reply.ReadString();
    return ErrCode(NFC_SUCCESS);
}
ErrCode NfcTagSessionProxy::WriteNdefTag(std::string tagData)
{
    if (mRemoteDied) {
        HILOGE("failed to `%{public}s`,remote service is died!", __func__);
        return NFC_FAILED;
    }
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        return NFC_FAILED;
    }
    data.WriteInt32(NFC_SUCCESS);
    data.WriteString(tagData);
    HILOGD("NfcTagSessionProxy WriteNdefTag tagData is %{public}s", tagData.c_str());

    int error = Remote()->SendRequest(NFC_SVR_CMD_WRITE_NDEF_TAG, data, reply, option);
    if (error != ERR_NONE) {
        HILOGE("WriteNdefTag failed,error code is %{public}d", error);
        return NFC_FAILED;
    }

    int exception = reply.ReadInt32();
    if (exception) {
        return NFC_FAILED;
    }
    return ErrCode(reply.ReadInt32());
}
ErrCode NfcTagSessionProxy::RegListener(const sptr<INfcTagCallback> &callback)
{
    return OHOS::NFC::NfcTagCallbackStub::GetInstance().RegisterUserCallBack(callback);
}
ErrCode NfcTagSessionProxy::UnregListener(const sptr<INfcTagCallback> &callback)
{
    return OHOS::NFC::NfcTagCallbackStub::GetInstance().RegisterUserCallBack(nullptr);
}

void NfcTagSessionProxy::OnRemoteDied(const wptr<IRemoteObject> &remoteObject)
{
    HILOGD("Remote service is died!");
    mRemoteDied = true;
}
}  // namespace NFC
}  // namespace OHOS