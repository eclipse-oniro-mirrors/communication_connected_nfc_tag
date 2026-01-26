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
#include "nfc_napi_adapter.h"
#include <vector>
#include <functional>
#include "nfc_tag_log.h"
#include "nfc_tag_errcode.h"
#include "nfc_tag_client.h"

namespace OHOS {
namespace NFC {

namespace {
constexpr size_t ARGC_ONE = 1;
constexpr size_t ARGC_TWO = 2;
}

napi_value Init(napi_env env, napi_callback_info info)
{
    ErrCode ret = DelayedRefSingleton<NfcTagClient>::GetInstance().Init();
    napi_value result;
    napi_get_boolean(env, ret == NFC_SUCCESS, &result);
    return result;
}

napi_value Uninit(napi_env env, napi_callback_info info)
{
    ErrCode ret = DelayedRefSingleton<NfcTagClient>::GetInstance().Uninit();
    napi_value result;
    napi_get_boolean(env, ret == NFC_SUCCESS, &result);
    return result;
}

napi_value Initialize(napi_env env, napi_callback_info info)
{
    ErrCode ret = DelayedRefSingleton<NfcTagClient>::GetInstance().Init();
    CheckNfcStatusCodeAndThrow(env, ret);
    return CreateUndefined(env);
}

napi_value UnInitialize(napi_env env, napi_callback_info info)
{
    ErrCode ret = DelayedRefSingleton<NfcTagClient>::GetInstance().Uninit();
    CheckNfcStatusCodeAndThrow(env, ret);
    return CreateUndefined(env);
}

napi_value ReadNdefTag(napi_env env, napi_callback_info info)
{
    size_t argc = ARGC_TWO;
    napi_value argv[ARGC_TWO];
    napi_value thisVar = nullptr;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, nullptr));
    HILOGI("argc = %{public}zu", argc);
    if (argc >= ARGC_TWO) {
        CheckNfcStatusCodeAndThrow(env, NFC_INVALID_PARAMETER);
        return CreateUndefined(env);
    }

    ReadAsyncContext *asyncContext = new ReadAsyncContext(env);
    napi_create_string_latin1(env, "readNdefTag", NAPI_AUTO_LENGTH, &asyncContext->resourceName_);

    asyncContext->executeFunc_ = [&](void* data) -> void {
        ReadAsyncContext *context = static_cast<ReadAsyncContext *>(data);
        context->errorCode_ = DelayedRefSingleton<NfcTagClient>::GetInstance().ReadNdefTag(context->respNdefData_);
        HILOGI("ReadNdefTag end errorCode = %{public}d", context->errorCode_);
    };

    asyncContext->completeFunc_ = [&](void* data) -> void {
        ReadAsyncContext *context = static_cast<ReadAsyncContext *>(data);
        if (context->errorCode_ == NFC_SUCCESS) {
            napi_create_string_utf8(context->env_, context->respNdefData_.c_str(), NAPI_AUTO_LENGTH, &context->result_);
            HILOGI("ReadNdefTag len = %{public}zu", context->respNdefData_.length());
        } else {
            int businessCode = FormatErrorCode(context->errorCode_);
            context->result_ = GenerateBusinessError(context->env_, businessCode, BuildErrorMessage(businessCode));
        }
    };

    size_t nonCallbackArgNum = 0;
    return DoAsyncWork(env, asyncContext, argc, argv, nonCallbackArgNum);
}

napi_value WriteNdefTag(napi_env env, napi_callback_info info)
{
    size_t argc = ARGC_TWO;
    napi_value argv[ARGC_TWO];
    napi_value thisVar = nullptr;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, nullptr));
    HILOGI("argc = %{public}zu", argc);
    if (argc > ARGC_TWO || argc == 0) {
        CheckNfcStatusCodeAndThrow(env, NFC_INVALID_PARAMETER);
        return CreateUndefined(env);
    }
    napi_valuetype valueType;
    if (argc == ARGC_TWO) {
        napi_typeof(env, argv[ARGC_ONE], &valueType);
        if (valueType != napi_function) {
            CheckNfcStatusCodeAndThrow(env, NFC_INVALID_PARAMETER);
            return CreateUndefined(env);
        }
    }

    std::string inputWrittenNdefData = "";
    if (!ParseString(env, inputWrittenNdefData, argv[0])) {
        CheckNfcStatusCodeAndThrow(env, NFC_INVALID_PARAMETER);
        return CreateUndefined(env);
    }
    HILOGI("WriteNdefTag argc = %{public}zu, len = %{public}zu", argc, inputWrittenNdefData.length());

    WriteAsyncContext *asyncContext = new WriteAsyncContext(env);
    napi_create_string_latin1(env, "writeNdefTag", NAPI_AUTO_LENGTH, &asyncContext->resourceName_);
    asyncContext->writtenNdefData_ = inputWrittenNdefData;

    asyncContext->executeFunc_ = [&](void* data) -> void {
        WriteAsyncContext *context = static_cast<WriteAsyncContext *>(data);
        context->errorCode_ = DelayedRefSingleton<NfcTagClient>::GetInstance().WriteNdefTag(context->writtenNdefData_);
        HILOGI("WriteNdefTag end errorCode = %{public}d", context->errorCode_);
    };

    asyncContext->completeFunc_ = [&](void* data) -> void {
        WriteAsyncContext *context = static_cast<WriteAsyncContext *>(data);
        if (context->errorCode_ == NFC_SUCCESS) {
            HILOGI("WriteNdefTag completeFunc in, no return value");
            context->result_ = CreateUndefined(context->env_);
        } else {
            int businessCode = FormatErrorCode(context->errorCode_);
            context->result_ = GenerateBusinessError(context->env_, businessCode, BuildErrorMessage(businessCode));
        }
    };

    size_t nonCallbackArgNum = 1;
    return DoAsyncWork(env, asyncContext, argc, argv, nonCallbackArgNum);
}

