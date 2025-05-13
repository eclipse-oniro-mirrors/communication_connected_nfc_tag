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
#include "nfc_tag_hdi_adapter.h"
#include "v1_1/iconnected_nfc_tag.h"
#undef private

using namespace testing;
using namespace testing::ext;

using namespace OHOS;
using namespace OHOS::HDI::ConnectedNfcTag;
using namespace OHOS::HDI::ConnectedNfcTag::V1_1;

int32_t IConnectedNfcTag::resultMock_ = -1;
bool IConnectedNfcTag::noProxy_ = true;
sptr<IConnectedNfcTag> IConnectedNfcTag::iconnectedNfcTag_ = nullptr;

namespace OHOS {
namespace NFC {

class NfcTagHdiAdapterTest : public ::testing::Test {
protected:
void SetUp() override {}
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

/*
 * @tc.number: NfcTagHdiAdapterTest_GetInstance_001
 * @tc.name: NfcTagHdiAdapterTest_GetInstance_001
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_GetInstance_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_GetInstance_001 start";
    IConnectedNfcTag::resultMock_ = 0;
    IConnectedNfcTag::noProxy_ = false;
    EXPECT_EQ(NfcTagHdiAdapter::GetInstance().Init(), NFC_SUCCESS);
    EXPECT_EQ(NfcTagHdiAdapter::GetInstance().Uninit(), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_GetInstance_001 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_Init_001
 * @tc.name: NfcTagHdiAdapterTest_Init_001
 * @tc.desc: return NFC_NO_HDI_PROXY
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_Init_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Init_001 start";
    IConnectedNfcTag::noProxy_ = true;
    NfcTagHdiAdapter adapter;
    ErrCode ret = adapter.Init();
    EXPECT_EQ(ret, NFC_NO_HDI_PROXY);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Init_001 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_Init_002
 * @tc.name: NfcTagHdiAdapterTest_Init_002
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_Init_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Init_002 start";
    IConnectedNfcTag::resultMock_ = -1;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    ErrCode ret = adapter.Init();
    EXPECT_EQ(ret, NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Init_002 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_Init_003
 * @tc.name: NfcTagHdiAdapterTest_Init_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_Init_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Init_003 start";
    IConnectedNfcTag::resultMock_ = 0;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    ErrCode ret = adapter.Init();
    EXPECT_EQ(ret, NFC_SUCCESS);
    ret = adapter.Uninit();
    EXPECT_EQ(ret, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Init_003 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_Uninit_001
 * @tc.name: NfcTagHdiAdapterTest_Uninit_001
 * @tc.desc: return NFC_NO_HDI_PROXY
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_Uninit_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Uninit_001 start";
    NfcTagHdiAdapter adapter;
    IConnectedNfcTag::noProxy_ = true;
    ErrCode ret = adapter.Uninit();
    EXPECT_EQ(ret, NFC_NO_HDI_PROXY);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Uninit_001 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_Uninit_002
 * @tc.name: NfcTagHdiAdapterTest_Uninit_002
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_Uninit_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Uninit_002 start";
    NfcTagHdiAdapter adapter;
    IConnectedNfcTag::resultMock_ = -1;
    IConnectedNfcTag::noProxy_ = false;
    ErrCode ret = adapter.Uninit();
    EXPECT_EQ(ret, NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Uninit_002 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_Uninit_003
 * @tc.name: NfcTagHdiAdapterTest_Uninit_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_Uninit_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Uninit_003 start";
    IConnectedNfcTag::resultMock_ = 0;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    ErrCode ret = adapter.Uninit();
    EXPECT_EQ(ret, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Uninit_003 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_ReadNdefTag_001
 * @tc.name: NfcTagHdiAdapterTest_ReadNdefTag_001
 * @tc.desc: return NFC_NO_HDI_PROXY
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_ReadNdefTag_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefTag_001 start";
    IConnectedNfcTag::noProxy_ = true;
    NfcTagHdiAdapter adapter;
    std::string tag;
    ErrCode ret = adapter.ReadNdefTag(tag);
    EXPECT_EQ(ret, NFC_NO_HDI_PROXY);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefTag_001 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_ReadNdefTag_002
 * @tc.name: NfcTagHdiAdapterTest_ReadNdefTag_002
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_ReadNdefTag_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefTag_002 start";
    IConnectedNfcTag::resultMock_ = -1;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    std::string tag;
    ErrCode ret = adapter.ReadNdefTag(tag);
    EXPECT_EQ(ret, NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefTag_002 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_ReadNdefTag_003
 * @tc.name: NfcTagHdiAdapterTest_ReadNdefTag_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_ReadNdefTag_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefTag_003 start";
    IConnectedNfcTag::resultMock_ = 0;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    std::string tag;
    ErrCode ret = adapter.ReadNdefTag(tag);
    EXPECT_EQ(ret, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefTag_003 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_WriteNdefTag_001
 * @tc.name: NfcTagHdiAdapterTest_WriteNdefTag_001
 * @tc.desc: return NFC_NO_HDI_PROXY
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_WriteNdefTag_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefTag_001 start";
    IConnectedNfcTag::noProxy_ = true;
    NfcTagHdiAdapter adapter;
    std::string tag = "test";
    ErrCode ret = adapter.WriteNdefTag(tag);
    EXPECT_EQ(ret, NFC_NO_HDI_PROXY);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefTag_001 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_WriteNdefTag_002
 * @tc.name: NfcTagHdiAdapterTest_WriteNdefTag_002
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_WriteNdefTag_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefTag_002 start";
    IConnectedNfcTag::resultMock_ = -1;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    std::string tag = "test";
    ErrCode ret = adapter.WriteNdefTag(tag);
    EXPECT_EQ(ret, NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefTag_002 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_WriteNdefTag_003
 * @tc.name: NfcTagHdiAdapterTest_WriteNdefTag_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_WriteNdefTag_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefTag_003 start";
    IConnectedNfcTag::resultMock_ = 0;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    std::string tag = "test";
    ErrCode ret = adapter.WriteNdefTag(tag);
    EXPECT_EQ(ret, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefTag_003 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_ReadNdefData_001
 * @tc.name: NfcTagHdiAdapterTest_ReadNdefData_001
 * @tc.desc: return NFC_NO_HDI_PROXY
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_ReadNdefData_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefData_001 start";
    IConnectedNfcTag::noProxy_ = true;
    NfcTagHdiAdapter adapter;
    std::vector<uint8_t> tag;
    ErrCode ret = adapter.ReadNdefData(tag);
    EXPECT_EQ(ret, NFC_NO_HDI_PROXY);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefData_001 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_ReadNdefData_002
 * @tc.name: NfcTagHdiAdapterTest_ReadNdefData_002
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_ReadNdefData_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefData_002 start";
    IConnectedNfcTag::resultMock_ = -1;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    std::vector<uint8_t> tag;
    ErrCode ret = adapter.ReadNdefData(tag);
    EXPECT_EQ(ret, NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefData_002 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_ReadNdefData_003
 * @tc.name: NfcTagHdiAdapterTest_ReadNdefData_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_ReadNdefData_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefData_003 start";
    IConnectedNfcTag::resultMock_ = 0;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    std::vector<uint8_t> tag;
    ErrCode ret = adapter.ReadNdefData(tag);
    EXPECT_EQ(ret, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefData_003 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_WriteNdefData_001
 * @tc.name: NfcTagHdiAdapterTest_WriteNdefData_001
 * @tc.desc: return NFC_NO_HDI_PROXY
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_WriteNdefData_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefData_001 start";
    IConnectedNfcTag::noProxy_ = true;
    NfcTagHdiAdapter adapter;
    std::vector<uint8_t> tag = {3, 1, 0};
    ErrCode ret = adapter.WriteNdefData(tag);
    EXPECT_EQ(ret, NFC_NO_HDI_PROXY);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefData_001 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_WriteNdefData_002
 * @tc.name: NfcTagHdiAdapterTest_WriteNdefData_002
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_WriteNdefData_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefData_002 start";
    IConnectedNfcTag::resultMock_ = -1;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    std::vector<uint8_t> tag = {3, 1, 0};
    ErrCode ret = adapter.WriteNdefData(tag);
    EXPECT_EQ(ret, NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefData_002 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_WriteNdefData_003
 * @tc.name: NfcTagHdiAdapterTest_WriteNdefData_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_WriteNdefData_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefData_003 start";
    IConnectedNfcTag::resultMock_ = 0;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    std::vector<uint8_t> tag = {3, 1, 0};
    ErrCode ret = adapter.WriteNdefData(tag);
    EXPECT_EQ(ret, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefData_003 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_RegisterCallBack_001
 * @tc.name: NfcTagHdiAdapterTest_RegisterCallBack_001
 * @tc.desc: return NFC_NO_HDI_PROXY
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_RegisterCallBack_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_RegisterCallBack_001 start";
    IConnectedNfcTag::noProxy_ = true;
    NfcTagHdiAdapter adapter;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = adapter.RegisterCallBack(callbackTest);
    EXPECT_EQ(ret, NFC_NO_HDI_PROXY);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_RegisterCallBack_001 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_RegisterCallBack_002
 * @tc.name: NfcTagHdiAdapterTest_RegisterCallBack_002
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_RegisterCallBack_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_RegisterCallBack_002 start";
    IConnectedNfcTag::resultMock_ = -1;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = adapter.RegisterCallBack(callbackTest);
    EXPECT_EQ(ret, NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_RegisterCallBack_002 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_RegisterCallBack_003
 * @tc.name: NfcTagHdiAdapterTest_RegisterCallBack_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_RegisterCallBack_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_RegisterCallBack_003 start";
    IConnectedNfcTag::resultMock_ = 0;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = adapter.RegisterCallBack(callbackTest);
    EXPECT_EQ(ret, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_RegisterCallBack_003 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_UnRegisterCallBack_001
 * @tc.name: NfcTagHdiAdapterTest_UnRegisterCallBack_001
 * @tc.desc: return NFC_NO_HDI_PROXY
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_UnRegisterCallBack_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_UnRegisterCallBack_001 start";
    IConnectedNfcTag::noProxy_ = true;
    NfcTagHdiAdapter adapter;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = adapter.UnRegisterCallBack(callbackTest);
    EXPECT_EQ(ret, NFC_NO_HDI_PROXY);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_UnRegisterCallBack_001 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_UnRegisterCallBack_002
 * @tc.name: NfcTagHdiAdapterTest_UnRegisterCallBack_002
 * @tc.desc: return NFC_NO_CALLBACK
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_UnRegisterCallBack_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_UnRegisterCallBack_002 start";
    IConnectedNfcTag::resultMock_ = -1;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = adapter.UnRegisterCallBack(callbackTest);
    EXPECT_EQ(ret, NFC_NO_CALLBACK);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_UnRegisterCallBack_002 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_UnRegisterCallBack_003
 * @tc.name: NfcTagHdiAdapterTest_UnRegisterCallBack_003
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_UnRegisterCallBack_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_UnRegisterCallBack_003 start";
    IConnectedNfcTag::resultMock_ = 0;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    std::string tag = "test";
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = adapter.RegisterCallBack(callbackTest);
    EXPECT_EQ(ret, NFC_SUCCESS);
    ret = adapter.UnRegisterCallBack(callbackTest);
    EXPECT_EQ(ret, NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_UnRegisterCallBack_003 end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_UnRegisterCallBack_004
 * @tc.name: NfcTagHdiAdapterTest_UnRegisterCallBack_004
 * @tc.desc: return NFC_HDI_REMOTE_FAILED
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_UnRegisterCallBack_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_UnRegisterCallBack_004 start";
    IConnectedNfcTag::resultMock_ = 0;
    IConnectedNfcTag::noProxy_ = false;
    NfcTagHdiAdapter adapter;
    sptr<INfcTagCallbackTest> callbackTest = sptr<INfcTagCallbackTest>::MakeSptr();
    ErrCode ret = adapter.RegisterCallBack(callbackTest);
    EXPECT_EQ(ret, NFC_SUCCESS);
    IConnectedNfcTag::resultMock_ = -1;
    IConnectedNfcTag::noProxy_ = false;
    ret = adapter.UnRegisterCallBack(callbackTest);
    EXPECT_EQ(ret, NFC_HDI_REMOTE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_UnRegisterCallBack_004 end";
}

} // namespace NFC
} // namespace OHOS