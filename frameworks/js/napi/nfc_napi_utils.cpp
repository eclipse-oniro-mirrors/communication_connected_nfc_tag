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
#include "nfc_napi_utils.h"
#include "nfc_tag_log.h"
#include "securec.h"
#include "nfc_tag_errcode.h"

namespace OHOS {
namespace NFC {

static constexpr int BUSI_ERR_PERM = 201; // Permission denied.
static constexpr int BUSI_ERR_PARAM = 401; // The parameter check failed.
static constexpr int BUSI_ERR_CAPABILITY = 801; // Capability not supported.
static constexpr int BUSI_ERR_NFC_STATE_INVALID = 3200101; // Connected NFC tag running state is abnormal in service.

const static std::string KEY_CODE = "code";
const std::string NFC_PERM_DESC = "ohos.permission.NFC_TAG";
static constexpr int ARGS_TWO = 2;

#define NAPI_NFCTAG_CALL_RETURN(func)                                      \
    do {                                                                   \
        napi_status ret = (func);                                          \
        if (ret != napi_ok) {                                              \
            HILOGE("napi call function failed. ret:%{public}d", ret);      \
            return ret;                                                    \
        }                                                                  \
    } while (0)

napi_value CreateUndefined(napi_env env)
{
    napi_value result = nullptr;
    NAPI_CALL(env, napi_get_undefined(env, &result));
    return result;
}

static napi_value InitAsyncCallBackEnv(const napi_env& env, AsyncContext *asyncContext,
    const size_t argc, const napi_value *argv, const size_t nonCallbackArgNum)
{
    if (nonCallbackArgNum < argc) {
        napi_valuetype valueType;
        NAPI_CALL(env, napi_typeof(env, argv[nonCallbackArgNum], &valueType));
        NAPI_ASSERT(env, valueType == napi_function, "Wrong argument type. Function expected.");
        napi_create_reference(env, argv[nonCallbackArgNum], 1, &asyncContext->callback_);
    }
    return nullptr;
}

static napi_value InitAsyncPromiseEnv(const napi_env& env, AsyncContext *asyncContext, napi_value& promise)
{
    napi_deferred deferred;
    NAPI_CALL(env, napi_create_promise(env, &deferred, &promise));
    asyncContext->deferred_ = deferred;
    return nullptr;
}

static napi_value DoCallBackAsyncWork(const napi_env& env, AsyncContext *asyncContext)
{
    napi_create_async_work(
        env,
        nullptr,
        asyncContext->resourceName_,
        [](napi_env env, void* data) {
            if (data == nullptr) {
                HILOGE("Async data parameter is null");
                return;
            }
            AsyncContext *context = static_cast<AsyncContext *>(data);
            context->executeFunc_(context);
        },
        [](napi_env env, napi_status status, void* data) {
            if (data == nullptr) {
                HILOGE("Async data parameter is null");
                return;
            }
            AsyncContext *context = static_cast<AsyncContext *>(data);
            context->completeFunc_(data);
            napi_value undefined;
            napi_get_undefined(env, &undefined);
            napi_value callback;
            napi_value callbackValues[ARGS_TWO] = {nullptr};
            napi_value result = nullptr;
            callbackValues[0] = context->errorCode_ == NFC_SUCCESS ? undefined : context->result_;
            callbackValues[1] = context->errorCode_ == NFC_SUCCESS ? context->result_ : undefined;
            napi_get_reference_value(env, context->callback_, &callback);
            napi_call_function(env, nullptr, callback, ARGS_TWO, callbackValues, &result);
            if (context->callback_ != nullptr) {
                napi_delete_reference(env, context->callback_);
            }
            napi_delete_async_work(env, context->work_);
            delete context;
        },
        (void *)asyncContext,
        &asyncContext->work_);
    NAPI_CALL(env, napi_queue_async_work(env, asyncContext->work_));
    return CreateUndefined(env);
}

static napi_value DoPromiseAsyncWork(const napi_env& env, AsyncContext *asyncContext)
{
    napi_create_async_work(
        env,
        nullptr,
        asyncContext->resourceName_,
        [](napi_env env, void *data) {
            if (data == nullptr) {
                HILOGE("Async data parameter is null");
                return;
            }
            AsyncContext *context = static_cast<AsyncContext *>(data);
            context->executeFunc_(context);
        },
        [](napi_env env, napi_status status, void *data) {
            if (data == nullptr) {
                HILOGE("Async data parameter is null");
                return;
            }
            AsyncContext *context = static_cast<AsyncContext *>(data);
            context->completeFunc_(data);
            if (context->errorCode_ == NFC_SUCCESS) {
                napi_resolve_deferred(context->env_, context->deferred_, context->result_);
            } else {
                napi_reject_deferred(context->env_, context->deferred_, context->result_);
            }
            napi_delete_async_work(env, context->work_);
            delete context;
        },
        (void *)asyncContext,
        &asyncContext->work_);
    napi_queue_async_work(env, asyncContext->work_);
    return CreateUndefined(env);
}

napi_value DoAsyncWork(const napi_env& env, AsyncContext *asyncContext,
    const size_t argc, const napi_value *argv, const size_t nonCallbackArgNum)
{
    if (argc > nonCallbackArgNum) {
        InitAsyncCallBackEnv(env, asyncContext, argc, argv, nonCallbackArgNum);
        return DoCallBackAsyncWork(env, asyncContext);
    } else {
        napi_value promise;
        InitAsyncPromiseEnv(env, asyncContext, promise);
        DoPromiseAsyncWork(env, asyncContext);
        return promise;
    }
}

bool ParseString(napi_env env, std::string &param, napi_value args)
{
    napi_valuetype valuetype;
    napi_typeof(env, args, &valuetype);

    HILOGI("param=%{public}d.", valuetype);
    if (valuetype != napi_string) {
        HILOGE("Wrong argument type. String expected.");
        return false;
    }
    size_t size = 0;

    if (napi_get_value_string_utf8(env, args, nullptr, 0, &size) != napi_ok) {
        HILOGE("can not get string size");
        param = "";
        return false;
    }
    param.reserve(size + 1);
    param.resize(size);
    if (napi_get_value_string_utf8(env, args, param.data(), (size + 1), &size) != napi_ok) {
        HILOGE("can not get string value");
        param = "";
        return false;
    }
    return true;
}

napi_status ParseByteArray(napi_env env, napi_value array, std::vector<uint8_t> &outVec)
{
    bool isArray = false;
    napi_status status = napi_is_array(env, array, &isArray);
    if (!isArray || status != napi_ok) {
        HILOGE("not array");
        return napi_invalid_arg;
    }
    std::vector<uint8_t> vec {};
    uint32_t length = 0;
    NAPI_NFCTAG_CALL_RETURN(napi_get_array_length(env, array, &length));
    for (uint32_t i = 0; i < length; ++i) {
        napi_value element;
        NAPI_NFCTAG_CALL_RETURN(napi_get_element(env, array, i, &element));
        int32_t result = 0;
        NAPI_NFCTAG_CALL_RETURN(napi_get_value_int32(env, element, &result));
        if (result > 0xFF || result < 0) {
            return napi_invalid_arg;
        }
        vec.emplace_back(static_cast<uint8_t>(result));
    }
    outVec = std::move(vec);
    return napi_ok;
}

napi_value CreateNumberArray(napi_env env, const std::vector<uint8_t> &outVec)
{
    napi_value jsArray = nullptr;
    napi_create_array(env, &jsArray);
    for (size_t i = 0; i < outVec.size(); ++i) {
        napi_value element;
        napi_create_int32(env, static_cast<int32_t>(outVec[i]), &element);
        napi_set_element(env, jsArray, i, element);
    }
    return jsArray;
}

std::string BuildErrorMessage(int errCode)
{
    std::string errMsg;
    switch (errCode) {
        case BUSI_ERR_PERM:
            errMsg.append("Permission denied. ")
                .append("forbidden by permission: ")
                .append(NFC_PERM_DESC)
                .append(".");
            break;
        case BUSI_ERR_PARAM:
            errMsg = "Parameter error. The parameter number is invalid.";
            break;
        case BUSI_ERR_CAPABILITY:
            errMsg = "Capability not supported.";
            break;
        case BUSI_ERR_NFC_STATE_INVALID:
            errMsg = "Connected NFC tag running state is abnormal in service.";
            break;
        default:
            errMsg = "Unknown error message.";
            break;
    }
    return errMsg;
}

napi_value GenerateBusinessError(const napi_env &env, int errCode, const std::string &errMessage)
{
    napi_value code = nullptr;
    napi_create_int32(env, errCode, &code);
    napi_value message = nullptr;
    napi_create_string_utf8(env, errMessage.c_str(), NAPI_AUTO_LENGTH, &message);
    napi_value businessError = nullptr;
    napi_create_error(env, code, message, &businessError);
    napi_set_named_property(env, businessError, KEY_CODE.c_str(), code);
    return businessError;
}

int FormatErrorCode(ErrCode statusCode)
{
    int resultCode = 0;
    switch (statusCode) {
        case NFC_SUCCESS:
            resultCode = 0;
            break;
        case NFC_GRANT_FAILED:
            resultCode = BUSI_ERR_PERM;
            break;
        case NFC_SYS_PERM_FAILED:
            resultCode = BUSI_ERR_CAPABILITY;
            break;
        case NFC_INVALID_PARAMETER:
            resultCode = BUSI_ERR_PARAM;
            break;
        default:
            resultCode = BUSI_ERR_NFC_STATE_INVALID;
            break;
    }
    return resultCode;
}

napi_value CheckNfcStatusCodeAndThrow(const napi_env &env, ErrCode statusCode)
{
    napi_value result = nullptr;
    switch (statusCode) {
        case NFC_SUCCESS:
            result = CreateUndefined(env);
            break;
        case NFC_GRANT_FAILED:
            result = GenerateBusinessError(env, BUSI_ERR_PERM,
                BuildErrorMessage(BUSI_ERR_PERM));
            napi_throw(env, result);
            break;
        case NFC_SYS_PERM_FAILED:
            result = GenerateBusinessError(env, BUSI_ERR_CAPABILITY,
                BuildErrorMessage(BUSI_ERR_CAPABILITY));
            napi_throw(env, result);
            break;
        case NFC_INVALID_PARAMETER:
            result = GenerateBusinessError(env, BUSI_ERR_PARAM,
                BuildErrorMessage(BUSI_ERR_PARAM));
            napi_throw(env, result);
            break;
        default:
            result = GenerateBusinessError(env, BUSI_ERR_NFC_STATE_INVALID,
                BuildErrorMessage(BUSI_ERR_NFC_STATE_INVALID));
            napi_throw(env, result);
            break;
    }
    return result;
}

}  // namespace NFC
}  // namespace OHOS
