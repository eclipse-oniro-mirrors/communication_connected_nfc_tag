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
#include "nfc_tag_service_impl.h"
#include "define.h"
#include "log.h"
#include "nfc_tag_hdi_adapter.h"
#include "permission_tools.h"

namespace OHOS {
namespace NFC {
const bool REGISTER_RESULT =
    SystemAbility::MakeAndRegisterAbility(NfcTagServiceImpl::GetInstance().GetRefPtr());

sptr<NfcTagServiceImpl> NfcTagServiceImpl::GetInstance()
{
    static NfcTagServiceImpl sNfcTagServiceImpl;
    return &sNfcTagServiceImpl;
}

NfcTagServiceImpl::NfcTagServiceImpl()
    : SystemAbility(NFC_CONNECTED_TAG_ABILITY_ID, true), mPublishFlag(false),
    mState(ServiceRunningState::STATE_NOT_START)
{
}

NfcTagServiceImpl::~NfcTagServiceImpl()
{
}

void NfcTagServiceImpl::OnStart()
{
    HILOGI("NfcTagServiceImpl::OnStart() in");
    if (mState == ServiceRunningState::STATE_RUNNING) {
        HILOGI("Service has already started.");
        return;
    }
    if (!ServiceInit()) {
        HILOGE("Failed to init service");
        OnStop();
        return;
    }
    mState = ServiceRunningState::STATE_RUNNING;
    HILOGI("Start service!");
}

void NfcTagServiceImpl::OnStop()
{
    HILOGI("NfcTagServiceImpl::OnStop() in");
    mState = ServiceRunningState::STATE_NOT_START;
    mPublishFlag = false;
    HILOGI("Stop service!");
}

bool NfcTagServiceImpl::ServiceInit()
{
    HILOGI("NfcTagServiceImpl::ServiceInit() in!");
    if (!mPublishFlag) {
        bool ret = Publish(NfcTagServiceImpl::GetInstance());
        if (!ret) {
            HILOGE("Failed to publish service!");
            return false;
        }
        mPublishFlag = true;
    }
    return true;
}

ErrCode NfcTagServiceImpl::Init()
{
    HILOGE("NfcTagServiceImpl:Init() in!");
    if (!PermissionTools::IsGranted(OHOS::NFC::TAG_PERMISSION)) {
        HILOGE("NfcTagServiceImpl:Init() IsGranted failed!");
        return NFC_FAILED;
    }
    if (NfcTagHdiAdapter::GetInstance().Init() == 0) {
        return NFC_SUCCESS;
    }
    return NFC_FAILED;
}

ErrCode NfcTagServiceImpl::Uninit()
{
    if (!PermissionTools::IsGranted(OHOS::NFC::TAG_PERMISSION)) {
        HILOGE("NfcTagServiceImpl:Uninit() IsGranted failed!");
        return NFC_FAILED;
    }
    if (NfcTagHdiAdapter::GetInstance().Uninit() == 0) {
        return NFC_SUCCESS;
    }
    return NFC_FAILED;
}

ErrCode NfcTagServiceImpl::ReadNdefTag(std::string &response)
{
    if (!PermissionTools::IsGranted(OHOS::NFC::TAG_PERMISSION)) {
        HILOGE("NfcTagServiceImpl:ReadNdefTag() IsGranted failed!");
        return NFC_FAILED;
    }
    response = NfcTagHdiAdapter::GetInstance().ReadNdefTag();
    return NFC_SUCCESS;
}

ErrCode NfcTagServiceImpl::WriteNdefTag(std::string data)
{
    if (!PermissionTools::IsGranted(OHOS::NFC::TAG_PERMISSION)) {
        HILOGE("NfcTagServiceImpl:WriteNdefTag() IsGranted failed!");
        return NFC_FAILED;
    }
    if (NfcTagHdiAdapter::GetInstance().WriteNdefTag(data) == 0) {
        return NFC_SUCCESS;
    }
    return NFC_FAILED;
}

ErrCode NfcTagServiceImpl::RegListener(const sptr<INfcTagCallback> &callback)
{
    if (!PermissionTools::IsGranted(OHOS::NFC::TAG_PERMISSION)) {
        HILOGE("NfcTagServiceImpl:RegListener() IsGranted failed!");
        return NFC_FAILED;
    }
    return NFC_SUCCESS;
}
ErrCode NfcTagServiceImpl::UnregListener(const sptr<INfcTagCallback> &callback)
{
    if (!PermissionTools::IsGranted(OHOS::NFC::TAG_PERMISSION)) {
        HILOGE("NfcTagServiceImpl:UnregListener() IsGranted failed!");
        return NFC_FAILED;
    }
    return NFC_SUCCESS;
}
}  // namespace NFC
}  // namespace OHOS