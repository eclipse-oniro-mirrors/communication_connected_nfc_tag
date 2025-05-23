/*
 * Copyright (C) 2022-2025 Huawei Device Co., Ltd.
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
#ifndef NFC_INFC_TAG_CALLBACK_H
#define NFC_INFC_TAG_CALLBACK_H

#include <iremote_stub.h>
#include "nfc_tag_errcode.h"

namespace OHOS {
namespace NFC {
class INfcTagCallback : public IRemoteBroker {
public:
    virtual ErrCode OnNotify(int nfcRfState) = 0;
    enum {
        CMD_ON_NFC_TAG_NOTIFY = 0,
    };
public:
    DECLARE_INTERFACE_DESCRIPTOR(u"ohos.nfc.INfcTagCallback");
};
}  // namespace NFC
}  // namespace OHOS
#endif
