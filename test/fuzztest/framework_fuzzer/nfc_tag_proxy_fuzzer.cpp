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
#include "nfc_tag_proxy_fuzzer.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "nfc_tag_log.h"
#include "nfc_tag_service.h"
#include "nfc_tag_callback_stub.h"
#include "nfc_tag_proxy.h"
#include "nfc_tag_access_token_mock.h"

namespace OHOS {
namespace NFC {

class NfcTagCallbackStubTest : public NfcTagCallbackStub {
public:
    ErrCode OnNotify(int nfcRfState) override
    {
        return NFC_SUCCESS;
    }
};

static sptr<NfcTagService> g_nfcTagService = nullptr;
static sptr<NfcTagCallbackStubTest> g_nfcTagCallback = new NfcTagCallbackStubTest();
static sptr<NfcTagProxy> g_nfcTagProxy = nullptr;
bool OnStartFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (g_nfcTagService == nullptr) {
        g_nfcTagService =
            new NfcTagService(NFC_CONNECTED_TAG_ABILITY_ID, true);
        g_nfcTagService->OnStart();
        g_nfcTagProxy = new NfcTagProxy(g_nfcTagService);
    }
    return true;
}

bool InitCmdFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    return g_nfcTagProxy->Init();
}

bool UnInitCmdFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    return g_nfcTagProxy->Uninit();
}

bool ReadNdefTagFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    std::string resp;
    return g_nfcTagProxy->ReadNdefTag(resp);
}

bool WriteNdefTagFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    std::string tag(data, data + size);
    return g_nfcTagProxy->WriteNdefTag(tag);
}

bool ReadNdefDataFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    std::vector<uint8_t> resp;
    return g_nfcTagProxy->ReadNdefData(resp);
}

bool WriteNdefDataFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    std::vector<uint8_t> tag(data, data + size);
    return g_nfcTagProxy->WriteNdefData(tag);
}

bool RegisterCallbackFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }

    return g_nfcTagProxy->RegListener(g_nfcTagCallback);
}

bool UnRegisterCallbackFuzztest(const uint8_t* data, size_t size)
{
    HILOGI("start");
    if (data == nullptr) {
        return false;
    }
    return g_nfcTagProxy->UnregListener(g_nfcTagCallback);
}

}  // namespace NFC
}  // namespace OHOS

 /* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    OHOS::NFC::NfcTagAccessTokenMock::SetNativeTokenInfo();
    OHOS::NFC::OnStartFuzztest(data, size);
    OHOS::NFC::InitCmdFuzztest(data, size);
    OHOS::NFC::ReadNdefTagFuzztest(data, size);
    OHOS::NFC::WriteNdefTagFuzztest(data, size);
    OHOS::NFC::ReadNdefDataFuzztest(data, size);
    OHOS::NFC::WriteNdefDataFuzztest(data, size);
    OHOS::NFC::RegisterCallbackFuzztest(data, size);
    OHOS::NFC::UnRegisterCallbackFuzztest(data, size);
    OHOS::NFC::UnInitCmdFuzztest(data, size);
    return 0;
}