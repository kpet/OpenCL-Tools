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

#include "trace.hpp"

void Trace::print_stats(std::ostream& os) const {
    info("Stats\n");
    std::unordered_map<oclapi::command, unsigned long long> command_counts;
    for (auto& call : m_calls) {
        command_counts[call.id()] += 1;
    }

    for (auto& cmd_num : command_counts) {
        info("%s: %llu\n", oclapi::command_name(cmd_num.first), cmd_num.second);
    }
}
