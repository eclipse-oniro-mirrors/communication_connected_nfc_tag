/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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
#ifndef NAPI_NFC_TAG_IMPL_H_
#define NAPI_NFC_TAG_IMPL_H_

#include "define.h"
#include "infc_tag.h"
#include "infc_tag_callback.h"
#include "nfc_tag_session_proxy.h"

namespace OHOS {
namespace NFC {
class NfcTagImpl : public INfcTag {
public:
    explicit NfcTagImpl();
    virtual ~NfcTagImpl();

    static NfcTagImpl& GetInstance();

    ErrCode Init() override;

    ErrCode Uninit() override;

    ErrCode ReadNdefTag(std::string &response) override;

    ErrCode WriteNdefTag(std::string data) override;

    ErrCode RegListener(const sptr<INfcTagCallback> &callback) override;

    ErrCode UnregListener(const sptr<INfcTagCallback> &callback) override;
private:
    sptr<INfcTagSession> tagSessionProxy_;
};
}  // namespace NFC
}  // namespace OHOS
#endif
