// Copyright 2019-2023 The OpenCL-Tools authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <ostream>

template <typename T> void serialize(std::ostream& os, const T& val) {
    os.write(reinterpret_cast<char*>(const_cast<T*>(&val)), sizeof(val));
}

template <> inline void serialize(std::ostream& os, const std::string& str) {
    uint32_t len = static_cast<uint32_t>(str.length());
    serialize(os, len);
    for (auto c : str) {
        serialize(os, c);
    }
}

template <typename T> T deserialize(std::istream& is) {
    T val;
    is.read(reinterpret_cast<char*>(&val), sizeof(val));
    return val;
}

template <> inline std::string deserialize(std::istream& is) {
    uint32_t len = deserialize<uint32_t>(is);
    std::string ret;
    for (uint32_t i = 0; i < len; i++) {
        ret += deserialize<char>(is);
    }
    return ret;
}
