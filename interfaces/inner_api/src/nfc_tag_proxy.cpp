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

#define LOG_TAG "NFCTAG_FWK_PROXY"
#include "nfc_tag_proxy.h"
#include "nfc_tag_log.h"
#include "nfc_tag_errcode.h"
#include "nfc_tag_callback_stub.h"

namespace OHOS {
namespace NFC {
static int NFC_TAG_MAX_LEN = 512;
NfcTagProxy::NfcTagProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<INfcTagService>(impl), remoteDied_(false)
{
    HILOGI("enter");
    if (impl) {
        if (impl->IsProxyObject() && impl->AddDeathRecipient(this)) {
            HILOGI("AddDeathRecipient!");
        } else {
            HILOGW("no recipient!");
        }
    }
}
NfcTagProxy::~NfcTagProxy()
{
}

ErrCode NfcTagProxy::Init()
{
    if (remoteDied_) {
        HILOGE("remote service is died!");
        return NFC_REMOTE_DIED;
    }
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOGE("WriteInterfaceToken failed");
        return NFC_IPC_WRITETOKE_FAILED;
    }

    int error = Remote()->SendRequest(NFC_TAG_CMD_INIT, data, reply, option);
    if (error != ERR_NONE) {
        HILOGE("Init failed, error code is %{public}d", error);
        return NFC_IPC_SEND_FAILED;
    }

    return ErrCode(reply.ReadInt32());
}
ErrCode NfcTagProxy::Uninit()
{
    if (remoteDied_) {
        HILOGE("remote service is died!");
        return NFC_REMOTE_DIED;
    }
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOGE("WriteInterfaceToken failed");
        return NFC_IPC_WRITETOKE_FAILED;
    }

    int error = Remote()->SendRequest(NFC_TAG_CMD_UNINIT, data, reply, option);
    if (error != ERR_NONE) {
        HILOGE("Uninit failed, error code is %{public}d", error);
        return NFC_IPC_SEND_FAILED;
    }

    return ErrCode(reply.ReadInt32());
}
ErrCode NfcTagProxy::ReadNdefTag(std::string &response)
{
    if (remoteDied_) {
        HILOGE("remote service is died!");
        return NFC_REMOTE_DIED;
    }
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOGE("WriteInterfaceToken failed");
        return NFC_IPC_WRITETOKE_FAILED;
    }

    int error = Remote()->SendRequest(NFC_TAG_CMD_READ_NDEF_TAG, data, reply, option);
    if (error != ERR_NONE) {
        HILOGE("ReadNdefTag failed, error code is %{public}d", error);
        return NFC_IPC_SEND_FAILED;
    }
    int result = reply.ReadInt32();
    if (!reply.ReadString(response)) {
        HILOGE("ipc ReadString failed");
        return NFC_IPC_READ_FAILED;
    }
    return ErrCode(result);
}
ErrCode NfcTagProxy::WriteNdefTag(const std::string &tagData)
{
    if (remoteDied_) {
        HILOGE("remote service is died!");
        return NFC_REMOTE_DIED;
    }
    if (tagData.length() == 0) {
        HILOGE("invalid length");
        return NFC_INVALID_PARAMETER;
    }
    MessageOption option;
    MessageParcel data;
    MessageParcel reply;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOGE("WriteInterfaceToken failed");
        return NFC_IPC_WRITETOKE_FAILED;
    }
    if (!data.WriteString(tagData)) {
        HILOGE("WriteString failed");
        return NFC_IPC_WRITE_FAILED;
    }
    HILOGI("data len %{public}lu", tagData.length());

    int error = Remote()->SendRequest(NFC_TAG_CMD_WRITE_NDEF_TAG, data, reply, option);
    if (error != ERR_NONE) {
        HILOGE("SendRequest failed,error code is %{public}d", error);
        return NFC_IPC_SEND_FAILED;
    }

    return ErrCode(reply.ReadInt32());
}

ErrCode NfcTagProxy::ReadNdefData(std::vector<uint8_t> &data)
{
    if (remoteDied_) {
        HILOGE("remote service is died!");
        return NFC_REMOTE_DIED;
    }
    MessageOption option;
    MessageParcel parcelData;
    MessageParcel reply;
    if (!parcelData.WriteInterfaceToken(GetDescriptor())) {
        HILOGE("WriteInterfaceToken failed");
        return NFC_IPC_WRITETOKE_FAILED;
    }

    int error = Remote()->SendRequest(NFC_TAG_CMD_READ_NDEF_DATA, parcelData, reply, option);
    if (error != ERR_NONE) {
        HILOGE("ReadNdefTag failed, error code is %{public}d", error);
        return NFC_IPC_SEND_FAILED;
    }

    int result = reply.ReadInt32();
    if (!reply.ReadUInt8Vector(&data)) {
        HILOGE("ReadUInt8Vector random error");
        return NFC_IPC_READ_FAILED;
    }
    return ErrCode(result);
}

