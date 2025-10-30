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

#define LOG_TAG "NFCTAG_STUB"
#include "nfc_tag_stub.h"
#include "nfc_tag_log.h"
#include "nfc_tag_errcode.h"
#include "nfc_tag_callback_proxy.h"

namespace OHOS {
namespace NFC {

static int32_t NFC_TAG_MAX_LEN = 512;

NfcTagStub::NfcTagStub()
{
    HILOGI("enter");
    InitHandleMap();
}

NfcTagStub::~NfcTagStub()
{}

bool NfcTagStub::InitHandleMap()
{
    handleFuncMap[NFC_TAG_CMD_INIT] = &NfcTagStub::OnInit;
    handleFuncMap[NFC_TAG_CMD_UNINIT] = &NfcTagStub::OnUninit;
    handleFuncMap[NFC_TAG_CMD_READ_NDEF_TAG] = &NfcTagStub::OnReadNdefTag;
    handleFuncMap[NFC_TAG_CMD_WRITE_NDEF_TAG] = &NfcTagStub::OnWriteNdefTag;
    handleFuncMap[NFC_TAG_CMD_READ_NDEF_DATA] = &NfcTagStub::OnReadNdefData;
    handleFuncMap[NFC_TAG_CMD_WRITE_NDEF_DATA] = &NfcTagStub::OnWriteNdefData;
    handleFuncMap[NFC_TAG_CMD_REGISTER_CALLBACK] = &NfcTagStub::OnRegisterCallback;
    handleFuncMap[NFC_TAG_CMD_UNREGISTER_CALLBACK] = &NfcTagStub::OnUnRegisterCallback;
    return true;
}

int NfcTagStub::OnRemoteRequest(uint32_t code, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    HILOGI("code: %{public}d", code);
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        HILOGW("invalid token Descriptor");
        return static_cast<int>(NFC_INVALID_TOKEN);
    }

    HandleFuncMap::iterator iter = handleFuncMap.find(code);
    if (iter != handleFuncMap.end()) {
        ErrCode ret = (this->*(iter->second))(code, data, reply);
        if (ret != NFC_SUCCESS) {
            HILOGE("code: %{public}d, ret: %{public}d", code, ret);
        }
        return static_cast<int>(ret);
    }
    HILOGE("not find function to deal, code %{public}u", code);
    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
}

ErrCode NfcTagStub::OnInit(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    HILOGI("code %{public}u", code);
    ErrCode ret = Init();
    reply.WriteInt32(static_cast<int32_t>(ret));
    return ret;
}

ErrCode NfcTagStub::OnUninit(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    HILOGI("code %{public}u", code);
    ErrCode ret = Uninit();
    reply.WriteInt32(static_cast<int32_t>(ret));
    return ret;
}

ErrCode NfcTagStub::OnReadNdefTag(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    HILOGI("code %{public}u, datasize %{public}zu", code, data.GetRawDataSize());
    std::string response;
    ErrCode ret = ReadNdefTag(response);
    reply.WriteInt32(static_cast<int32_t>(ret));
    reply.WriteString(response);
    return ret;
}

ErrCode NfcTagStub::OnWriteNdefTag(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    std::string dataToWrite;
    if (!data.ReadString(dataToWrite)) {
        HILOGE("ipc ReadString failed");
        return NFC_IPC_READ_FAILED;
    }
    int32_t dataLen = static_cast<int32_t>(dataToWrite.length());
    HILOGI("datasize %{public}zu, dataLen = %{public}d", data.GetRawDataSize(), dataLen);
    if (dataLen > NFC_TAG_MAX_LEN || dataLen <= 0) {
        return NFC_INVALID_PARAMETER;
    }
    ErrCode ret = WriteNdefTag(dataToWrite);
    reply.WriteInt32(dataLen);
    return ret;
}

ErrCode NfcTagStub::OnReadNdefData(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    HILOGI("code %{public}u, datasize %{public}zu", code, data.GetRawDataSize());
    std::vector<uint8_t> response;
    ErrCode ret = ReadNdefData(response);
    reply.WriteInt32(static_cast<int32_t>(ret));
    reply.WriteUInt8Vector(response);
    return ret;
}

ErrCode NfcTagStub::OnWriteNdefData(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    std::vector<uint8_t> dataToWrite;
    if (!data.ReadUInt8Vector(&dataToWrite)) {
        HILOGE("ReadUInt8Vector failed");
        reply.WriteInt32(NFC_IPC_READ_FAILED);
        return NFC_IPC_READ_FAILED;
    }
    int32_t size = static_cast<int32_t>(dataToWrite.size());
    HILOGI("datasize %{public}zu, size = %{public}d",
        data.GetRawDataSize(), size);
    if (size > NFC_TAG_MAX_LEN || size <= 0) {
        return NFC_INVALID_PARAMETER;
    }
    ErrCode ret = WriteNdefData(dataToWrite);
    reply.WriteInt32(static_cast<int32_t>(ret));
    return ret;
}

ErrCode NfcTagStub::OnRegisterCallback(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    sptr<IRemoteObject> object = data.ReadRemoteObject();
    if (object == nullptr) {
        HILOGE("receive remote object is null");
        reply.WriteInt32(NFC_NO_OBJECT);
        return NFC_NO_OBJECT;
    }
    sptr<INfcTagCallback> listener = iface_cast<INfcTagCallback>(object);
    if (listener == nullptr) {
        HILOGE("listener is nullptr");
        reply.WriteInt32(NFC_NO_CALLBACK);
        return NFC_NO_CALLBACK;
    }
    ErrCode ret = RegListener(listener);
    reply.WriteInt32(static_cast<int32_t>(ret));
    return ret;
}

ErrCode NfcTagStub::OnUnRegisterCallback(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    sptr<IRemoteObject> object = data.ReadRemoteObject();
    if (object == nullptr) {
        HILOGE("receive remote object is null");
        reply.WriteInt32(NFC_NO_OBJECT);
        return NFC_NO_OBJECT;
    }
    sptr<INfcTagCallback> listener = iface_cast<INfcTagCallback>(object);
    if (listener == nullptr) {
        HILOGE("listener is nullptr");
        reply.WriteInt32(NFC_NO_CALLBACK);
        return NFC_NO_CALLBACK;
    }
    ErrCode ret = UnregListener(listener);
    reply.WriteInt32(static_cast<int32_t>(ret));
    return ret;
}

}  // namespace NFC
}  // namespace OHOS