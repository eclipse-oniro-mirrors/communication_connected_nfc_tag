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
#include "nfc_tag_impl.h"
#include "define.h"
#include "iservice_registry.h"
#include "log.h"

namespace OHOS {
namespace NFC {
NfcTagImpl::NfcTagImpl()
{
    HILOGI("NfcTagImpl() in");
    sptr<ISystemAbilityManager> sa_mgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (sa_mgr == nullptr) {
        HILOGE("failed to get SystemAbilityManager");
        return;
    }

    sptr<IRemoteObject> object = sa_mgr->GetSystemAbility(NFC_CONNECTED_TAG_ABILITY_ID);
    if (object == nullptr) {
        HILOGE("failed to get connected nfc tag SERVICE");
        return;
    }

    tagSessionProxy_ = iface_cast<INfcTagSession>(object);
    if (tagSessionProxy_ == nullptr) {
        tagSessionProxy_ = new (std::nothrow) NfcTagSessionProxy(object);
    }

    if (tagSessionProxy_ == nullptr) {
        HILOGE("NfcTagSessionProxy init failed!");
    }
}
NfcTagImpl::~NfcTagImpl()
{
}
NfcTagImpl& NfcTagImpl::GetInstance()
{
    static NfcTagImpl tagImplSingleton;
    return tagImplSingleton;
}
ErrCode NfcTagImpl::Init()
{
    return tagSessionProxy_->Init();
}
ErrCode NfcTagImpl::Uninit()
{
    return tagSessionProxy_->Uninit();
}
ErrCode NfcTagImpl::ReadNdefTag(std::string &response)
{
    return tagSessionProxy_->ReadNdefTag(response);
}
ErrCode NfcTagImpl::WriteNdefTag(std::string data)
{
    return tagSessionProxy_->WriteNdefTag(data);
}
ErrCode NfcTagImpl::RegListener(const sptr<INfcTagCallback> &callback)
{
    return tagSessionProxy_->RegListener(callback);
}
ErrCode NfcTagImpl::UnregListener(const sptr<INfcTagCallback> &callback)
{
    return tagSessionProxy_->UnregListener(callback);
}
}  // namespace NFC
}  // namespace OHOS