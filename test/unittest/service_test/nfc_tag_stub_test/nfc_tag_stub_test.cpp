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
#define private public
#include "nfc_tag_stub.h"
#undef private
#include "nfc_tag_callback_stub.h"
#include "iremote_broker.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace NFC {

class NfcTagStubTest : public ::testing::Test {
protected:
void SetUp() override {}
void TearDown() override {}
};

ErrCode g_serviceRetMock = NFC_ERR_MAX;

class NfcTagStubMock: public NfcTagStub
{
public:
    NfcTagStubMock() {}
    ~NfcTagStubMock() {}

    ErrCode Init()
    {
        return g_serviceRetMock;
    }

    ErrCode Uninit()
    {
        return g_serviceRetMock;
    }

    ErrCode ReadNdefTag(std::string &response)
    {
        return g_serviceRetMock;
    }

    ErrCode WriteNdefTag(const std::string &data)
    {
        return g_serviceRetMock;
    }

    ErrCode ReadNdefData(std::vector<uint8_t> &data)
    {
        return g_serviceRetMock;
    }

    ErrCode WriteNdefData(const std::vector<uint8_t> &data)
    {
        return g_serviceRetMock;
    }

    ErrCode RegListener(const sptr<INfcTagCallback> &callback)
    {
        return g_serviceRetMock;
    }

    ErrCode UnregListener(const sptr<INfcTagCallback> &callback)
    {
        return g_serviceRetMock;
    }
};

