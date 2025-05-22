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

#define LOG_TAG "NFCTAG_FWK_NAPI"
#include "nfc_tag_log.h"
#include "nfc_napi_adapter.h"
#include "nfc_napi_event.h"

namespace OHOS {
namespace NFC {

static const std::string NFC_RF_TYPE = "NfcRfType";
static constexpr int32_t NFC_RF_LEAVE = 0;
static constexpr int32_t NFC_RF_ENTER = 1;

static napi_value CreateEnumConstructor(napi_env env, napi_callback_info info)
{
    napi_value argReturn = nullptr;
    void *dataTmp = nullptr;

    napi_get_cb_info(env, info, nullptr, nullptr, &argReturn, &dataTmp);
    napi_value global = nullptr;
    napi_get_global(env, &global);
    return argReturn;
}

static napi_value CreateEnumNfcRfType(napi_env env, napi_value exports)
{
    napi_value leave = nullptr;
    napi_value enter = nullptr;
    napi_create_int32(env, NFC_RF_LEAVE, &leave);
    napi_create_int32(env, NFC_RF_ENTER, &enter);
    napi_property_descriptor desc[] = {
        DECLARE_NAPI_STATIC_PROPERTY("NFC_RF_LEAVE", leave),
        DECLARE_NAPI_STATIC_PROPERTY("NFC_RF_ENTER", enter),
    };
    napi_value result = nullptr;
    napi_define_class(env, NFC_RF_TYPE.c_str(), NAPI_AUTO_LENGTH, CreateEnumConstructor, nullptr,
        sizeof(desc) / sizeof(*desc), desc, &result);
    napi_set_named_property(env, exports, NFC_RF_TYPE.c_str(), result);
    return exports;
}

/*
 * Module initialization function
 */
static napi_value InitJs(napi_env env, napi_value exports)
{
    HILOGI("Init, nfc_napi_entry");

    napi_property_descriptor desc[] = {
        DECLARE_NAPI_FUNCTION("init", Init),
        DECLARE_NAPI_FUNCTION("uninit", Uninit),
        DECLARE_NAPI_FUNCTION("initialize", Initialize),
        DECLARE_NAPI_FUNCTION("uninitialize", UnInitialize),
        DECLARE_NAPI_FUNCTION("readNdefTag", ReadNdefTag),
        DECLARE_NAPI_FUNCTION("writeNdefTag", WriteNdefTag),
        DECLARE_NAPI_FUNCTION("read", ReadNdefData),
        DECLARE_NAPI_FUNCTION("write", WriteNdefData),
        DECLARE_NAPI_FUNCTION("on", On),
        DECLARE_NAPI_FUNCTION("off", Off),
    };

    NAPI_CALL(env, napi_define_properties(env, exports, sizeof(desc) / sizeof(napi_property_descriptor), desc));
    CreateEnumNfcRfType(env, exports);
    return exports;
}

static napi_module nfcConnectedTagModule = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = NULL,
    .nm_register_func = InitJs,
    .nm_modname = "connectedTag", // modname MUST matched with the file name of connectedTag.d.ts of js apis.
    .nm_priv = ((void *)0),
    .reserved = { 0 }
};

extern "C" __attribute__((constructor)) void RegisterConnectedTagModule(void)
{
    HILOGI("%{public}s nm_modname:%{public}s", __func__, nfcConnectedTagModule.nm_modname);
    napi_module_register(&nfcConnectedTagModule);
}
}  // namespace NFC
}  // namespace OHOS
