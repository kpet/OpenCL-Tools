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

#define CL_API_SUFFIX_USER __attribute__((weak))

#include "cl_headers.hpp"
#include "log.hpp"

extern "C" {

void __attribute__((noreturn)) unimplemented(const char* name) {
    fatal("UNIMPLEMENTED: %s\n", name);
}

#include "ocltools-trace-stubs-gen.hpp"
}
