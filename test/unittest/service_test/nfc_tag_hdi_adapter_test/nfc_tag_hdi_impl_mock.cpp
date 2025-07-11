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

#include "nfc_tag_hdi_impl.h"

namespace OHOS {
namespace NFC {

static ErrCode g_errCodeMock = NFC_SUCCESS;

void HdiImplReturnMock(ErrCode errCode)
{
    g_errCodeMock = errCode;
}

void NfcTagDriverStatusListener::OnReceive(const OHOS::HDI::ServiceManager::V1_0::ServiceStatus &status)
{}

NfcTagHdiCallBack::~NfcTagHdiCallBack()
{}

ErrCode NfcTagHdiCallBack::RegisterUpperCallBack(sptr<INfcTagCallback> listener)
{
    return g_errCodeMock;
}
ErrCode NfcTagHdiCallBack::UnRegisterUpperCallBack(sptr<INfcTagCallback> listener)
{
    return g_errCodeMock;
}
int32_t NfcTagHdiCallBack::OnChipEvent(HDI::ConnectedNfcTag::V1_1::ConnectedNfcTagEvent event,
    const std::vector<uint8_t>& message)
{
    return 0;
}

NfcTagHdiImpl& NfcTagHdiImpl::GetInstance()
{
    static NfcTagHdiImpl sNfcTagHdiImpl;
    return sNfcTagHdiImpl;
}

NfcTagHdiImpl::NfcTagHdiImpl()
{}

NfcTagHdiImpl::~NfcTagHdiImpl()
{}

ErrCode NfcTagHdiImpl::Init()
{
    return g_errCodeMock;
}

ErrCode NfcTagHdiImpl::Uninit()
{
    return g_errCodeMock;
}

ErrCode NfcTagHdiImpl::ReadNdefTag(std::string &data)
{
    return g_errCodeMock;
}

ErrCode NfcTagHdiImpl::WriteNdefTag(const std::string &data)
{
    return g_errCodeMock;
}

ErrCode NfcTagHdiImpl::ReadNdefData(std::vector<uint8_t> &data)
{
    return g_errCodeMock;
}

ErrCode NfcTagHdiImpl::WriteNdefData(const std::vector<uint8_t> &data)
{
    return g_errCodeMock;
}

ErrCode NfcTagHdiImpl::RegisterCallBack(sptr<INfcTagCallback> listener)
{
    return g_errCodeMock;
}

ErrCode NfcTagHdiImpl::UnRegisterCallBack(sptr<INfcTagCallback> listener)
{
    return g_errCodeMock;
}

ErrCode NfcTagHdiImpl::InitDriver()
{
    return g_errCodeMock;
}

void NfcTagHdiImpl::OnNfcTagDriverStart()
{}

void NfcTagHdiImpl::OnNfcTagDriverStop()
{}

sptr<OHOS::HDI::ConnectedNfcTag::V1_1::IConnectedNfcTag> NfcTagHdiImpl::GetProxy()
{
    return nfcTagProxy_;
}

}  // namespace NFC
}  // namespace OHOS