napi_value ReadNdefData(napi_env env, napi_callback_info info)
{
    size_t argc = ARGC_TWO;
    napi_value argv[ARGC_TWO];
    napi_value thisVar = nullptr;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, nullptr));
    HILOGI("argc = %{public}zu", argc);
    if (argc >= ARGC_TWO) {
        CheckNfcStatusCodeAndThrow(env, NFC_INVALID_PARAMETER);
        return CreateUndefined(env);
    }

    ReadDataAsyncContext *asyncContext = new ReadDataAsyncContext(env);
    napi_create_string_latin1(env, "readNdefData", NAPI_AUTO_LENGTH, &asyncContext->resourceName_);

    asyncContext->executeFunc_ = [&](void* data) -> void {
        ReadDataAsyncContext *context = static_cast<ReadDataAsyncContext *>(data);
        context->errorCode_ = DelayedRefSingleton<NfcTagClient>::GetInstance().ReadNdefData(context->respNdefData_);
        HILOGI("ReadNdefData end errorCode = %{public}d", context->errorCode_);
    };

    asyncContext->completeFunc_ = [&](void* data) -> void {
        ReadDataAsyncContext *context = static_cast<ReadDataAsyncContext *>(data);
        if (context->errorCode_ == NFC_SUCCESS) {
            context->result_ = CreateNumberArray(context->env_, context->respNdefData_);
        } else {
            int businessCode = FormatErrorCode(context->errorCode_);
            context->result_ = GenerateBusinessError(context->env_, businessCode, BuildErrorMessage(businessCode));
        }
    };

    size_t nonCallbackArgNum = 0;
    return DoAsyncWork(env, asyncContext, argc, argv, nonCallbackArgNum);
}

napi_value WriteNdefData(napi_env env, napi_callback_info info)
{
    size_t argc = ARGC_TWO;
    napi_value argv[ARGC_TWO];
    napi_value thisVar = nullptr;
    NAPI_CALL(env, napi_get_cb_info(env, info, &argc, argv, &thisVar, nullptr));
    HILOGI("argc = %{public}zu", argc);
    if (argc > ARGC_TWO || argc == 0) {
        CheckNfcStatusCodeAndThrow(env, NFC_INVALID_PARAMETER);
        return CreateUndefined(env);
    }
    napi_valuetype valueType;
    if (argc == ARGC_TWO) {
        napi_typeof(env, argv[ARGC_ONE], &valueType);
        if (valueType != napi_function) {
            CheckNfcStatusCodeAndThrow(env, NFC_INVALID_PARAMETER);
            return CreateUndefined(env);
        }
    }

    std::vector<uint8_t> inputWrittenNdefData;
    if (ParseByteArray(env, argv[0], inputWrittenNdefData) != napi_ok) {
        HILOGE("WriteNdefData ParseByteArray failed");
        CheckNfcStatusCodeAndThrow(env, NFC_INVALID_PARAMETER);
        return CreateUndefined(env);
    }
    HILOGI("WriteNdefData argc = %{public}zu, size = %{public}zu", argc, inputWrittenNdefData.size());

    WriteDataAsyncContext *asyncContext = new WriteDataAsyncContext(env);
    napi_create_string_latin1(env, "WriteNdefData", NAPI_AUTO_LENGTH, &asyncContext->resourceName_);
    asyncContext->writtenNdefData_ = inputWrittenNdefData;

    asyncContext->executeFunc_ = [&](void* data) -> void {
        WriteDataAsyncContext *context = static_cast<WriteDataAsyncContext *>(data);
        context->errorCode_ = DelayedRefSingleton<NfcTagClient>::GetInstance().WriteNdefData(context->writtenNdefData_);
        HILOGI("WriteNdefData end errorCode = %{public}d", context->errorCode_);
    };

    asyncContext->completeFunc_ = [&](void* data) -> void {
        WriteDataAsyncContext *context = static_cast<WriteDataAsyncContext *>(data);
        if (context->errorCode_ == NFC_SUCCESS) {
            HILOGI("WriteNdefData completeFunc in, no return value");
            context->result_ = CreateUndefined(context->env_);
        } else {
            int businessCode = FormatErrorCode(context->errorCode_);
            context->result_ = GenerateBusinessError(context->env_, businessCode, BuildErrorMessage(businessCode));
        }
    };

    size_t nonCallbackArgNum = 1;
    return DoAsyncWork(env, asyncContext, argc, argv, nonCallbackArgNum);
}

}  // namespace NFC
}  // namespace OHOS
