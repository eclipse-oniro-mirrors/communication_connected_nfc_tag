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

#define LOG_TAG "NFCTAG_PERM"
#include "nfc_tag_sys_perm.h"
#include "nfc_tag_log.h"
#include "accesstoken_kit.h"
#include "ipc_skeleton.h"
#include "tokenid_kit.h"
#include "nfc_tag_utils.h"

using namespace OHOS::Security::AccessToken;

namespace OHOS {
namespace NFC {
ErrCode NfcTagServiceVerifyPermissionsHook(void)
{
    uint64_t fullTokenId = IPCSkeleton::GetCallingFullTokenID();
    bool isSystemApp = TokenIdKit::IsSystemAppByFullTokenID(fullTokenId);
    AccessTokenID callerToken = IPCSkeleton::GetCallingTokenID();
    ATokenTypeEnum tokenType = AccessTokenKit::GetTokenTypeFlag(callerToken);
    HILOGI("isSystemApp: %{public}d, tokenType: %{public}d",
        static_cast<int>(isSystemApp), static_cast<int>(tokenType));
    // only allow system app or system sa call
    if (isSystemApp || (tokenType == ATokenTypeEnum::TOKEN_NATIVE)) {
        return NFC_SUCCESS;
    }
    return NFC_SYS_PERM_FAILED;
}
}  // namespace NFC
}  // namespace OHOS
