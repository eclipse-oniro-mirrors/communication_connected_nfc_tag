/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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

#ifndef NFC_TAG_LOG_H
#define NFC_TAG_LOG_H

#ifdef NFC_TAG_HILOG
#include "hilog/log.h"

#ifdef HILOGD
#undef HILOGD
#endif

#ifdef HILOGI
#undef HILOGI
#endif

#ifdef HILOGW
#undef HILOGW
#endif

#ifdef HILOGE
#undef HILOGE
#endif

#ifdef HILOGF
#undef HILOGF
#endif

#ifdef LOG_DOMAIN
#undef LOG_DOMAIN
#endif
#define LOG_DOMAIN 0xD000308

#ifndef LOG_TAG
#define LOG_TAG "NFCTAG"
#endif

#define HILOGF(fmt, ...) HILOG_FATAL( \
    LOG_CORE, "[(%{public}s:%{public}d)]" fmt, __func__, __LINE__, ##__VA_ARGS__)
#define HILOGE(fmt, ...) HILOG_ERROR( \
    LOG_CORE, "[(%{public}s:%{public}d)]" fmt, __func__, __LINE__, ##__VA_ARGS__)
#define HILOGW(fmt, ...) HILOG_WARN(  \
    LOG_CORE, "[(%{public}s:%{public}d)]" fmt, __func__, __LINE__, ##__VA_ARGS__)
#define HILOGI(fmt, ...) HILOG_INFO(  \
    LOG_CORE, "[(%{public}s:%{public}d)]" fmt, __func__, __LINE__, ##__VA_ARGS__)
#define HILOGD(fmt, ...) HILOG_DEBUG( \
    LOG_CORE, "[(%{public}s:%{public}d)]" fmt, __func__, __LINE__, ##__VA_ARGS__)
#else

#define HILOGD(...)
#define HILOGI(...)
#define HILOGW(...)
#define HILOGE(...)
#define HILOGF(...)
#endif  // NFC_TAG_HILOG

#define NFC_CHECK_RETURN(cond, fmt, ...)        \
    if (!(cond)) {                              \
        HILOGE(fmt, ##__VA_ARGS__);             \
        return;                                 \
    }
#define NFC_CHECK_RETURN_RET(cond, ret, fmt, ...)               \
    if (!(cond)) {                                              \
        HILOGE(fmt, ##__VA_ARGS__);                             \
        return ret;                                             \
    }

#endif  // NFC_TAG_LOG_H
