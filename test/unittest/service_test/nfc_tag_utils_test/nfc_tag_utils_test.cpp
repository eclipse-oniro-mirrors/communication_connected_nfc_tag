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
#include "nfc_tag_utils.h"
#include "common_mock.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::AccessToken;

namespace OHOS {
namespace NFC {

class NfcTagUtilsTest : public ::testing::Test {
protected:
void SetUp() override
{
    InitCommonMock();
}
void TearDown() override {}
};


/**
 * @tc.name  : NfcTagUtilsTest_IsGranted_001
 * @tc.number: NfcTagUtilsTest_IsGranted_001
 * @tc.desc  : Test when permission is granted, the function should return true
 */
HWTEST_F(NfcTagUtilsTest, NfcTagUtilsTest_IsGranted_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_IsGranted_001 start";
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_NATIVE;
    AccessTokenKit::accessTokenKitRet_ = PermissionState::PERMISSION_GRANTED;
    IPCSkeleton::callingTokenId_ = 1000;

    EXPECT_EQ(true, Utils::IsGranted(TAG_PERMISSION));
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_IsGranted_001 end";
}

/**
 * @tc.name  : NfcTagUtilsTest_IsGranted_002
 * @tc.number: NfcTagUtilsTest_IsGranted_002
 * @tc.desc  : Test when permission is not granted, the function should return false
 */
HWTEST_F(NfcTagUtilsTest, NfcTagUtilsTest_IsGranted_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_IsGranted_002 start";
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_NATIVE;
    AccessTokenKit::accessTokenKitRet_ = PermissionState::PERMISSION_DENIED;
    IPCSkeleton::callingTokenId_ = 1000;

    EXPECT_EQ(false, Utils::IsGranted(TAG_PERMISSION));
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_IsGranted_002 end";
}

/**
 * @tc.name  : NfcTagUtilsTest_IsGranted_003
 * @tc.number: NfcTagUtilsTest_IsGranted_003
 * @tc.desc  : Test when token type is not native or HAP, the function should return false
 */
HWTEST_F(NfcTagUtilsTest, NfcTagUtilsTest_IsGranted_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_IsGranted_003 start";
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_SHELL;
    AccessTokenKit::accessTokenKitRet_ = PermissionState::PERMISSION_DENIED;
    IPCSkeleton::callingTokenId_ = 1000;

    EXPECT_EQ(false, Utils::IsGranted(TAG_PERMISSION));
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_IsGranted_003 end";
}


/**
 * @tc.name  : NfcTagUtilsTest_IsGranted_004
 * @tc.number: NfcTagUtilsTest_IsGranted_004
 * @tc.desc  : return true
 */
HWTEST_F(NfcTagUtilsTest, NfcTagUtilsTest_IsGranted_004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_IsGranted_004 start";
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_HAP;
    AccessTokenKit::accessTokenKitRet_ = PermissionState::PERMISSION_GRANTED;
    IPCSkeleton::callingTokenId_ = 1000;

    EXPECT_EQ(true, Utils::IsGranted(TAG_PERMISSION));
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_IsGranted_004 end";
}

/**
 * @tc.name  : NfcTagUtilsTest_GetCallingName_001
 * @tc.number: NfcTagUtilsTest_GetCallingName_001
 * @tc.desc  : return ""
 */
HWTEST_F(NfcTagUtilsTest, NfcTagUtilsTest_GetCallingName_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_GetCallingName_001 start";
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_HAP;
    IPCSkeleton::callingTokenId_ = 1000;
    AccessTokenKit::hapTokenRet_ = -1;
    AccessTokenKit::hapTokenInfo_.bundleName = "123";
    EXPECT_EQ("", Utils::GetCallingName());
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_GetCallingName_001 end";
}

/**
 * @tc.name  : NfcTagUtilsTest_GetCallingName_002
 * @tc.number: NfcTagUtilsTest_GetCallingName_002
 * @tc.desc  : return "123"
 */
HWTEST_F(NfcTagUtilsTest, NfcTagUtilsTest_GetCallingName_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_GetCallingName_002 start";
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_HAP;
    IPCSkeleton::callingTokenId_ = 1000;
    AccessTokenKit::hapTokenRet_ = 0;
    AccessTokenKit::hapTokenInfo_.bundleName = "123";
    EXPECT_EQ("123", Utils::GetCallingName());
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_GetCallingName_002 end";
}

/**
 * @tc.name  : NfcTagUtilsTest_GetCallingName_003
 * @tc.number: NfcTagUtilsTest_GetCallingName_003
 * @tc.desc  : return ""
 */
HWTEST_F(NfcTagUtilsTest, NfcTagUtilsTest_GetCallingName_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_GetCallingName_003 start";
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_SHELL;
    IPCSkeleton::callingTokenId_ = 1000;
    AccessTokenKit::nativeTokenRet_ = -1;
    AccessTokenKit::nativeTokenInfo_.processName = "123";
    EXPECT_EQ("", Utils::GetCallingName());
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_GetCallingName_003 end";
}

/**
 * @tc.name  : NfcTagUtilsTest_GetCallingName_004
 * @tc.number: NfcTagUtilsTest_GetCallingName_004
 * @tc.desc  : return "123"
 */
HWTEST_F(NfcTagUtilsTest, NfcTagUtilsTest_GetCallingName_004, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_GetCallingName_004 start";
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_SHELL;
    IPCSkeleton::callingTokenId_ = 1000;
    AccessTokenKit::nativeTokenRet_ = 0;
    AccessTokenKit::nativeTokenInfo_.processName = "123";
    EXPECT_EQ("123", Utils::GetCallingName());
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_GetCallingName_004 end";
}

/**
 * @tc.name  : NfcTagUtilsTest_GetCallingName_005
 * @tc.number: NfcTagUtilsTest_GetCallingName_005
 * @tc.desc  : return ""
 */
HWTEST_F(NfcTagUtilsTest, NfcTagUtilsTest_GetCallingName_005, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_GetCallingName_005 start";
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_INVALID;
    IPCSkeleton::callingTokenId_ = 1000;
    AccessTokenKit::hapTokenRet_ = -1;
    AccessTokenKit::nativeTokenRet_ = -1;
    AccessTokenKit::nativeTokenInfo_.processName = "123";
    EXPECT_EQ("", Utils::GetCallingName());
    GTEST_LOG_(INFO) << "NfcTagUtilsTest_GetCallingName_005 end";
}

} // namespace NFC
} // namespace OHOS