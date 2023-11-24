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

#include <cstdint>
#include <cstring>

namespace oclapi {

#include "ocl-api-gen.hpp"

static const char* command_name(command cmd) {
    return gCommandNames[static_cast<uint32_t>(cmd)];
}

static command command_enum(const char* name) {
    for (uint32_t i = 0; i < static_cast<uint32_t>(command::MAX_COMMAND); i++) {
        if (!strcmp(name, gCommandNames[i])) {
            return static_cast<command>(i);
        }
    }
    return command::MAX_COMMAND;
}

enum ocl_callback : uint32_t
{
    OCL_CALLBACK_CONTEXT_NOTIFICATION,
    OCL_CALLBACK_PROGRAM_BUILD,
};

} // namespace oclapi
