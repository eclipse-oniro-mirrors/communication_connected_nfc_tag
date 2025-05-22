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

#ifndef NFC_TAG_IREMOTE_OBJECT_H
#define NFC_TAG_IREMOTE_OBJECT_H

#include <string>
#include "refbase.h"
#include "message_option.h"
#include "message_parcel.h"

namespace OHOS {
class IRemoteObject : public virtual RefBase {
public:
    class DeathRecipient : public virtual RefBase {
    public:
        virtual void OnRemoteDied(const wptr<IRemoteObject> &object) = 0;
    };
    virtual int SendRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option) = 0;
    virtual void GetInterfaceDescriptor() {}
    virtual bool AddDeathRecipient(const sptr<DeathRecipient> &recipient)
    {
        return true;
    }
    virtual bool RemoveDeathRecipient(const sptr<DeathRecipient> &recipient)
    {
        return true;
    }
    bool IsProxyObject()
    {
        return false;
    }
    const std::u16string descriptor_;
};

} // namespace OHOS

#endif // NFC_TAG_IREMOTE_OBJECT_H