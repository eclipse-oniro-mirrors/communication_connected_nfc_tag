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
#include "nfc_tag_hdi_adapter.h"
#include "log.h"
#include "v1_0/iconnected_nfc_tag.h"

using namespace OHOS::HDI::ConnectedNfcTag;
using IConnectedNfcTagV1_0 = OHOS::HDI::ConnectedNfcTag::V1_0::IConnectedNfcTag;

namespace OHOS {
namespace NFC {
static sptr<IConnectedNfcTagV1_0> g_nfcTagHdi;
NfcTagHdiAdapter::NfcTagHdiAdapter()
{
    HILOGI("NfcTagHdiAdapter: NfcTagHdiAdapter called.");
    sptr<IConnectedNfcTagV1_0> nfcHdi = IConnectedNfcTagV1_0::Get(true);
    if (nfcHdi == nullptr) {
        HILOGE("NfcTagHdiAdapter: IConnectedTagHdi::Get failed.");
    }
    g_nfcTagHdi = nfcHdi;
}

NfcTagHdiAdapter::~NfcTagHdiAdapter()
{
    HILOGI("NfcTagHdiAdapter: ~NfcTagHdiAdapter called.");
}
NfcTagHdiAdapter &NfcTagHdiAdapter::GetInstance()
{
    static NfcTagHdiAdapter sNfcTagHdiAdapter;
    return sNfcTagHdiAdapter;
}
int32_t NfcTagHdiAdapter::Init()
{
    HILOGI("NfcTagHdiAdapter::Init() starts");
    if (g_nfcTagHdi != nullptr) {
        g_nfcTagHdi->Init();
    }
    return 0;
}
int32_t NfcTagHdiAdapter::Uninit()
{
    HILOGI("NfcTagHdiAdapter::Uninit() starts");
    if (g_nfcTagHdi != nullptr) {
        g_nfcTagHdi->Uninit();
    }
    return 0;
}
std::string NfcTagHdiAdapter::ReadNdefTag()
{
    HILOGI("NfcTagHdiAdapter::ReadNdefTag() starts");
    std::string resp = "";
    if (g_nfcTagHdi != nullptr) {
        g_nfcTagHdi->ReadNdefTag(resp);
        HILOGI("NfcTagHdiAdapter::ReadNdefTag() resp = %{public}s", resp.c_str());
    }
    return resp;
}
int32_t NfcTagHdiAdapter::WriteNdefTag(std::string data)
{
    HILOGI("NfcTagHdiAdapter::WriteNdefTag() starts data = %{public}s", data.c_str());
    if (g_nfcTagHdi != nullptr) {
        g_nfcTagHdi->WriteNdefTag(data);
    }
    return 0;
}
}  // namespace NFC
}  // namespace OHOS
