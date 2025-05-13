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

#define LOG_TAG "NFCTAG_UTILS"
#include "nfc_tag_utils.h"
#include "nfc_tag_log.h"
#include "accesstoken_kit.h"
#include "ipc_skeleton.h"

using namespace OHOS::Security::AccessToken;

namespace OHOS {
namespace NFC {
bool Utils::IsGranted(const std::string &permission)
{
    AccessTokenID callerToken = IPCSkeleton::GetCallingTokenID();
    ATokenTypeEnum tokenType = AccessTokenKit::GetTokenTypeFlag(callerToken);
    int result = PermissionState::PERMISSION_DENIED;
    if (tokenType == ATokenTypeEnum::TOKEN_NATIVE || tokenType == ATokenTypeEnum::TOKEN_HAP) {
        result = AccessTokenKit::VerifyAccessToken(callerToken, permission);
    } else {
        result = PermissionState::PERMISSION_DENIED;
    }
    return result == PermissionState::PERMISSION_GRANTED;
}
std::string Utils::GetCallingName()
{
    uint32_t tokenId = IPCSkeleton::GetCallingTokenID();
    return GetCallingName(tokenId);
}

std::string Utils::GetCallingName(const uint32_t &tokenId)
{
    ATokenTypeEnum callingType = AccessTokenKit::GetTokenTypeFlag(tokenId);
    switch (callingType) {
        case ATokenTypeEnum::TOKEN_HAP : {
            HapTokenInfo hapTokenInfo;
            if (AccessTokenKit::GetHapTokenInfo(tokenId, hapTokenInfo) == AccessTokenKitRet::RET_SUCCESS) {
                return hapTokenInfo.bundleName;
            }
            HILOGE("callingType(%{public}d), GetHapTokenInfo failed.", callingType);
            return "";
        }
        case ATokenTypeEnum::TOKEN_SHELL:
        case ATokenTypeEnum::TOKEN_NATIVE: {
            NativeTokenInfo nativeTokenInfo;
            if (AccessTokenKit::GetNativeTokenInfo(tokenId, nativeTokenInfo) == AccessTokenKitRet::RET_SUCCESS) {
                return nativeTokenInfo.processName;
            }
            HILOGE("callingType(%{public}d), GetNativeTokenInfo failed.", callingType);
            return "";
        }
        default:
            HILOGE("callingType(%{public}d) is invalid.", callingType);
            return "";
    }
}

}  // namespace NFC
}  // namespace OHOS