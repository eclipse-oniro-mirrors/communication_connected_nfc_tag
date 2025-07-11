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
#include "nfc_tag_callback_proxy.h"
#include "nfc_tag_callback_stub.h"
#include "nfc_tag_hdi_adapter.h"
#include "accesstoken_kit.h"
#include "ipc_skeleton.h"
#include "tokenid_kit.h"
#include "common_mock.h"

using namespace testing;
using namespace testing::ext;

namespace OHOS {
namespace NFC {

class NfcTagCallBackProxyTest : public ::testing::Test {
protected:
void SetUp() override
{
    InitCommonMock();
}
void TearDown() override {}
};

/*
 * @tc.number: NfcTagCallBackProxyTest_OnNotify_001
 * @tc.name: NfcTagCallBackProxyTest_OnNotify_001
 * @tc.desc: return NFC_IPC_WRITETOKE_FAILED
*/
HWTEST_F(NfcTagCallBackProxyTest, NfcTagCallBackProxyTest_OnNotify_001, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackProxyTest_OnNotify_001 start";
    MessageParcel::writeTokenRetMock_ = false;
    sptr<NfcTagCallbackStub> stubTest = new NfcTagCallbackStub();
    NfcTagCallBackProxy proxy(stubTest);
    EXPECT_EQ(proxy.OnNotify(0), NFC_IPC_WRITETOKE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagCallBackProxyTest_OnNotify_001 end";
}

/*
 * @tc.number: NfcTagCallBackProxyTest_OnNotify_002
 * @tc.name: NfcTagCallBackProxyTest_OnNotify_002
 * @tc.desc: return NFC_IPC_WRITE_FAILED
*/
HWTEST_F(NfcTagCallBackProxyTest, NfcTagCallBackProxyTest_OnNotify_002, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackProxyTest_OnNotify_002 start";
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRetMock_ = false;
    sptr<NfcTagCallbackStub> stubTest = new NfcTagCallbackStub();
    NfcTagCallBackProxy proxy(stubTest);
    EXPECT_EQ(proxy.OnNotify(0), NFC_IPC_WRITE_FAILED);
    GTEST_LOG_(INFO) << "NfcTagCallBackProxyTest_OnNotify_002 end";
}

/*
 * @tc.number: NfcTagCallBackProxyTest_OnNotify_003
 * @tc.name: NfcTagCallBackProxyTest_OnNotify_003
 * @tc.desc: return NFC_IPC_SEND_FAILED
*/
HWTEST_F(NfcTagCallBackProxyTest, NfcTagCallBackProxyTest_OnNotify_003, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackProxyTest_OnNotify_003 start";
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = -1;
    sptr<NfcTagCallbackStub> stubTest = new NfcTagCallbackStub();
    NfcTagCallBackProxy proxy(stubTest);
    EXPECT_EQ(proxy.OnNotify(0), NFC_IPC_SEND_FAILED);
    GTEST_LOG_(INFO) << "NfcTagCallBackProxyTest_OnNotify_003 end";
}

/*
 * @tc.number: NfcTagCallBackProxyTest_OnNotify_004
 * @tc.name: NfcTagCallBackProxyTest_OnNotify_004
 * @tc.desc: return NFC_IPC_READ_FAILED
*/
HWTEST_F(NfcTagCallBackProxyTest, NfcTagCallBackProxyTest_OnNotify_004, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackProxyTest_OnNotify_004 start";
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    MessageParcel::readRetMock_ = false;
    sptr<NfcTagCallbackStub> stubTest = new NfcTagCallbackStub();
    NfcTagCallBackProxy *proxy = new NfcTagCallBackProxy(stubTest);
    EXPECT_EQ(proxy->OnNotify(0), NFC_IPC_READ_FAILED);
    GTEST_LOG_(INFO) << "NfcTagCallBackProxyTest_OnNotify_004 end";
}

/*
 * @tc.number: NfcTagCallBackProxyTest_OnNotify_005
 * @tc.name: NfcTagCallBackProxyTest_OnNotify_005
 * @tc.desc: return NFC_SUCCESS
*/
HWTEST_F(NfcTagCallBackProxyTest, NfcTagCallBackProxyTest_OnNotify_005, TestSize.Level0)
{
    GTEST_LOG_(INFO) << "NfcTagCallBackProxyTest_OnNotify_005 start";
    MessageParcel::writeTokenRetMock_ = true;
    MessageParcel::writeRetMock_ = true;
    IPCObjectStub::sendRequestRetMock_ = 0;
    MessageParcel::readRetMock_ = true;
    sptr<NfcTagCallbackStub> stubTest = new NfcTagCallbackStub();
    NfcTagCallBackProxy *proxy = new NfcTagCallBackProxy(stubTest);
    EXPECT_EQ(proxy->OnNotify(0), NFC_SUCCESS);
    GTEST_LOG_(INFO) << "NfcTagCallBackProxyTest_OnNotify_005 end";
}

} // namespace NFC
} // namespace OHOS