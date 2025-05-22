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

#ifndef NFC_TAG_MESSAGE_PARCEL_H
#define NFC_TAG_MESSAGE_PARCEL_H

#include <string>
#include "refbase.h"

namespace OHOS {
class IRemoteObject;
class MessageParcel {
public:
    bool WriteRemoteObject(const sptr<IRemoteObject> &object)
    {
        return writeRemoteRetMock_;
    }
    sptr<IRemoteObject> ReadRemoteObject()
    {
        return readRemoteMock_;
    }
    bool WriteInterfaceToken(std::u16string name)
    {
        return writeTokenRetMock_;
    }
    std::u16string ReadInterfaceToken()
    {
        return interfaceTokenMock_;
    }
    bool WriteInt32(int32_t value)
    {
        return writeRetMock_;
    }
    bool WriteUint64(uint64_t value)
    {
        return writeRetMock_;
    }
    bool WriteString(std::string value)
    {
        return writeRetMock_;
    }
    bool ReadInt32(int32_t& value)
    {
        return readRetMock_;
    }
    int32_t ReadInt32()
    {
        return readInt32Mock_;
    }
    int64_t ReadInt64()
    {
        return readInt64Mock_;
    }
    bool ReadString(std::string& value)
    {
        value = readStringMock_;
        return readRetMock_;
    }
    size_t GetRawDataSize()
    {
        return 0;
    }
    bool WriteUInt8Vector(const std::vector<uint8_t> &val)
    {
        return writeVectorUInt8RetMock_;
    }
    bool ReadUInt8Vector(std::vector<uint8_t> *val)
    {
        *val = readVectorUInt8Mock_;
        return readVectorUInt8RetMock_;
    }
    std::u16string GetInterfaceToken() const;
    static bool writeRemoteRetMock_;
    static sptr<IRemoteObject> readRemoteMock_;
    static std::u16string interfaceTokenMock_;
    static bool writeRetMock_;
    static bool readRetMock_;
    static bool writeTokenRetMock_;
    static std::string readStringMock_;
    static int32_t readInt32Mock_;
    static int64_t readInt64Mock_;
    static int32_t writeInt32Save_;
    static bool readVectorUInt8RetMock_;
    static std::vector<uint8_t> readVectorUInt8Mock_;
    static bool writeVectorUInt8RetMock_;
};
} // OHOS

#endif // NFC_TAG_MESSAGE_PARCEL_H