/*
 * @tc.number: NfcTagStubTest_InitHandleMap_001
 * @tc.name: NfcTagStubTest_InitHandleMap_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_InitHandleMap_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_InitHandleMap_001 start";
    NfcTagStubMock stub;
    EXPECT_EQ(true, stub.InitHandleMap());
    GTEST_LOG_(INFO) << "NfcTagStubTest_InitHandleMap_001 end";
}

/*
 * @tc.number: NfcTagStubTest_OnRemoteRequest_001
 * @tc.name: NfcTagStubTest_OnRemoteRequest_001
 * @tc.desc: return NFC_INVALID_TOKEN
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnRemoteRequest_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRemoteRequest_001 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    MessageParcel::interfaceTokenMock_ = u"";
    int result = stub.OnRemoteRequest(INfcTagService::NFC_TAG_CMD_INIT, data, reply, option);
    EXPECT_EQ(result, static_cast<int>(NFC_INVALID_TOKEN));
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRemoteRequest_001 end";
}

/*
 * @tc.number: NfcTagStubTest_OnRemoteRequest_002
 * @tc.name: NfcTagStubTest_OnRemoteRequest_002
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnRemoteRequest_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRemoteRequest_002 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    MessageParcel::interfaceTokenMock_ = u"ohos.nfc.IConnectedNfcTagService";
    g_serviceRetMock = NFC_SUCCESS;
    int result = stub.OnRemoteRequest(INfcTagService::NFC_TAG_CMD_INIT, data, reply, option);
    EXPECT_EQ(result, static_cast<int>(NFC_SUCCESS));
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRemoteRequest_002 end";
}

/*
 * @tc.number: NfcTagStubTest_OnRemoteRequest_003
 * @tc.name: NfcTagStubTest_OnRemoteRequest_003
 * @tc.desc: return -1
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnRemoteRequest_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRemoteRequest_003 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    MessageParcel::interfaceTokenMock_ = u"ohos.nfc.IConnectedNfcTagService";
    g_serviceRetMock = NFC_SUCCESS;
    IPCObjectStub::sendRequestRetMock_ = -1;
    int result = stub.OnRemoteRequest(-1, data, reply, option);
    EXPECT_EQ(result, -1);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRemoteRequest_003 end";
}

/*
 * @tc.number: NfcTagStubTest_OnInit_001
 * @tc.name: NfcTagStubTest_OnInit_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnInit_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnInit_001 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    g_serviceRetMock = NFC_SUCCESS;
    ErrCode result = stub.OnInit(0, data, reply);
    EXPECT_EQ(result, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnInit_001 end";
}

/*
 * @tc.number: NfcTagStubTest_OnUninit_001
 * @tc.name: NfcTagStubTest_OnUninit_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnUninit_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnUninit_001 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    g_serviceRetMock = NFC_SUCCESS;
    ErrCode result = stub.OnUninit(0, data, reply);
    EXPECT_EQ(result, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnUninit_001 end";
}

/*
 * @tc.number: NfcTagStubTest_OnReadNdefTag_001
 * @tc.name: NfcTagStubTest_OnReadNdefTag_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnReadNdefTag_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnReadNdefTag_001 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    g_serviceRetMock = NFC_SUCCESS;
    ErrCode result = stub.OnReadNdefTag(0, data, reply);
    EXPECT_EQ(result, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnReadNdefTag_001 end";
}

/*
 * @tc.number: NfcTagStubTest_OnWriteNdefTag_001
 * @tc.name: NfcTagStubTest_OnWriteNdefTag_001
 * @tc.desc: return NFC_IPC_READ_FAILED
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnWriteNdefTag_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefTag_001 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageParcel::readRetMock_ = false;
    g_serviceRetMock = NFC_SUCCESS;
    ErrCode result = stub.OnWriteNdefTag(0, data, reply);
    EXPECT_EQ(result, NFC_IPC_READ_FAILED);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefTag_001 end";
}

/*
 * @tc.number: NfcTagStubTest_OnWriteNdefTag_002
 * @tc.name: NfcTagStubTest_OnWriteNdefTag_002
 * @tc.desc: return NFC_INVALID_PARAMETER
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnWriteNdefTag_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefTag_002 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageParcel::readStringMock_ = "";
    MessageParcel::readRetMock_ = true;
    g_serviceRetMock = NFC_SUCCESS;
    ErrCode result = stub.OnWriteNdefTag(0, data, reply);
    EXPECT_EQ(result, NFC_INVALID_PARAMETER);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefTag_002 end";
}

/*
 * @tc.number: NfcTagStubTest_OnWriteNdefTag_003
 * @tc.name: NfcTagStubTest_OnWriteNdefTag_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnWriteNdefTag_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefTag_003 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageParcel::readStringMock_ = "test";
    MessageParcel::readRetMock_ = true;
    g_serviceRetMock = NFC_SUCCESS;
    ErrCode result = stub.OnWriteNdefTag(0, data, reply);
    EXPECT_EQ(result, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefTag_003 end";
}

/*
 * @tc.number: NfcTagStubTest_OnReadNdefData_001
 * @tc.name: NfcTagStubTest_OnReadNdefData_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnReadNdefData_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnReadNdefData_001 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    g_serviceRetMock = NFC_SUCCESS;
    ErrCode result = stub.OnReadNdefData(0, data, reply);
    EXPECT_EQ(result, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnReadNdefData_001 end";
}

/*
 * @tc.number: NfcTagStubTest_OnWriteNdefData_001
 * @tc.name: NfcTagStubTest_OnWriteNdefData_001
 * @tc.desc: return NFC_IPC_READ_FAILED
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnWriteNdefData_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefData_001 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageParcel::readVectorUInt8RetMock_ = false;
    g_serviceRetMock = NFC_SUCCESS;
    ErrCode result = stub.OnWriteNdefData(0, data, reply);
    EXPECT_EQ(result, NFC_IPC_READ_FAILED);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefData_001 end";
}

/*
 * @tc.number: NfcTagStubTest_OnWriteNdefData_002
 * @tc.name: NfcTagStubTest_OnWriteNdefData_002
 * @tc.desc: return NFC_INVALID_PARAMETER
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnWriteNdefData_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefData_002 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageParcel::readVectorUInt8RetMock_ = true;
    MessageParcel::readVectorUInt8Mock_ = {};
    g_serviceRetMock = NFC_SUCCESS;
    ErrCode result = stub.OnWriteNdefData(0, data, reply);
    EXPECT_EQ(result, NFC_INVALID_PARAMETER);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefData_002 end";
}

/*
 * @tc.number: NfcTagStubTest_OnWriteNdefData_003
 * @tc.name: NfcTagStubTest_OnWriteNdefData_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnWriteNdefData_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefData_003 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageParcel::readVectorUInt8RetMock_ = true;
    MessageParcel::readVectorUInt8Mock_ = {3, 1, 0};
    g_serviceRetMock = NFC_SUCCESS;
    ErrCode result = stub.OnWriteNdefData(0, data, reply);
    EXPECT_EQ(result, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnWriteNdefData_003 end";
}

/*
 * @tc.number: NfcTagStubTest_OnRegisterCallback_001
 * @tc.name: NfcTagStubTest_OnRegisterCallback_001
 * @tc.desc: return NFC_NO_OBJECT
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnRegisterCallback_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRegisterCallback_001 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageParcel::readRemoteMock_ = nullptr;
    ErrCode result = stub.OnRegisterCallback(0, data, reply);
    EXPECT_EQ(result, NFC_NO_OBJECT);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRegisterCallback_001 end";
}

/*
 * @tc.number: NfcTagStubTest_OnRegisterCallback_002
 * @tc.name: NfcTagStubTest_OnRegisterCallback_002
 * @tc.desc: return NFC_NO_CALLBACK
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnRegisterCallback_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRegisterCallback_002 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageParcel::readRemoteMock_ = new NfcTagCallbackStub();
    IRemoteBrokerMock::g_brokerMock = nullptr;
    ErrCode result = stub.OnRegisterCallback(0, data, reply);
    EXPECT_EQ(result, NFC_NO_CALLBACK);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRegisterCallback_002 end";
}

/*
 * @tc.number: NfcTagStubTest_OnRegisterCallback_003
 * @tc.name: NfcTagStubTest_OnRegisterCallback_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnRegisterCallback_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRegisterCallback_003 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    g_serviceRetMock = NFC_SUCCESS;
    MessageParcel::readRemoteMock_ = new NfcTagCallbackStub();
    IRemoteBrokerMock::g_brokerMock = new NfcTagCallbackStub();
    ErrCode result = stub.OnRegisterCallback(0, data, reply);
    EXPECT_EQ(result, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnRegisterCallback_003 end";
}

/*
 * @tc.number: NfcTagStubTest_OnUnRegisterCallback_001
 * @tc.name: NfcTagStubTest_OnUnRegisterCallback_001
 * @tc.desc: return NFC_NO_OBJECT
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnUnRegisterCallback_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnUnRegisterCallback_001 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    MessageParcel::readRemoteMock_ = nullptr;
    ErrCode result = stub.OnUnRegisterCallback(0, data, reply);
    EXPECT_EQ(result, NFC_NO_OBJECT);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnUnRegisterCallback_001 end";
}

/*
 * @tc.number: NfcTagStubTest_OnUnRegisterCallback_002
 * @tc.name: NfcTagStubTest_OnUnRegisterCallback_002
 * @tc.desc: return NFC_NO_CALLBACK
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnUnRegisterCallback_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnUnRegisterCallback_002 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    IRemoteBrokerMock::g_brokerMock = nullptr;
    MessageParcel::readRemoteMock_ = new NfcTagCallbackStub();
    ErrCode result = stub.OnUnRegisterCallback(0, data, reply);
    EXPECT_EQ(result, NFC_NO_CALLBACK);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnUnRegisterCallback_002 end";
}

/*
 * @tc.number: NfcTagStubTest_OnUnRegisterCallback_003
 * @tc.name: NfcTagStubTest_OnUnRegisterCallback_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagStubTest, NfcTagStubTest_OnUnRegisterCallback_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnUnRegisterCallback_003 start";
    NfcTagStubMock stub;
    MessageParcel data;
    MessageParcel reply;
    g_serviceRetMock = NFC_SUCCESS;
    MessageParcel::readRemoteMock_ = new NfcTagCallbackStub();
    IRemoteBrokerMock::g_brokerMock = new NfcTagCallbackStub();
    ErrCode result = stub.OnUnRegisterCallback(0, data, reply);
    EXPECT_EQ(result, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagStubTest_OnUnRegisterCallback_003 end";
}

} // namespace NFC
} // namespace OHOS