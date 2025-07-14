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
#include "nfc_tag_hdi_adapter.h"
#include "v1_1/iconnected_nfc_tag.h"

using namespace testing;
using namespace testing::ext;

using namespace OHOS;
using namespace OHOS::HDI::ConnectedNfcTag;
using namespace OHOS::HDI::ConnectedNfcTag::V1_1;

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
 * @tc.number: NfcTagHdiAdapterTest_Init
 * @tc.name: NfcTagHdiAdapterTest_Init
 * @tc.desc: Init call
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_Init, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Init start";
    NfcTagHdiAdapter adapter;
    EXPECT_EQ(adapter.Init(), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Init end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_Uninit
 * @tc.name: NfcTagHdiAdapterTest_Uninit
 * @tc.desc: Uninit call
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_Uninit, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Uninit start";
    NfcTagHdiAdapter adapter;
    EXPECT_EQ(adapter.Uninit(), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_Uninit end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_ReadNdefTag
 * @tc.name: NfcTagHdiAdapterTest_ReadNdefTag
 * @tc.desc: ReadNdefTag call
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_ReadNdefTag, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefTag start";
    NfcTagHdiAdapter adapter;
    std::string tag;
    EXPECT_EQ(adapter.ReadNdefTag(tag), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefTag end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_WriteNdefTag
 * @tc.name: NfcTagHdiAdapterTest_WriteNdefTag
 * @tc.desc: WriteNdefTag call
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_WriteNdefTag, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefTag start";
    NfcTagHdiAdapter adapter;
    std::string tag;
    EXPECT_EQ(adapter.WriteNdefTag(tag), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefTag end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_ReadNdefData
 * @tc.name: NfcTagHdiAdapterTest_ReadNdefData
 * @tc.desc: ReadNdefData call
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_ReadNdefData, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefData start";
    NfcTagHdiAdapter adapter;
    std::vector<uint8_t> data;
    EXPECT_EQ(adapter.ReadNdefData(data), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_ReadNdefData end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_WriteNdefData
 * @tc.name: NfcTagHdiAdapterTest_WriteNdefData
 * @tc.desc: WriteNdefData call
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_WriteNdefData, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefData start";
    NfcTagHdiAdapter adapter;
    std::vector<uint8_t> data;
    EXPECT_EQ(adapter.WriteNdefData(data), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_WriteNdefData end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_RegisterCallBack
 * @tc.name: NfcTagHdiAdapterTest_RegisterCallBack
 * @tc.desc: RegisterCallBack call
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_RegisterCallBack, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_RegisterCallBack start";
    NfcTagHdiAdapter adapter;
    EXPECT_EQ(adapter.RegisterCallBack(nullptr), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_RegisterCallBack end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_UnRegisterCallBack
 * @tc.name: NfcTagHdiAdapterTest_UnRegisterCallBack
 * @tc.desc: UnRegisterCallBack call
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_UnRegisterCallBack, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_UnRegisterCallBack start";
    NfcTagHdiAdapter adapter;
    EXPECT_EQ(adapter.UnRegisterCallBack(nullptr), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_UnRegisterCallBack end";
}

/*
 * @tc.number: NfcTagHdiAdapterTest_InitDriver
 * @tc.name: NfcTagHdiAdapterTest_InitDriver
 * @tc.desc: InitDriver call
*/
HWTEST_F(NfcTagHdiAdapterTest, NfcTagHdiAdapterTest_InitDriver, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_InitDriver start";
    NfcTagHdiAdapter adapter;
    EXPECT_EQ(adapter.InitDriver(), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagHdiAdapterTest_InitDriver end";
}

} // namespace NFC
} // namespace OHOS