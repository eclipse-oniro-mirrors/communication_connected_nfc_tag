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

#ifndef OHOS_NFC_TAG_UTILS
#define OHOS_NFC_TAG_UTILS
#include <string>
namespace OHOS {
namespace NFC {
static const std::string TAG_PERMISSION = "ohos.permission.NFC_TAG";
class Utils {
public:
    /**
     * @Description : Check the Permission.
     *
     * @param permission - the permission to be checked
     * @return true: granted; false: not granted
     */
    static bool IsGranted(const std::string &permission);

    static std::string GetCallingName();
    static std::string GetCallingName(const uint32_t &tokenId);
};
}  // namespace NFC
}  // namespace OHOS
#endif // OHOS_NFC_TAG_UTILS