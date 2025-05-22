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

#define LOG_TAG "NFCTAG_FUZZER"
#include "nfc_tag_service_fuzzer.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "nfc_tag_log.h"
#include "nfc_tag_service.h"
#include "nfc_tag_callback_stub.h"
#include "nfc_tag_access_token_mock.h"

namespace OHOS {
namespace NFC {

class NfcTagCallbackStubTest: public NfcTagCallbackStub {
public:
    ErrCode OnNotify(int nfcRfState) override
    {
        return NFC_SUCCESS;
    }
};

static sptr<NfcTagService> g_nfcTagService = nullptr;
static sptr<NfcTagCallbackStubTest> g_nfcTagCallback = new NfcTagCallbackStubTest();

bool OnStartFuzztest(const uint8_t* data, size_t size)
{
    if (g_nfcTagService == nullptr) {
        g_nfcTagService =
            new NfcTagService(NFC_CONNECTED_TAG_ABILITY_ID, true);
        g_nfcTagService->OnStart();
    }
    return true;
}

bool SendRequest(MessageParcel &datas, uint32_t code)
{
    MessageParcel reply;
    MessageOption option;
    g_nfcTagService->OnRemoteRequest(code, datas, reply, option);
    int exception = reply.ReadInt32();
    if (exception != static_cast<int>(NFC_SUCCESS)) {
        HILOGI("exception: %{public}d", exception);
        return false;
    }
    HILOGI("OnRemoteRequest success");
    return true;
}

bool UnknownFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    MessageParcel datas;
    std::u16string descriptor = NfcTagStub::GetDescriptor();
    datas.WriteInterfaceToken(descriptor);
    uint32_t code = -1;
    return SendRequest(datas, code);
}

bool InitCmdFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    MessageParcel datas;
    std::u16string descriptor = NfcTagStub::GetDescriptor();
    datas.WriteInterfaceToken(descriptor);
    return SendRequest(datas, INfcTagService::NFC_TAG_CMD_INIT);
}

bool UnInitCmdFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    MessageParcel datas;
    std::u16string descriptor = NfcTagStub::GetDescriptor();
    datas.WriteInterfaceToken(descriptor);
    return SendRequest(datas, INfcTagService::NFC_TAG_CMD_UNINIT);
}

bool ReadNdefTagFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    MessageParcel datas;
    std::u16string descriptor = NfcTagStub::GetDescriptor();
    datas.WriteInterfaceToken(descriptor);
    return SendRequest(datas, INfcTagService::NFC_TAG_CMD_READ_NDEF_TAG);
}

bool WriteNdefTagFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    MessageParcel datas;
    std::u16string descriptor = NfcTagStub::GetDescriptor();
    datas.WriteInterfaceToken(descriptor);
    std::string tag(data, data + size);
    datas.WriteString(tag);
    return SendRequest(datas, INfcTagService::NFC_TAG_CMD_WRITE_NDEF_TAG);
}

bool ReadNdefDataFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    MessageParcel datas;
    std::u16string descriptor = NfcTagStub::GetDescriptor();
    datas.WriteInterfaceToken(descriptor);
    return SendRequest(datas, INfcTagService::NFC_TAG_CMD_READ_NDEF_DATA);
}

bool WriteNdefDataFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    MessageParcel datas;
    std::u16string descriptor = NfcTagStub::GetDescriptor();
    datas.WriteInterfaceToken(descriptor);
    std::vector<uint8_t> tag(data, data + size);
    datas.WriteUInt8Vector(tag);
    return SendRequest(datas, INfcTagService::NFC_TAG_CMD_WRITE_NDEF_DATA);
}

bool RegisterCallbackFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    MessageParcel datas;
    std::u16string descriptor = NfcTagStub::GetDescriptor();
    datas.WriteInterfaceToken(descriptor);
    std::vector<uint8_t> tag(data, data + size);
    datas.WriteRemoteObject(g_nfcTagCallback->AsObject());
    return SendRequest(datas, INfcTagService::NFC_TAG_CMD_REGISTER_CALLBACK);
}

bool UnRegisterCallbackFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    MessageParcel datas;
    std::u16string descriptor = NfcTagStub::GetDescriptor();
    datas.WriteInterfaceToken(descriptor);
    std::vector<uint8_t> tag(data, data + size);
    datas.WriteRemoteObject(g_nfcTagCallback->AsObject());
    return SendRequest(datas, INfcTagService::NFC_TAG_CMD_UNREGISTER_CALLBACK);
}

}  // namespace NFC
}  // namespace OHOS

 /* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    OHOS::NFC::NfcTagAccessTokenMock::SetNativeTokenInfo();
    OHOS::NFC::OnStartFuzztest(data, size);
    OHOS::NFC::UnknownFuzztest(data, size);
    OHOS::NFC::InitCmdFuzztest(data, size);
    OHOS::NFC::UnInitCmdFuzztest(data, size);
    OHOS::NFC::ReadNdefTagFuzztest(data, size);
    OHOS::NFC::WriteNdefTagFuzztest(data, size);
    OHOS::NFC::ReadNdefDataFuzztest(data, size);
    OHOS::NFC::WriteNdefDataFuzztest(data, size);
    OHOS::NFC::RegisterCallbackFuzztest(data, size);
    OHOS::NFC::UnRegisterCallbackFuzztest(data, size);
    return 0;
}