ErrCode NfcTagProxy::WriteNdefData(const std::vector<uint8_t> &data)
{
    if (remoteDied_) {
        HILOGE("remote service is died!");
        return NFC_REMOTE_DIED;
    }
    if (data.size() == 0 || data.size() > NFC_TAG_MAX_LEN) {
        HILOGE("invalid size");
        return NFC_INVALID_PARAMETER;
    }
    MessageOption option;
    MessageParcel parcelData;
    MessageParcel reply;
    if (!parcelData.WriteInterfaceToken(GetDescriptor())) {
        HILOGE("WriteInterfaceToken failed");
        return NFC_IPC_WRITETOKE_FAILED;
    }
    if (!parcelData.WriteUInt8Vector(data)) {
        HILOGE("WriteUInt8Vector failed");
        return NFC_IPC_WRITE_FAILED;
    }

    int error = Remote()->SendRequest(NFC_TAG_CMD_WRITE_NDEF_DATA, parcelData, reply, option);
    if (error != ERR_NONE) {
        HILOGE("SendRequest failed,error code is %{public}d", error);
        return NFC_IPC_SEND_FAILED;
    }

    return ErrCode(reply.ReadInt32());
}

ErrCode NfcTagProxy::RegListener(const sptr<INfcTagCallback> &callback)
{
    if (remoteDied_) {
        HILOGE("remote service is died!");
        return NFC_REMOTE_DIED;
    }
    if (callback == nullptr) {
        HILOGE("proxy callback is null");
        return NFC_INVALID_PARAMETER;
    }
    if (callback->AsObject() == nullptr) {
        HILOGE("callback->AsObject() == nullptr");
        return NFC_INVALID_PARAMETER;
    }
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOGE("RegListener WriteInterfaceToken error");
        return NFC_IPC_WRITETOKE_FAILED;
    }
    if (!data.WriteRemoteObject(callback->AsObject())) {
        HILOGE("proxy write callback failed");
        return NFC_IPC_WRITE_FAILED;
    }
    sptr<IRemoteObject> remote = Remote();
    if (remote == nullptr) {
        HILOGE("can't get remote");
        return NFC_NO_REMOTE;
    }

    int32_t ret = remote->SendRequest(NFC_TAG_CMD_REGISTER_CALLBACK, data, reply, option);
    if (ret != 0) {
        HILOGE("SendRequest failed, error number is %{public}d", ret);
        return NFC_IPC_SEND_FAILED;
    }

    return ErrCode(reply.ReadInt32());
}
ErrCode NfcTagProxy::UnregListener(const sptr<INfcTagCallback> &callback)
{
    if (remoteDied_) {
        HILOGE("remote service is died!");
        return NFC_REMOTE_DIED;
    }
    if (callback == nullptr) {
        HILOGE("proxy callback is null");
        return NFC_INVALID_PARAMETER;
    }
    if (callback->AsObject() == nullptr) {
        HILOGE("callback->AsObject() == nullptr");
        return NFC_INVALID_PARAMETER;
    }
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        HILOGE("UnRegListener WriteInterfaceToken error");
        return NFC_IPC_WRITETOKE_FAILED;
    }
    if (!data.WriteRemoteObject(callback->AsObject())) {
        HILOGE("proxy write callback failed");
        return NFC_IPC_WRITE_FAILED;
    }
    sptr<IRemoteObject> remote = Remote();
    if (remote == nullptr) {
        HILOGE("can't get remote");
        return NFC_NO_REMOTE;
    }

    int32_t ret = remote->SendRequest(NFC_TAG_CMD_UNREGISTER_CALLBACK, data, reply, option);
    if (ret != 0) {
        HILOGE("SendRequest failed, error number is %{public}d", ret);
        return NFC_IPC_SEND_FAILED;
    }

    return ErrCode(reply.ReadInt32());
}

void NfcTagProxy::OnRemoteDied(const wptr<IRemoteObject> &remoteObject)
{
    HILOGE("Remote service is died!");
    remoteDied_ = true;
    sptr<IRemoteObject> remote = Remote();
    if (remote == nullptr) {
        HILOGE("can't get remote");
        return;
    }
    remote->RemoveDeathRecipient(this);
}
}  // namespace NFC
}  // namespace OHOS