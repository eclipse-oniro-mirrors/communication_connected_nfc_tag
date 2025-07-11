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

#ifndef SYSTEM_ABILITY_LISTENER_H
#define SYSTEM_ABILITY_LISTENER_H

#include "system_ability_status_change_stub.h"

namespace OHOS {
namespace NFC {
class SystemAbilityListener : public OHOS::SystemAbilityStatusChangeStub, public NoCopyable {
public:
    using ListenerFunc = std::function<void(void)>;
    SystemAbilityListener(const std::string &name, int32_t systemAbilityId,
        ListenerFunc addFunc, ListenerFunc removeFunc);
    ~SystemAbilityListener() override {};

    void OnAddSystemAbility(int32_t systemAbilityId, const std::string &deviceId) override;
    void OnRemoveSystemAbility(int32_t systemAbilityId, const std::string &deviceId) override;
    static sptr<SystemAbilityListener> Subscribe(const std::string &name, int32_t systemAbilityId,
        ListenerFunc addFunc, ListenerFunc removeFunc);
    static void UnSubscribe(int32_t systemAbilityId, sptr<SystemAbilityListener> &listener);

private:
    std::string name_;
    int32_t systemAbilityId_ = -1;
    ListenerFunc addFunc_;
    ListenerFunc removeFunc_;
};
}  // namespace NFC
}  // namespace OHOS
#endif
