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
#ifndef NFC_NAPI_UTILS_H_
#define NFC_NAPI_UTILS_H_

#include <string>
#include <chrono>
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "nfc_tag_errcode.h"

namespace OHOS {
namespace NFC {

class AsyncContext {
public:
    napi_env env_;
    napi_async_work work_;
    napi_deferred deferred_;
    napi_ref callback_ = 0;
    std::function<void(void*)> executeFunc_;
    std::function<void(void*)> completeFunc_;
    napi_value resourceName_;
    napi_value result_;
    ErrCode errorCode_;

    explicit AsyncContext(napi_env e, napi_async_work w = nullptr, napi_deferred d = nullptr)
    {
        env_ = e;
        work_ = w;
        deferred_ = d;
        executeFunc_ = nullptr;
        completeFunc_ = nullptr;
        result_ = nullptr;
        errorCode_ = NFC_ERR_MAX;
    }

    AsyncContext() = delete;

    virtual ~AsyncContext()
    {
    }
};

napi_value CreateUndefined(napi_env env);
napi_value DoAsyncWork(const napi_env& env, AsyncContext *asyncContext,
    const size_t argc, const napi_value *argv, const size_t nonCallbackArgNum);
bool ParseString(napi_env env, std::string &param, napi_value args);
napi_status ParseByteArray(napi_env env, napi_value array, std::vector<uint8_t> &outVec);
napi_value CreateNumberArray(napi_env env, const std::vector<uint8_t> &outVec);
std::string BuildErrorMessage(int errCode);
int FormatErrorCode(ErrCode statusCode);
napi_value GenerateBusinessError(const napi_env &env, int errCode, const std::string &errMessage);
napi_value CheckNfcStatusCodeAndThrow(const napi_env &env, ErrCode statusCode);

}  // namespace NFC
}  // namespace OHOS

#endif
