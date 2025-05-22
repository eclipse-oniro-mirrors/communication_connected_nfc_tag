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
#include "nfc_tag_proxy.h"
#undef private
#include "nfc_tag_stub.h"
#include "system_ability.h"
#include "accesstoken_kit.h"
#include "ipc_skeleton.h"
#include "tokenid_kit.h"
#include "common_mock.h"
#include "nfc_tag_callback_stub.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace NFC {

static int g_onRemoteRetMock = -1;
static ErrCode g_onRetMock = NFC_ERR_MAX;

class NfcTagProxyTest : public ::testing::Test {
protected:
void SetUp() override
{
    InitCommonMock();
    g_onRemoteRetMock = -1;
    g_onRetMock = NFC_ERR_MAX;
}
void TearDown() override {}
};

NfcTagStub::NfcTagStub()
{}

NfcTagStub::~NfcTagStub()
{}

bool NfcTagStub::InitHandleMap()
{
    return true;
}

int NfcTagStub::OnRemoteRequest(uint32_t code, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    return g_onRemoteRetMock;
}

ErrCode NfcTagStub::OnInit(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return g_onRetMock;
}
ErrCode NfcTagStub::OnUninit(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return g_onRetMock;
}
ErrCode NfcTagStub::OnReadNdefTag(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return g_onRetMock;
}
ErrCode NfcTagStub::OnWriteNdefTag(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return g_onRetMock;
}
ErrCode NfcTagStub::OnReadNdefData(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return g_onRetMock;
}
ErrCode NfcTagStub::OnWriteNdefData(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return g_onRetMock;
}
ErrCode NfcTagStub::OnRegisterCallback(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return g_onRetMock;
}
ErrCode NfcTagStub::OnUnRegisterCallback(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return g_onRetMock;
}

class NfcTagService : public SystemAbility, public NfcTagStub {
public:
    NfcTagService() {}
    ~NfcTagService() {}

    void OnStart() override {}
    void OnStop() override {}

    ErrCode Init() override
    {
        return NFC_SUCCESS;
    }

    ErrCode Uninit() override
    {
        return NFC_SUCCESS;
    }

    ErrCode ReadNdefTag(std::string &response) override
    {
        return NFC_SUCCESS;
    }

    ErrCode WriteNdefTag(const std::string &data) override
    {
        return NFC_SUCCESS;
    }

    ErrCode ReadNdefData(std::vector<uint8_t> &data) override
    {
        return NFC_SUCCESS;
    }

    ErrCode WriteNdefData(const std::vector<uint8_t> &data) override
    {
        return NFC_SUCCESS;
    }

    ErrCode RegListener(const sptr<INfcTagCallback> &callback) override
    {
        return NFC_SUCCESS;
    }

    ErrCode UnregListener(const sptr<INfcTagCallback> &callback) override
    {
        return NFC_SUCCESS;
    }
};

