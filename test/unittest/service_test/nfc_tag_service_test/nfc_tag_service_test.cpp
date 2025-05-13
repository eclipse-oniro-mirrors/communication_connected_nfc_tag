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
#include "nfc_tag_service.h"
#include "nfc_tag_utils.h"
#undef private
#include "nfc_tag_hdi_adapter.h"
#include "accesstoken_kit.h"
#include "ipc_skeleton.h"
#include "tokenid_kit.h"
#include "common_mock.h"

using namespace testing;
using namespace testing::ext;
using namespace OHOS;
using namespace OHOS::Security::AccessToken;

namespace OHOS {
namespace NFC {

ErrCode g_hdiCodeMock = NFC_ERR_MAX;
bool g_isGrantedMock = false;
std::string g_callingNameMock = "";
ErrCode g_verifyPermissionMock = NFC_ERR_MAX;

class NfcTagCallBackManagerTest : public ::testing::Test {
protected:
void SetUp() override
{
    InitCommonMock();
}
void TearDown() override {}
};

class NfcTagServiceTest : public ::testing::Test {
protected:
void SetUp() override
{
    InitCommonMock();
    g_hdiCodeMock = NFC_ERR_MAX;
    g_isGrantedMock = false;
    g_callingNameMock = "";
    g_verifyPermissionMock = NFC_ERR_MAX;
}
void TearDown() override {}
};

class NfcTagHdiAdapter::Impl : public OHOS::RefBase {
};

NfcTagHdiAdapter::NfcTagHdiAdapter()
{
}

NfcTagHdiAdapter::~NfcTagHdiAdapter()
{
}

NfcTagHdiAdapter &NfcTagHdiAdapter::GetInstance()
{
    static NfcTagHdiAdapter sNfcTagHdiAdapter;
    return sNfcTagHdiAdapter;
}

ErrCode NfcTagHdiAdapter::Init()
{
    return g_hdiCodeMock;
}

ErrCode NfcTagHdiAdapter::Uninit()
{
    return g_hdiCodeMock;
}

ErrCode NfcTagHdiAdapter::ReadNdefTag(std::string &data)
{
    return g_hdiCodeMock;
}

ErrCode NfcTagHdiAdapter::WriteNdefTag(const std::string &data)
{
    return g_hdiCodeMock;
}

ErrCode NfcTagHdiAdapter::ReadNdefData(std::vector<uint8_t> &data)
{
    return g_hdiCodeMock;
}

ErrCode NfcTagHdiAdapter::WriteNdefData(const std::vector<uint8_t> &data)
{
    return g_hdiCodeMock;
}

ErrCode NfcTagHdiAdapter::RegisterCallBack(sptr<INfcTagCallback> listener)
{
    return g_hdiCodeMock;
}

ErrCode NfcTagHdiAdapter::UnRegisterCallBack(sptr<INfcTagCallback> listener)
{
    return g_hdiCodeMock;
}

NfcTagStub::NfcTagStub()
{}

NfcTagStub::~NfcTagStub()
{}

int NfcTagStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    return 0;
}

bool NfcTagStub::InitHandleMap()
{
    return true;
}

ErrCode NfcTagStub::OnInit(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return NFC_SUCCESS;
}

ErrCode NfcTagStub::OnUninit(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return NFC_SUCCESS;
}

ErrCode NfcTagStub::OnReadNdefTag(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return NFC_SUCCESS;
}

ErrCode NfcTagStub::OnWriteNdefTag(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return NFC_SUCCESS;
}

ErrCode NfcTagStub::OnReadNdefData(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return NFC_SUCCESS;
}

ErrCode NfcTagStub::OnWriteNdefData(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return NFC_SUCCESS;
}

ErrCode NfcTagStub::OnRegisterCallback(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return NFC_SUCCESS;
}

ErrCode NfcTagStub::OnUnRegisterCallback(uint32_t code, MessageParcel &data, MessageParcel &reply)
{
    return NFC_SUCCESS;
}

bool Utils::IsGranted(const std::string &permission)
{
    return g_isGrantedMock;
}

std::string Utils::GetCallingName()
{
    return g_callingNameMock;
}

ErrCode NfcTagServiceVerifyPermissionsHook(void)
{
    return g_verifyPermissionMock;
}

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

class NfcTagServiceMock : public NfcTagService {
public:
    NfcTagServiceMock(int32_t saId, bool runOnCreate) : NfcTagService(saId, runOnCreate) {}
    ~NfcTagServiceMock() {}
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
 * @tc.number: NfcTagCallBackManagerTest_RegisterListener_001
 * @tc.name: NfcTagCallBackManagerTest_RegisterListener_001
 * @tc.desc: return NFC_INVALID_CALLBACK
*/
HWTEST_F(NfcTagCallBackManagerTest, NfcTagCallBackManagerTest_RegisterListener_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_RegisterListener_001 start";
    NfcTagCallBackManager callbackManager;
    ErrCode ret = callbackManager.RegisterListener(nullptr);
    EXPECT_EQ(ret, NFC_INVALID_CALLBACK);
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_RegisterListener_001 end";
}

/*
 * @tc.number: NfcTagCallBackManagerTest_RegisterListener_002
 * @tc.name: NfcTagCallBackManagerTest_RegisterListener_002
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagCallBackManagerTest, NfcTagCallBackManagerTest_RegisterListener_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_RegisterListener_002 start";
    IPCSkeleton::callingFullTokenId_ = 123456789;
    NfcTagCallBackManager callbackManager;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = callbackManager.RegisterListener(callbackTest);
    EXPECT_EQ(ret, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_RegisterListener_002 end";
}

/*
 * @tc.number: NfcTagCallBackManagerTest_RegisterListener_003
 * @tc.name: NfcTagCallBackManagerTest_RegisterListener_003
 * @tc.desc: return NFC_CALLBACK_REGISTERED
*/
HWTEST_F(NfcTagCallBackManagerTest, NfcTagCallBackManagerTest_RegisterListener_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_RegisterListener_003 start";
    IPCSkeleton::callingFullTokenId_ = 123456789;
    NfcTagCallBackManager callbackManager;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = callbackManager.RegisterListener(callbackTest);
    EXPECT_EQ(ret, NFC_SUCCESS);
    ret = callbackManager.RegisterListener(callbackTest);
    EXPECT_EQ(ret, NFC_CALLBACK_REGISTERED);
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_RegisterListener_003 end";
}

/*
 * @tc.number: NfcTagCallBackManagerTest_RegisterListener_004
 * @tc.name: NfcTagCallBackManagerTest_RegisterListener_004
 * @tc.desc: return NFC_TOO_MANY_CALLBACK
*/
HWTEST_F(NfcTagCallBackManagerTest, NfcTagCallBackManagerTest_RegisterListener_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_RegisterListener_004 start";
    IPCSkeleton::callingFullTokenId_ = 1;
    NfcTagCallBackManager callbackManager;
    ErrCode ret = NFC_SUCCESS;
    for (int i = 0; i <= 30; i++) {
        sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
        ret = callbackManager.RegisterListener(callbackTest);
        if (i == 30) {
            EXPECT_EQ(ret, NFC_TOO_MANY_CALLBACK);
        } else {
            EXPECT_EQ(ret, NFC_SUCCESS);
        }
        IPCSkeleton::callingFullTokenId_++;
    }
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_RegisterListener_004 end";
}

/*
 * @tc.number: NfcTagCallBackManagerTest_UnRegisterListener_001
 * @tc.name: NfcTagCallBackManagerTest_UnRegisterListener_001
 * @tc.desc: return NFC_CALLBACK_NOT_REGISTERED
*/
HWTEST_F(NfcTagCallBackManagerTest, NfcTagCallBackManagerTest_UnRegisterListener_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_UnRegisterListener_001 start";
    IPCSkeleton::callingFullTokenId_ = 1;
    NfcTagCallBackManager callbackManager;
    ErrCode ret = callbackManager.UnRegisterListener(nullptr);
    EXPECT_EQ(ret, NFC_CALLBACK_NOT_REGISTERED);
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_UnRegisterListener_001 end";
}

/*
 * @tc.number: NfcTagCallBackManagerTest_UnRegisterListener_002
 * @tc.name: NfcTagCallBackManagerTest_UnRegisterListener_002
 * @tc.desc: return NFC_CALLBACK_NOT_EQUAL
*/
HWTEST_F(NfcTagCallBackManagerTest, NfcTagCallBackManagerTest_UnRegisterListener_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_UnRegisterListener_002 start";
    IPCSkeleton::callingFullTokenId_ = 1;
    NfcTagCallBackManager callbackManager;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = callbackManager.RegisterListener(callbackTest);
    EXPECT_EQ(ret, NFC_SUCCESS);
    ret = callbackManager.UnRegisterListener(nullptr);
    EXPECT_EQ(ret, NFC_CALLBACK_NOT_EQUAL);
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_UnRegisterListener_002 end";
}

/*
 * @tc.number: NfcTagCallBackManagerTest_OnNotify_001
 * @tc.name: NfcTagCallBackManagerTest_OnNotify_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagCallBackManagerTest, NfcTagCallBackManagerTest_OnNotify_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_OnNotify_001 start";
    IPCSkeleton::callingFullTokenId_ = 1;
    NfcTagCallBackManager callbackManager;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = callbackManager.RegisterListener(callbackTest);
    EXPECT_EQ(ret, NFC_SUCCESS);
    ret = callbackManager.OnNotify(1);
    EXPECT_EQ(ret, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagCallBackManagerTest_OnNotify_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_OnStart_001
 * @tc.name: NfcTagServiceTest_OnStart_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_OnStart_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_OnStart_001 start";
    NfcTagServiceMock service(123, true);
    service.OnStart();
    EXPECT_EQ(service.published_, true);
    EXPECT_EQ(service.state_, ServiceRunningState::STATE_RUNNING);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_OnStart_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_OnStop_001
 * @tc.name: NfcTagServiceTest_OnStop_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_OnStop_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_OnStop_001 start";
    NfcTagServiceMock service(123, true);
    service.OnStop();
    EXPECT_EQ(service.published_, false);
    EXPECT_EQ(service.state_, ServiceRunningState::STATE_NOT_START);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_OnStop_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_ServiceInit_001
 * @tc.name: NfcTagServiceTest_ServiceInit_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_ServiceInit_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ServiceInit_001 start";
    NfcTagServiceMock service(123, true);
    EXPECT_EQ(service.ServiceInit(), true);
    EXPECT_EQ(service.published_, true);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ServiceInit_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_VerifyPermissionsBeforeEntry_001
 * @tc.name: NfcTagServiceTest_VerifyPermissionsBeforeEntry_001
 * @tc.desc: return NFC_GRANT_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_VerifyPermissionsBeforeEntry_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_VerifyPermissionsBeforeEntry_001 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = false;
    EXPECT_EQ(service.VerifyPermissionsBeforeEntry(), NFC_GRANT_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_VerifyPermissionsBeforeEntry_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_VerifyPermissionsBeforeEntry_002
 * @tc.name: NfcTagServiceTest_VerifyPermissionsBeforeEntry_002
 * @tc.desc: return NFC_GRANT_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_VerifyPermissionsBeforeEntry_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_VerifyPermissionsBeforeEntry_002 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SYS_PERM_FAILED;
    EXPECT_EQ(service.VerifyPermissionsBeforeEntry(), NFC_SYS_PERM_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_VerifyPermissionsBeforeEntry_002 end";
}

/*
 * @tc.number: NfcTagServiceTest_VerifyPermissionsBeforeEntry_003
 * @tc.name: NfcTagServiceTest_VerifyPermissionsBeforeEntry_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_VerifyPermissionsBeforeEntry_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_VerifyPermissionsBeforeEntry_003 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    EXPECT_EQ(service.VerifyPermissionsBeforeEntry(), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_VerifyPermissionsBeforeEntry_003 end";
}

/*
 * @tc.number: NfcTagServiceTest_Init_001
 * @tc.name: NfcTagServiceTest_Init_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_Init_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Init_001 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    EXPECT_EQ(service.Init(), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Init_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_Init_002
 * @tc.name: NfcTagServiceTest_Init_002
 * @tc.desc: return NFC_GRANT_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_Init_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Init_002 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = false;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    EXPECT_EQ(service.Init(), NFC_GRANT_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Init_002 end";
}

/*
 * @tc.number: NfcTagServiceTest_Init_003
 * @tc.name: NfcTagServiceTest_Init_003
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_Init_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Init_003 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_HDI_REMOTE_FAILED;
    EXPECT_EQ(service.Init(), NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Init_003 end";
}

/*
 * @tc.number: NfcTagServiceTest_Uninit_001
 * @tc.name: NfcTagServiceTest_Uninit_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_Uninit_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Uninit_001 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    EXPECT_EQ(service.Uninit(), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Uninit_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_Uninit_002
 * @tc.name: NfcTagServiceTest_Uninit_002
 * @tc.desc: return NFC_GRANT_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_Uninit_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Uninit_002 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = false;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    EXPECT_EQ(service.Uninit(), NFC_GRANT_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Uninit_002 end";
}

/*
 * @tc.number: NfcTagServiceTest_Uninit_003
 * @tc.name: NfcTagServiceTest_Uninit_003
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_Uninit_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Uninit_003 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_HDI_REMOTE_FAILED;
    EXPECT_EQ(service.Uninit(), NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_Uninit_003 end";
}

/*
 * @tc.number: NfcTagServiceTest_ReadNdefTag_001
 * @tc.name: NfcTagServiceTest_ReadNdefTag_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_ReadNdefTag_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefTag_001 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    std::string resp;
    EXPECT_EQ(service.ReadNdefTag(resp), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefTag_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_ReadNdefTag_002
 * @tc.name: NfcTagServiceTest_ReadNdefTag_002
 * @tc.desc: return NFC_GRANT_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_ReadNdefTag_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefTag_002 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = false;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    std::string resp;
    EXPECT_EQ(service.ReadNdefTag(resp), NFC_GRANT_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefTag_002 end";
}

/*
 * @tc.number: NfcTagServiceTest_ReadNdefTag_003
 * @tc.name: NfcTagServiceTest_ReadNdefTag_003
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_ReadNdefTag_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefTag_003 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_HDI_REMOTE_FAILED;
    std::string resp;
    EXPECT_EQ(service.ReadNdefTag(resp), NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefTag_003 end";
}

/*
 * @tc.number: NfcTagServiceTest_WriteNdefTag_001
 * @tc.name: NfcTagServiceTest_WriteNdefTag_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_WriteNdefTag_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefTag_001 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    std::string data = "test";
    EXPECT_EQ(service.WriteNdefTag(data), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefTag_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_WriteNdefTag_002
 * @tc.name: NfcTagServiceTest_WriteNdefTag_002
 * @tc.desc: return NFC_GRANT_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_WriteNdefTag_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefTag_002 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = false;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    std::string data = "test";
    EXPECT_EQ(service.WriteNdefTag(data), NFC_GRANT_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefTag_002 end";
}

/*
 * @tc.number: NfcTagServiceTest_WriteNdefTag_003
 * @tc.name: NfcTagServiceTest_WriteNdefTag_003
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_WriteNdefTag_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefTag_003 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_HDI_REMOTE_FAILED;
    std::string data = "test";
    EXPECT_EQ(service.WriteNdefTag(data), NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefTag_003 end";
}

/*
 * @tc.number: NfcTagServiceTest_ReadNdefData_001
 * @tc.name: NfcTagServiceTest_ReadNdefData_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_ReadNdefData_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefData_001 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    std::vector<uint8_t> data;
    EXPECT_EQ(service.ReadNdefData(data), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefData_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_ReadNdefData_002
 * @tc.name: NfcTagServiceTest_ReadNdefData_002
 * @tc.desc: return NFC_GRANT_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_ReadNdefData_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefData_002 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = false;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    std::vector<uint8_t> data;
    EXPECT_EQ(service.ReadNdefData(data), NFC_GRANT_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefData_002 end";
}

/*
 * @tc.number: NfcTagServiceTest_ReadNdefData_003
 * @tc.name: NfcTagServiceTest_ReadNdefData_003
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_ReadNdefData_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefData_003 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_HDI_REMOTE_FAILED;
    std::vector<uint8_t> data;
    EXPECT_EQ(service.ReadNdefData(data), NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_ReadNdefData_003 end";
}

/*
 * @tc.number: NfcTagServiceTest_WriteNdefData_001
 * @tc.name: NfcTagServiceTest_WriteNdefData_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_WriteNdefData_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefData_001 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    std::vector<uint8_t> data = {3, 1, 0};
    EXPECT_EQ(service.WriteNdefData(data), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefData_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_WriteNdefData_002
 * @tc.name: NfcTagServiceTest_WriteNdefData_002
 * @tc.desc: return NFC_GRANT_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_WriteNdefData_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefData_002 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = false;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    std::vector<uint8_t> data = {3, 1, 0};
    EXPECT_EQ(service.WriteNdefData(data), NFC_GRANT_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefData_002 end";
}

/*
 * @tc.number: NfcTagServiceTest_WriteNdefData_003
 * @tc.name: NfcTagServiceTest_WriteNdefData_003
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_WriteNdefData_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefData_003 start";
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_HDI_REMOTE_FAILED;
    std::vector<uint8_t> data = {3, 1, 0};
    EXPECT_EQ(service.WriteNdefData(data), NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_WriteNdefData_003 end";
}

/*
 * @tc.number: NfcTagServiceTest_RegListener_001
 * @tc.name: NfcTagServiceTest_RegListener_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_RegListener_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_RegListener_001 start";
    IPCSkeleton::callingFullTokenId_ = 123456789;
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    EXPECT_EQ(service.RegListener(callbackTest), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_RegListener_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_RegListener_002
 * @tc.name: NfcTagServiceTest_RegListener_002
 * @tc.desc: return NFC_INVALID_CALLBACK
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_RegListener_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_RegListener_002 start";
    IPCSkeleton::callingFullTokenId_ = 123456789;
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    EXPECT_EQ(service.RegListener(nullptr), NFC_INVALID_CALLBACK);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_RegListener_002 end";
}


/*
 * @tc.number: NfcTagServiceTest_UnregListener_001
 * @tc.name: NfcTagServiceTest_UnregListener_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_UnregListener_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_UnregListener_001 start";
    IPCSkeleton::callingFullTokenId_ = 123456789;
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    EXPECT_EQ(service.RegListener(callbackTest), NFC_SUCCESS);
    EXPECT_EQ(service.UnregListener(callbackTest), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_UnregListener_001 end";
}

/*
 * @tc.number: NfcTagServiceTest_UnregListener_002
 * @tc.name: NfcTagServiceTest_UnregListener_002
 * @tc.desc: return NFC_INVALID_CALLBACK
*/
HWTEST_F(NfcTagServiceTest, NfcTagServiceTest_UnregListener_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagServiceTest_UnregListener_002 start";
    IPCSkeleton::callingFullTokenId_ = 123456789;
    NfcTagServiceMock service(123, true);
    g_isGrantedMock = true;
    g_verifyPermissionMock = NFC_SUCCESS;
    g_hdiCodeMock = NFC_SUCCESS;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    sptr<INfcTagCallbackTest> callbackTest2 = sptr<INfcTagCallbackTest>::MakeSptr();
    EXPECT_EQ(service.UnregListener(callbackTest), NFC_CALLBACK_NOT_REGISTERED);
    EXPECT_EQ(service.RegListener(callbackTest), NFC_SUCCESS);
    EXPECT_EQ(service.UnregListener(callbackTest2), NFC_CALLBACK_NOT_EQUAL);
    EXPECT_EQ(service.UnregListener(callbackTest), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagServiceTest_UnregListener_002 end";
}

} // namespace NFC
} // namespace OHOS