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
#ifndef NAPI_NFC_TAG_IMPL_H_
#define NAPI_NFC_TAG_IMPL_H_

#include <mutex>
#include "singleton.h"
#include "nfc_tag_errcode.h"
#include "infc_tag_callback.h"
#include "nfc_tag_proxy.h"
#include "nfc_tag_callback_stub.h"

namespace OHOS {
namespace NFC {
class NfcTagClient final : public DelayedRefSingleton<NfcTagClient> {
    DECLARE_DELAYED_REF_SINGLETON(NfcTagClient);

public:
    DISALLOW_COPY_AND_MOVE(NfcTagClient);
public:

    ErrCode Init();

    ErrCode Uninit();

    ErrCode ReadNdefTag(std::string &response);

    ErrCode WriteNdefTag(const std::string &data);

    ErrCode ReadNdefData(std::vector<uint8_t> &data);

    ErrCode WriteNdefData(const std::vector<uint8_t> &data);

    ErrCode RegListener(const sptr<INfcTagCallback> &callback);

    ErrCode UnregListener(const sptr<INfcTagCallback> &callback);

    static const char* GetErrCodeString(ErrCode code);

private:
    sptr<INfcTagService> GetService();
private:
    sptr<INfcTagService> serviceProxy_{};
    std::mutex proxyLock_;
    sptr<NfcTagCallbackStub> callbackStub_{};
    std::mutex callbackLock_;
};
}  // namespace NFC
}  // namespace OHOS
#endif