/*
 * @tc.number: NfcTagProxyTest_Construct_001
 * @tc.name: NfcTagProxyTest_Construct_001
 * @tc.desc: None
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_Construct_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_Construct_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    NfcTagProxy proxy(nullptr);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_Construct_001 end";
}

/*
 * @tc.number: NfcTagProxyTest_Init_001
 * @tc.name: NfcTagProxyTest_Init_001
 * @tc.desc: return NFC_REMOTE_DIED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_Init_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_Init_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = true;
    EXPECT_EQ(proxy.Init(), NFC_REMOTE_DIED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_Init_001 end";
}

/*
 * @tc.number: NfcTagProxyTest_Init_002
 * @tc.name: NfcTagProxyTest_Init_002
 * @tc.desc: return NFC_IPC_WRITETOKE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_Init_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_Init_002 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    EXPECT_EQ(proxy.Init(), NFC_IPC_WRITETOKE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_Init_002 end";
}

/*
 * @tc.number: NfcTagProxyTest_Init_003
 * @tc.name: NfcTagProxyTest_Init_003
 * @tc.desc: return NFC_IPC_SEND_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_Init_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_Init_003 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = -1;
    EXPECT_EQ(proxy.Init(), NFC_IPC_SEND_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_Init_003 end";
}

/*
 * @tc.number: NfcTagProxyTest_Init_004
 * @tc.name: NfcTagProxyTest_Init_004
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_Init_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_Init_004 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    MessageParcel::readInt32Mock_ = 0;
    EXPECT_EQ(proxy.Init(), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_Init_004 end";
}

/*
 * @tc.number: NfcTagProxyTest_UnInit_001
 * @tc.name: NfcTagProxyTest_UnInit_001
 * @tc.desc: return NFC_REMOTE_DIED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_UnInit_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnInit_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = true;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    EXPECT_EQ(proxy.Uninit(), NFC_REMOTE_DIED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnInit_001 end";
}

/*
 * @tc.number: NfcTagProxyTest_UnInit_002
 * @tc.name: NfcTagProxyTest_UnInit_002
 * @tc.desc: return NFC_IPC_WRITETOKE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_UnInit_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnInit_002 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    EXPECT_EQ(proxy.Uninit(), NFC_IPC_WRITETOKE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnInit_002 end";
}

/*
 * @tc.number: NfcTagProxyTest_UnInit_003
 * @tc.name: NfcTagProxyTest_UnInit_003
 * @tc.desc: return NFC_IPC_SEND_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_UnInit_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnInit_003 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = -1;
    EXPECT_EQ(proxy.Uninit(), NFC_IPC_SEND_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnInit_003 end";
}

/*
 * @tc.number: NfcTagProxyTest_UnInit_004
 * @tc.name: NfcTagProxyTest_UnInit_004
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_UnInit_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnInit_004 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    MessageParcel::readInt32Mock_ = 0;
    EXPECT_EQ(proxy.Uninit(), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnInit_004 end";
}

/*
 * @tc.number: NfcTagProxyTest_ReadNdefTag_001
 * @tc.name: NfcTagProxyTest_ReadNdefTag_001
 * @tc.desc: return NFC_REMOTE_DIED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_ReadNdefTag_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefTag_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = true;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::string tag;
    EXPECT_EQ(proxy.ReadNdefTag(tag), NFC_REMOTE_DIED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefTag_001 end";
}

/*
 * @tc.number: NfcTagProxyTest_ReadNdefTag_002
 * @tc.name: NfcTagProxyTest_ReadNdefTag_002
 * @tc.desc: return NFC_IPC_WRITETOKE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_ReadNdefTag_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefTag_002 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::string tag;
    EXPECT_EQ(proxy.ReadNdefTag(tag), NFC_IPC_WRITETOKE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefTag_002 end";
}

/*
 * @tc.number: NfcTagProxyTest_ReadNdefTag_003
 * @tc.name: NfcTagProxyTest_ReadNdefTag_003
 * @tc.desc: return NFC_IPC_SEND_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_ReadNdefTag_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefTag_003 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = -1;
    std::string tag;
    EXPECT_EQ(proxy.ReadNdefTag(tag), NFC_IPC_SEND_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefTag_003 end";
}

/*
 * @tc.number: NfcTagProxyTest_ReadNdefTag_004
 * @tc.name: NfcTagProxyTest_ReadNdefTag_004
 * @tc.desc: return NFC_IPC_READ_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_ReadNdefTag_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefTag_004 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    MessageParcel::readRetMock_ = false;
    std::string tag;
    EXPECT_EQ(proxy.ReadNdefTag(tag), NFC_IPC_READ_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefTag_004 end";
}

/*
 * @tc.number: NfcTagProxyTest_ReadNdefTag_005
 * @tc.name: NfcTagProxyTest_ReadNdefTag_005
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_ReadNdefTag_005, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefTag_005 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    MessageParcel::readRetMock_ = true;
    MessageParcel::readInt32Mock_ = 0;
    std::string tag;
    EXPECT_EQ(proxy.ReadNdefTag(tag), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefTag_005 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefTag_001
 * @tc.name: NfcTagProxyTest_WriteNdefTag_001
 * @tc.desc: return NFC_REMOTE_DIED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefTag_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = true;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::string tag = "test";
    EXPECT_EQ(proxy.WriteNdefTag(tag), NFC_REMOTE_DIED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_001 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefTag_002
 * @tc.name: NfcTagProxyTest_WriteNdefTag_002
 * @tc.desc: return NFC_INVALID_PARAMETER
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefTag_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_002 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::string tag = "";
    EXPECT_EQ(proxy.WriteNdefTag(tag), NFC_INVALID_PARAMETER);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_002 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefTag_003
 * @tc.name: NfcTagProxyTest_WriteNdefTag_003
 * @tc.desc: return NFC_IPC_WRITETOKE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefTag_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_003 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::string tag = "test";
    EXPECT_EQ(proxy.WriteNdefTag(tag), NFC_IPC_WRITETOKE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_003 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefTag_004
 * @tc.name: NfcTagProxyTest_WriteNdefTag_004
 * @tc.desc: return NFC_IPC_WRITE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefTag_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_004 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = -1;
    std::string tag = "test";
    EXPECT_EQ(proxy.WriteNdefTag(tag), NFC_IPC_WRITE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_004 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefTag_005
 * @tc.name: NfcTagProxyTest_WriteNdefTag_005
 * @tc.desc: return NFC_IPC_SEND_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefTag_005, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_005 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = -1;
    std::string tag = "test";
    EXPECT_EQ(proxy.WriteNdefTag(tag), NFC_IPC_SEND_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_005 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefTag_006
 * @tc.name: NfcTagProxyTest_WriteNdefTag_006
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefTag_006, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_006 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::string tag = "test";
    MessageParcel::readRetMock_ = true;
    MessageParcel::readInt32Mock_ = 0;
    EXPECT_EQ(proxy.WriteNdefTag(tag), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefTag_006 end";
}

/*
 * @tc.number: NfcTagProxyTest_ReadNdefData_001
 * @tc.name: NfcTagProxyTest_ReadNdefData_001
 * @tc.desc: return NFC_REMOTE_DIED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_ReadNdefData_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefData_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = true;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::vector<uint8_t> tag;
    EXPECT_EQ(proxy.ReadNdefData(tag), NFC_REMOTE_DIED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefData_001 end";
}

/*
 * @tc.number: NfcTagProxyTest_ReadNdefData_002
 * @tc.name: NfcTagProxyTest_ReadNdefData_002
 * @tc.desc: return NFC_IPC_WRITETOKE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_ReadNdefData_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefData_002 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::vector<uint8_t> tag;
    EXPECT_EQ(proxy.ReadNdefData(tag), NFC_IPC_WRITETOKE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefData_002 end";
}

/*
 * @tc.number: NfcTagProxyTest_ReadNdefData_003
 * @tc.name: NfcTagProxyTest_ReadNdefData_003
 * @tc.desc: return NFC_IPC_SEND_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_ReadNdefData_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefData_003 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = -1;
    std::vector<uint8_t> tag;
    EXPECT_EQ(proxy.ReadNdefData(tag), NFC_IPC_SEND_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefData_003 end";
}

/*
 * @tc.number: NfcTagProxyTest_ReadNdefData_004
 * @tc.name: NfcTagProxyTest_ReadNdefData_004
 * @tc.desc: return NFC_IPC_READ_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_ReadNdefData_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefData_004 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    MessageParcel::readRetMock_ = true;
    MessageParcel::readVectorUInt8RetMock_ = false;
    std::vector<uint8_t> tag;
    EXPECT_EQ(proxy.ReadNdefData(tag), NFC_IPC_READ_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefData_004 end";
}

/*
 * @tc.number: NfcTagProxyTest_ReadNdefData_005
 * @tc.name: NfcTagProxyTest_ReadNdefData_005
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_ReadNdefData_005, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefData_005 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    MessageParcel::readRetMock_ = true;
    MessageParcel::readVectorUInt8RetMock_ = true;
    MessageParcel::readInt32Mock_ = 0;
    std::vector<uint8_t> tag;
    EXPECT_EQ(proxy.ReadNdefData(tag), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_ReadNdefData_005 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefData_001
 * @tc.name: NfcTagProxyTest_WriteNdefData_001
 * @tc.desc: return NFC_REMOTE_DIED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefData_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = true;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::vector<uint8_t> tag = {3, 1, 0};
    EXPECT_EQ(proxy.WriteNdefData(tag), NFC_REMOTE_DIED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_001 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefData_002
 * @tc.name: NfcTagProxyTest_WriteNdefData_002
 * @tc.desc: return NFC_INVALID_PARAMETER
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefData_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_002 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::vector<uint8_t> tag = {};
    EXPECT_EQ(proxy.WriteNdefData(tag), NFC_INVALID_PARAMETER);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_002 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefData_003
 * @tc.name: NfcTagProxyTest_WriteNdefData_003
 * @tc.desc: return NFC_IPC_WRITETOKE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefData_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_003 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::vector<uint8_t> tag = {3, 1, 0};
    EXPECT_EQ(proxy.WriteNdefData(tag), NFC_IPC_WRITETOKE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_003 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefData_004
 * @tc.name: NfcTagProxyTest_WriteNdefData_004
 * @tc.desc: return NFC_IPC_WRITE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefData_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_004 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeVectorUInt8RetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = -1;
    std::vector<uint8_t> tag = {3, 1, 0};
    EXPECT_EQ(proxy.WriteNdefData(tag), NFC_IPC_WRITE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_004 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefData_005
 * @tc.name: NfcTagProxyTest_WriteNdefData_005
 * @tc.desc: return NFC_IPC_SEND_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefData_005, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_005 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeVectorUInt8RetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = -1;
    std::vector<uint8_t> tag = {3, 1, 0};
    EXPECT_EQ(proxy.WriteNdefData(tag), NFC_IPC_SEND_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_005 end";
}

/*
 * @tc.number: NfcTagProxyTest_WriteNdefData_006
 * @tc.name: NfcTagProxyTest_WriteNdefData_006
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_WriteNdefData_006, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_006 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeVectorUInt8RetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    std::vector<uint8_t> tag = {3, 1, 0};
    MessageParcel::readRetMock_ = true;
    MessageParcel::readInt32Mock_ = 0;
    EXPECT_EQ(proxy.WriteNdefData(tag), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_WriteNdefData_006 end";
}

/*
 * @tc.number: NfcTagProxyTest_RegListener_001
 * @tc.name: NfcTagProxyTest_RegListener_001
 * @tc.desc: return NFC_REMOTE_DIED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_RegListener_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = true;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    EXPECT_EQ(proxy.RegListener(nullptr), NFC_REMOTE_DIED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_001 end";
}

/*
 * @tc.number: NfcTagProxyTest_RegListener_002
 * @tc.name: NfcTagProxyTest_RegListener_002
 * @tc.desc: return NFC_INVALID_PARAMETER
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_RegListener_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_002 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    EXPECT_EQ(proxy.RegListener(nullptr), NFC_INVALID_PARAMETER);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_002 end";
}

/*
 * @tc.number: NfcTagProxyTest_RegListener_003
 * @tc.name: NfcTagProxyTest_RegListener_003
 * @tc.desc: return NFC_IPC_WRITETOKE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_RegListener_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_003 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    sptr<NfcTagCallbackStub> callback = new NfcTagCallbackStub();
    EXPECT_EQ(proxy.RegListener(callback), NFC_IPC_WRITETOKE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_003 end";
}

/*
 * @tc.number: NfcTagProxyTest_RegListener_004
 * @tc.name: NfcTagProxyTest_RegListener_004
 * @tc.desc: return NFC_IPC_WRITE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_RegListener_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_004 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRemoteRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    sptr<NfcTagCallbackStub> callback = new NfcTagCallbackStub();
    EXPECT_EQ(proxy.RegListener(callback), NFC_IPC_WRITE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_004 end";
}

/*
 * @tc.number: NfcTagProxyTest_RegListener_005
 * @tc.name: NfcTagProxyTest_RegListener_005
 * @tc.desc: return NFC_IPC_SEND_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_RegListener_005, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_005 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRemoteRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = -1;
    sptr<NfcTagCallbackStub> callback = new NfcTagCallbackStub();
    EXPECT_EQ(proxy.RegListener(callback), NFC_IPC_SEND_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_005 end";
}

/*
 * @tc.number: NfcTagProxyTest_RegListener_006
 * @tc.name: NfcTagProxyTest_RegListener_006
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_RegListener_006, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_006 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRemoteRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    MessageParcel::readInt32Mock_ = 0;
    sptr<NfcTagCallbackStub> callback = new NfcTagCallbackStub();
    EXPECT_EQ(proxy.RegListener(callback), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_RegListener_006 end";
}

/*
 * @tc.number: NfcTagProxyTest_UnregListener_001
 * @tc.name: NfcTagProxyTest_UnregListener_001
 * @tc.desc: return NFC_REMOTE_DIED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_UnregListener_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = true;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    EXPECT_EQ(proxy.UnregListener(nullptr), NFC_REMOTE_DIED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_001 end";
}

/*
 * @tc.number: NfcTagProxyTest_UnregListener_002
 * @tc.name: NfcTagProxyTest_UnregListener_002
 * @tc.desc: return NFC_INVALID_PARAMETER
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_UnregListener_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_002 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    EXPECT_EQ(proxy.UnregListener(nullptr), NFC_INVALID_PARAMETER);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_002 end";
}

/*
 * @tc.number: NfcTagProxyTest_UnregListener_003
 * @tc.name: NfcTagProxyTest_UnregListener_003
 * @tc.desc: return NFC_IPC_WRITETOKE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_UnregListener_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_003 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    sptr<NfcTagCallbackStub> callback = new NfcTagCallbackStub();
    EXPECT_EQ(proxy.UnregListener(callback), NFC_IPC_WRITETOKE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_003 end";
}

/*
 * @tc.number: NfcTagProxyTest_UnregListener_004
 * @tc.name: NfcTagProxyTest_UnregListener_004
 * @tc.desc: return NFC_IPC_WRITE_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_UnregListener_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_004 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRemoteRetMock_ = false;
    IPCObjectStub::sendRequestRetMock_ = 0;
    sptr<NfcTagCallbackStub> callback = new NfcTagCallbackStub();
    EXPECT_EQ(proxy.UnregListener(callback), NFC_IPC_WRITE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_004 end";
}

/*
 * @tc.number: NfcTagProxyTest_UnregListener_005
 * @tc.name: NfcTagProxyTest_UnregListener_005
 * @tc.desc: return NFC_IPC_SEND_FAILED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_UnregListener_005, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_005 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRemoteRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = -1;
    sptr<NfcTagCallbackStub> callback = new NfcTagCallbackStub();
    EXPECT_EQ(proxy.UnregListener(callback), NFC_IPC_SEND_FAILED);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_005 end";
}

/*
 * @tc.number: NfcTagProxyTest_UnregListener_006
 * @tc.name: NfcTagProxyTest_UnregListener_006
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_UnregListener_006, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_006 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    proxy.remoteDied_ = false;
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRemoteRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    MessageParcel::readInt32Mock_ = 0;
    sptr<NfcTagCallbackStub> callback = new NfcTagCallbackStub();
    EXPECT_EQ(proxy.UnregListener(callback), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_UnregListener_006 end";
}

/*
 * @tc.number: NfcTagProxyTest_OnRemoteDied_001
 * @tc.name: NfcTagProxyTest_OnRemoteDied_001
 * @tc.desc: return NFC_REMOTE_DIED
*/
HWTEST_F(NfcTagProxyTest, NfcTagProxyTest_OnRemoteDied_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagProxyTest_OnRemoteDied_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagService> stubTest = new NfcTagService();
    NfcTagProxy proxy(stubTest);
    IRemoteProxyTest::remoteNull = true;
    proxy.OnRemoteDied(nullptr);
    GTEST_LOG_(INFO) << "NfcTagProxyTest_OnRemoteDied_001 end";
}

} // namespace NFC
} // namespace OHOS