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

#ifndef NFC_TAG_ICONNECTED_NFC_TAG_H
#define NFC_TAG_ICONNECTED_NFC_TAG_H

#include "iconnected_nfc_tag_callback.h"
#include "hdi_base.h"

namespace OHOS {
namespace HDI {
namespace ConnectedNfcTag {
namespace V1_1 {

class IConnectedNfcTag : public HdiBase {
public:
    IConnectedNfcTag() {}
    ~IConnectedNfcTag() {}

    static sptr<OHOS::HDI::ConnectedNfcTag::V1_1::IConnectedNfcTag> Get()
    {
        if (noProxy_) {
            return nullptr;
        }
        if (iconnectedNfcTag_ == nullptr) {
            iconnectedNfcTag_ = sptr<IConnectedNfcTag>::MakeSptr();
        }
        return iconnectedNfcTag_;
    }

    virtual int32_t RegisterCallBack(const sptr<OHOS::HDI::ConnectedNfcTag::V1_1::IConnectedNfcTagCallback>& callbackObj)
    {
        return resultMock_;
    }

    virtual int32_t Init()
    {
        return resultMock_;
    }

    virtual int32_t Uninit()
    {
        return resultMock_;
    }

    virtual int32_t ReadNdefTag(std::string& ndefData)
    {
        return resultMock_;
    }

    virtual int32_t WriteNdefTag(const std::string& ndefData)
    {
        return resultMock_;
    }

    virtual int32_t ReadNdefData(std::vector<uint8_t>& ndefData)
    {
        return resultMock_;
    }

    virtual int32_t WriteNdefData(const std::vector<uint8_t>& ndefData)
    {
        return resultMock_;
    }

    static int32_t resultMock_;
    static bool noProxy_;
    static sptr<IConnectedNfcTag> iconnectedNfcTag_;
};
} // V1_1
} // ConnectedNfcTag
} // HDI
} // OHOS
#endif // NFC_TAG_ICONNECTED_NFC_TAG_H