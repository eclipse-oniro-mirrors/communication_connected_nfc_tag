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
#define LOG_TAG "NFCTAG_SA"
#include "system_ability_listener.h"
#include "nfc_tag_log.h"
#include "nfc_tag_errcode.h"
#include "iservice_registry.h"
#include "if_system_ability_manager.h"

namespace OHOS {
namespace NFC {
SystemAbilityListener::SystemAbilityListener(const std::string &name, int32_t systemAbilityId,
    ListenerFunc addFunc, ListenerFunc removeFunc)
    : name_(name),
      systemAbilityId_(systemAbilityId),
      addFunc_(addFunc),
      removeFunc_(removeFunc)
{
    HILOGI("enter.");
}

void SystemAbilityListener::OnAddSystemAbility(int32_t systemAbilityId, const std::string &deviceId)
{
    HILOGI("enter");
    if (systemAbilityId != systemAbilityId_) {
        HILOGI("systemAbilityId is not same.");
        return;
    }

    if (addFunc_ != nullptr) {
        HILOGI("addFunc_ called");
        addFunc_();
    }
}

void SystemAbilityListener::OnRemoveSystemAbility(int32_t systemAbilityId, const std::string &deviceId)
{
    HILOGI("enter");
    if (systemAbilityId != systemAbilityId_) {
        HILOGI("systemAbilityId is not same.");
        return;
    }

    if (removeFunc_ != nullptr) {
        HILOGI("removeFunc_ called");
        removeFunc_();
    }
}

sptr<SystemAbilityListener> SystemAbilityListener::Subscribe(const std::string &name, int32_t systemAbilityId,
    ListenerFunc addFunc, ListenerFunc removeFunc)
{
    HILOGI("name:%{public}s, systemAbilityId:%{public}d", name.c_str(), systemAbilityId);
    auto sam = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (sam == nullptr) {
        return nullptr;
    }
    auto listener = sptr<SystemAbilityListener>::MakeSptr(name, systemAbilityId, addFunc, removeFunc);
    int32_t ret = sam->SubscribeSystemAbility(systemAbilityId, listener);
    if (ret != ERR_OK) {
        HILOGE("SubscribeSystemAbility fail, name:%{public}s, "
            "systemAbilityId:%{public}d",
            name.c_str(), systemAbilityId);
        return nullptr;
    }

    HILOGI("Subscribe service name:%{public}s success", name.c_str());
    return listener;
}

void SystemAbilityListener::UnSubscribe(int32_t systemAbilityId, sptr<SystemAbilityListener> &listener)
{
    HILOGI("start systemAbilityId:%{public}d", systemAbilityId);
    auto sam = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (sam == nullptr) {
        return;
    }

    int32_t ret = sam->UnSubscribeSystemAbility(systemAbilityId, listener);
    if (ret != ERR_OK) {
        HILOGE("UnSubscribeSystemAbility fail.");
        return;
    }

    HILOGI("UnSubscribe service success");
}
}  // namespace NFC
}  // namespace OHOS
