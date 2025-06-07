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

#include "accesstoken_kit.h"
#include "ipc_skeleton.h"
#include "tokenid_kit.h"
#include "message_parcel.h"
#include "ipc_object_stub.h"
#include "iremote_broker.h"
#include "iremote_proxy.h"

using namespace OHOS;
using namespace OHOS::Security::AccessToken;

ATokenTypeEnum AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_INVALID;
HapTokenInfo AccessTokenKit::hapTokenInfo_{};
NativeTokenInfo AccessTokenKit::nativeTokenInfo_{};
int AccessTokenKit::accessTokenKitRet_ = -1;
int AccessTokenKit::hapTokenRet_ = -1;
int AccessTokenKit::nativeTokenRet_ = -1;

uint64_t IPCSkeleton::callingFullTokenId_ = 0;
int IPCSkeleton::callingTokenId_ = -1;
int IPCSkeleton::callingUid_ = -1;
int IPCSkeleton::callingPid_ = -1;

bool TokenIdKit::isSystemApp_ = false;

bool MessageParcel::writeRemoteRetMock_ = false;
sptr<IRemoteObject> MessageParcel::readRemoteMock_ = nullptr;
std::u16string MessageParcel::interfaceTokenMock_ = u"";
bool MessageParcel::writeRetMock_ = false;
bool MessageParcel::readRetMock_ = false;
bool MessageParcel::writeTokenRetMock_ = false;
std::string MessageParcel::readStringMock_{};
int32_t MessageParcel::readInt32Mock_ = -1;
int64_t MessageParcel::readInt64Mock_ = -1;
std::vector<uint8_t> MessageParcel::readVectorUInt8Mock_{};
bool MessageParcel::readVectorUInt8RetMock_ = false;
bool MessageParcel::writeVectorUInt8RetMock_ = false;

int IPCObjectStub::onRemoteRequestRetMock_ = -1;
int IPCObjectStub::sendRequestRetMock_ = -1;

sptr<IRemoteBroker> IRemoteBrokerMock::g_brokerMock = nullptr;
bool IRemoteProxyTest::remoteNull = false;

namespace OHOS {
namespace NFC {
void InitCommonMock(void)
{
    AccessTokenKit::tokenType_ = ATokenTypeEnum::TOKEN_INVALID;
    AccessTokenKit::hapTokenInfo_ = {};
    AccessTokenKit::nativeTokenInfo_ = {};
    AccessTokenKit::accessTokenKitRet_ = -1;
    AccessTokenKit::hapTokenRet_ = -1;
    AccessTokenKit::nativeTokenRet_ = -1;

    IPCSkeleton::callingFullTokenId_ = 0;
    IPCSkeleton::callingTokenId_ = -1;
    IPCSkeleton::callingUid_ = -1;
    IPCSkeleton::callingPid_ = -1;
    TokenIdKit::isSystemApp_ = false;

    MessageParcel::writeRemoteRetMock_ = false;
    MessageParcel::readRemoteMock_ = nullptr;
    MessageParcel::interfaceTokenMock_ = u"";
    MessageParcel::writeRetMock_ = false;
    MessageParcel::readRetMock_ = false;
    MessageParcel::writeTokenRetMock_ = false;
    MessageParcel::readStringMock_ = "";
    MessageParcel::readInt32Mock_ = -1;
    MessageParcel::readInt64Mock_ = -1;
    IPCObjectStub::onRemoteRequestRetMock_ = -1;
    IPCObjectStub::sendRequestRetMock_ = -1;
    MessageParcel::readVectorUInt8Mock_ = {};
    MessageParcel::readVectorUInt8RetMock_ = false;
    MessageParcel::writeVectorUInt8RetMock_ = false;

    IRemoteBrokerMock::g_brokerMock = nullptr;
    IRemoteProxyTest::remoteNull = false;
}

} // namespace NFC
} // namespace OHOS
