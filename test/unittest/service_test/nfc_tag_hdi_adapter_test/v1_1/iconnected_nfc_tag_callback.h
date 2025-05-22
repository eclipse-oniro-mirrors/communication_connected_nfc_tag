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

#ifndef NFC_TAG_ICONNECTED_NFC_TAG_CALLBACK_H
#define NFC_TAG_ICONNECTED_NFC_TAG_CALLBACK_H
#include "hdi_base.h"

namespace OHOS {
namespace HDI {
namespace ConnectedNfcTag {
namespace V1_1 {

enum ConnectedNfcTagEvent : int32_t {
    NFC_TAG_EVENT_ENTER_FIELD = 0,
    NFC_TAG_EVENT_LEAVE_FIELD = 1,
    NFC_TAG_CHIP_RESTART = 2,
};

class IConnectedNfcTagCallback : public HdiBase {
public:
    virtual ~IConnectedNfcTagCallback() = default;

    virtual int32_t OnChipEvent(OHOS::HDI::ConnectedNfcTag::V1_1::ConnectedNfcTagEvent event,
        const std::vector<uint8_t>& message) = 0;
};
} // V1_1
} // ConnectedNfcTag
} // HDI
} // OHOS

#endif // NFC_TAG_ICONNECTED_NFC_TAG_CALLBACK_H