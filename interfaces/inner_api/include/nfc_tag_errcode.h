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
#ifndef OHOS_NFC_TAG_ERRCODE_H
#define OHOS_NFC_TAG_ERRCODE_H

namespace OHOS {
namespace NFC {
/* nfc tag error code defines */
enum ErrCode {
    NFC_SUCCESS = 0,
    NFC_REMOTE_DIED,
    NFC_IPC_WRITETOKE_FAILED,
    NFC_INVALID_TOKEN,
    NFC_IPC_SEND_FAILED,
    NFC_IPC_WRITE_FAILED,
    NFC_IPC_READ_FAILED,
    NFC_NO_CALLBACK,
    NFC_NO_REMOTE,
    NFC_NO_OBJECT,
    NFC_NO_HDI_PROXY,
    NFC_NO_HDI_IMPL,
    NFC_GRANT_FAILED,
    NFC_SYS_PERM_FAILED,
    NFC_INVALID_PROXY,
    NFC_INVALID_CALLBACKSTUB,
    NFC_INVALID_CALLBACK,
    NFC_INVALID_PARAMETER,
    NFC_CALLBACK_REGISTERED,
    NFC_CALLBACK_NOT_REGISTERED,
    NFC_CALLBACK_NOT_EQUAL,
    NFC_TOO_MANY_CALLBACK,
    NFC_HDI_REMOTE_FAILED,
    NFC_INVALID_STATE,
    NFC_ERR_MAX
};

/* ---------Feature service ability id--------- */
#define NFC_CONNECTED_TAG_ABILITY_ID 1148

}  // namespace NFC
}  // namespace OHOS
#endif