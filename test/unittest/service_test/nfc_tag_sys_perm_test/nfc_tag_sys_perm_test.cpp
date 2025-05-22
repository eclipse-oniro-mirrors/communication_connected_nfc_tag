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

#include "gtest/gtest.h"
#include "accesstoken_kit.h"
#include "ipc_skeleton.h"
#include "tokenid_kit.h"
#include "nfc_tag_sys_perm.h"
#include "common_mock.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::AccessToken;

namespace OHOS {
namespace NFC {

class NfcTagSysPermTest : public ::testing::Test {
protected:
void SetUp() override
{
    InitCommonMock();
}
void TearDown() override {}
};

/**
 * @tc.name  : NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_001
 * @tc.number: NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_001
 * @tc.desc  : return NFC_SUCCESS
 */
HWTEST_F(NfcTagSysPermTest, NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_001 start";
    IPCSkeleton::callingPid_ = 1001;
    IPCSkeleton::callingUid_ = 1001;
    IPCSkeleton::callingFullTokenId_ = 123456789;
    TokenIdKit::isSystemApp_ = true;
    IPCSkeleton::callingTokenId_ = 123456789;
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_NATIVE;

    ErrCode result = NfcTagServiceVerifyPermissionsHook();
    EXPECT_EQ(result, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_001 end";
}

/**
 * @tc.name  : NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_002
 * @tc.number: NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_002
 * @tc.desc  : the function should return NFC_SUCCESS
 */
HWTEST_F(NfcTagSysPermTest, NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_002 start";
    IPCSkeleton::callingPid_ = 1002;
    IPCSkeleton::callingUid_ = 1002;
    IPCSkeleton::callingFullTokenId_ = 987654321;
    TokenIdKit::isSystemApp_ = false;
    IPCSkeleton::callingTokenId_ = 987654321;
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_NATIVE;

    ErrCode result = NfcTagServiceVerifyPermissionsHook();
    EXPECT_EQ(result, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_002 end";
}

/**
 * @tc.name  : NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_003
 * @tc.number: NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_003
 * @tc.desc  : the function should return NFC_SYS_PERM_FAILED
 */
HWTEST_F(NfcTagSysPermTest, NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_003 start";
    IPCSkeleton::callingPid_ = 1003;
    IPCSkeleton::callingUid_ = 1003;
    IPCSkeleton::callingFullTokenId_ = 123456789;
    TokenIdKit::isSystemApp_ = false;
    IPCSkeleton::callingTokenId_ = 123456789;
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_HAP;

    ErrCode result = NfcTagServiceVerifyPermissionsHook();
    EXPECT_EQ(result, NFC_SYS_PERM_FAILED);
    GTEST_LOG_(INFO) << "NfcTagSysPermTest_NfcTagServiceVerifyPermissionsHook_003 end";
}

} // namespace NFC
} // namespace OHOS