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

#ifndef NFC_TAG_IREMOTE_BROKER_H
#define NFC_TAG_IREMOTE_BROKER_H

#include "iremote_object.h"
#include "refbase.h"

namespace OHOS {
class IRemoteBroker : public virtual RefBase {
public:
    IRemoteBroker() = default;
    virtual ~IRemoteBroker() override = default;
    virtual sptr<IRemoteObject> AsObject() = 0;
};

template <typename T> class BrokerDelegator {
};

class IRemoteBrokerMock {
public:
    static sptr<IRemoteBroker> g_brokerMock;
};

template <typename INTERFACE> inline sptr<INTERFACE> iface_cast(const sptr<IRemoteObject> &object)
{
    return static_cast<INTERFACE *>(IRemoteBrokerMock::g_brokerMock.GetRefPtr());
}

} // namespace OHOS
#endif // NFC_TAG_IREMOTE_BROKER_H