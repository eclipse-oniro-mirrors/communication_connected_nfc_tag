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

#ifndef NFC_TAG_IREMOTE_PROXY_H
#define NFC_TAG_IREMOTE_PROXY_H
#include "iremote_broker.h"
#include "iremote_object.h"

namespace OHOS {

class IRemoteProxyTest {
public:
    static bool remoteNull;
};

template <typename INTERFACE> class IRemoteProxy : public INTERFACE {
public:
    explicit IRemoteProxy(const sptr<IRemoteObject> &object): object_(object)
    {}
    ~IRemoteProxy() override = default;
    sptr<IRemoteObject> Remote()
    {
        if (IRemoteProxyTest::remoteNull) {
            return nullptr;
        }
        return object_;
    }
protected:
    sptr<IRemoteObject> AsObject() override;
    sptr<IRemoteObject> object_;
};

template <typename INTERFACE> sptr<IRemoteObject> IRemoteProxy<INTERFACE>::AsObject()
{
    return Remote();
}

} // namespace OHOS
#endif // NFC_TAG_IREMOTE_PROXY_H