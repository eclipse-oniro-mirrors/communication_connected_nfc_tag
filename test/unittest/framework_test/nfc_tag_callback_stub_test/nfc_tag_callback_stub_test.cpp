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

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#define private public
#include "nfc_tag_callback_stub.h"
#undef private
#include "common_mock.h"
using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace NFC {
class NfcTagCallBackStubTest : public ::testing::Test {
protected:
void SetUp() override
{
    InitCommonMock();
}
void TearDown() override {}
};

class INfcTagCallbackTest : public INfcTagCallback {
public:
    ErrCode OnNotify(int nfcRfState)
    {
        return NFC_SUCCESS;
    }
    sptr<IRemoteObject> AsObject() override
    {
        return nullptr;
    }
    ~INfcTagCallbackTest() override = default;
};

class NfcTagCallbackStubMock : public NfcTagCallbackStub {
public:
    bool AddDeathRecipient(const sptr<DeathRecipient> &recipient) override
    {
        return true;
    }
    bool RemoveDeathRecipient(const sptr<DeathRecipient> &recipient) override
    {
        return true;
    }
};

/*
 * @tc.number: NfcTagCallBackStubTest_IsUserCallbackEmpty_001
 * @tc.name: NfcTagCallBackStubTest_IsUserCallbackEmpty_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_IsUserCallbackEmpty_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_IsUserCallbackEmpty_001 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    EXPECT_EQ(true, nfcTagCallbackStubMock.IsUserCallbackEmpty());
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_IsUserCallbackEmpty_001 end";
};

/*
 * @tc.number: NfcTagCallBackStubTest_OnNotify_001
 * @tc.name: NfcTagCallBackStubTest_OnNotify_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_OnNotify_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_OnNotify_001 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    nfcTagCallbackStubMock.callbackStore_.emplace_back(callbackTest);
    EXPECT_EQ(NFC_SUCCESS, nfcTagCallbackStubMock.OnNotify(1));
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_OnNotify_001 end";
};

/*
 * @tc.number: NfcTagCallBackStubTest_OnRemoteRequest_001
 * @tc.name: NfcTagCallBackStubTest_OnRemoteRequest_001
 * @tc.desc: return NFC_INVALID_TOKEN
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_OnRemoteRequest_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_OnRemoteRequest_001 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    MessageParcel::interfaceTokenMock_ = u"";
    int ret = nfcTagCallbackStubMock.OnRemoteRequest(
        static_cast<uint32_t>(INfcTagCallback::CMD_ON_NFC_TAG_NOTIFY),
        data, reply, option);
    EXPECT_EQ(ret, static_cast<int>(NFC_INVALID_TOKEN));
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_OnRemoteRequest_001 end";
}

/*
 * @tc.number: NfcTagCallBackStubTest_OnRemoteRequest_002
 * @tc.name: NfcTagCallBackStubTest_OnRemoteRequest_002
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_OnRemoteRequest_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_OnRemoteRequest_002 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    MessageParcel::interfaceTokenMock_ = u"ohos.nfc.INfcTagCallback";
    int ret = nfcTagCallbackStubMock.OnRemoteRequest(
        static_cast<uint32_t>(INfcTagCallback::CMD_ON_NFC_TAG_NOTIFY),
        data, reply, option);
    EXPECT_EQ(ret, static_cast<int>(NFC_SUCCESS));
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_OnRemoteRequest_002 end";
}

/*
 * @tc.number: NfcTagCallBackStubTest_OnRemoteRequest_003
 * @tc.name: NfcTagCallBackStubTest_OnRemoteRequest_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_OnRemoteRequest_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_OnRemoteRequest_003 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    MessageParcel::interfaceTokenMock_ = u"ohos.nfc.INfcTagCallback";
    IPCObjectStub::onRemoteRequestRetMock_ = NFC_SUCCESS;
    int ret = nfcTagCallbackStubMock.OnRemoteRequest(1000,
        data, reply, option);
    EXPECT_EQ(ret, static_cast<int>(NFC_SUCCESS));
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_OnRemoteRequest_003 end";
}

/*
 * @tc.number: NfcTagCallBackStubTest_RegisterUserCallBack_001
 * @tc.name: NfcTagCallBackStubTest_RegisterUserCallBack_001
 * @tc.desc: return NFC_NO_CALLBACK
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_RegisterUserCallBack_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_RegisterUserCallBack_001 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    EXPECT_EQ(nfcTagCallbackStubMock.RegisterUserCallBack(nullptr), NFC_NO_CALLBACK);
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_RegisterUserCallBack_001 end";
}

/*
 * @tc.number: NfcTagCallBackStubTest_RegisterUserCallBack_002
 * @tc.name: NfcTagCallBackStubTest_RegisterUserCallBack_002
 * @tc.desc: return NFC_CALLBACK_REGISTERED
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_RegisterUserCallBack_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_RegisterUserCallBack_002 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    nfcTagCallbackStubMock.callbackStore_.emplace_back(callbackTest);
    EXPECT_EQ(nfcTagCallbackStubMock.RegisterUserCallBack(callbackTest), NFC_CALLBACK_REGISTERED);
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_RegisterUserCallBack_002 end";
}

/*
 * @tc.number: NfcTagCallBackStubTest_RegisterUserCallBack_003
 * @tc.name: NfcTagCallBackStubTest_RegisterUserCallBack_003
 * @tc.desc: return NFC_TOO_MANY_CALLBACK
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_RegisterUserCallBack_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_RegisterUserCallBack_003 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    ErrCode ret = NFC_SUCCESS;
    for (int i = 0; i <= 30; i++) {
        sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
        ret = nfcTagCallbackStubMock.RegisterUserCallBack(callbackTest);
        if (i == 30) {
            EXPECT_EQ(ret, NFC_TOO_MANY_CALLBACK);
        } else {
            EXPECT_EQ(ret, NFC_SUCCESS);
        }
    }
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_RegisterUserCallBack_003 end";
}

/*
 * @tc.number: NfcTagCallBackStubTest_UnRegisterUserCallBack_001
 * @tc.name: NfcTagCallBackStubTest_UnRegisterUserCallBack_001
 * @tc.desc: return NFC_NO_CALLBACK
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_UnRegisterUserCallBack_001, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_UnRegisterUserCallBack_001 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    EXPECT_EQ(nfcTagCallbackStubMock.UnRegisterUserCallBack(nullptr), NFC_NO_CALLBACK);
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_UnRegisterUserCallBack_001 end";
}

/*
 * @tc.number: NfcTagCallBackStubTest_UnRegisterUserCallBack_002
 * @tc.name: NfcTagCallBackStubTest_UnRegisterUserCallBack_002
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_UnRegisterUserCallBack_002, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_UnRegisterUserCallBack_002 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    nfcTagCallbackStubMock.callbackStore_.emplace_back(callbackTest);
    EXPECT_EQ(nfcTagCallbackStubMock.UnRegisterUserCallBack(callbackTest), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_UnRegisterUserCallBack_002 end";
}

/*
 * @tc.number: NfcTagCallBackStubTest_UnRegisterUserCallBack_003
 * @tc.name: NfcTagCallBackStubTest_UnRegisterUserCallBack_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagCallBackStubTest, NfcTagCallBackStubTest_UnRegisterUserCallBack_003, TestSize.Level1)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_UnRegisterUserCallBack_003 start";
    NfcTagCallbackStubMock nfcTagCallbackStubMock;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    EXPECT_EQ(nfcTagCallbackStubMock.UnRegisterUserCallBack(callbackTest), NFC_CALLBACK_NOT_REGISTERED);
    GTEST_LOG_(INFO) << "NfcTagCallBackStubTest_UnRegisterUserCallBack_003 end";
}

} // namespace NFC
} // namespace OHOS