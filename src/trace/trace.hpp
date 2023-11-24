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

#include "call.hpp"

#include <fstream>
#include <iostream>

#include "serialize.hpp"

// TODO Trace header
//    magic
//    format version
//    pointer size // TODO check on deserialisation, has to match
//    endianness?
//    capture time (UTC)
//    capture OS?

struct Trace {
    enum flags : uint32_t
    {
        kImperfect = (1 << 0),
    };

    void set_flag(flags f) { m_flags |= f; }

    bool has_calls() const { return m_calls.size() > 0; }

    void record(Call& call) { m_calls.push_back(std::move(call)); }

    void print(std::ostream& out) {
        for (auto& call : m_calls) {
            call.print(out);
        };
    }

    size_t output_memory_requirements() const {
        size_t size = 0;
        for (auto& call : m_calls) {
            size += call.output_memory_requirements();
        }
        return size;
    }

    void serialize(std::ostream& os) {
        ::serialize(os, m_flags);
        ::serialize(os, static_cast<uint32_t>(m_calls.size()));
        for (auto& call : m_calls) {
            call.serialize(os);
        }
    }

    void deserialize(std::istream& is) {
        m_flags = ::deserialize<uint32_t>(is);
        uint32_t num_calls = ::deserialize<uint32_t>(is);
        for (unsigned i = 0; i < num_calls; i++) {
            Call call(is);
            m_calls.push_back(std::move(call));
        }
    }

    void save(const std::string& filename) {
        std::ofstream os(filename, std::ios::binary);
        info("Serialising trace to %s ... ", filename.c_str());
        serialize(os);
        os.close();
        info("done.");
    }

    bool load(const std::string& filename) {
        std::ifstream is(filename, std::ios::binary);
        if (!is.good()) {
            error("Can't open '%s'\n", filename.c_str());
            return false;
        }
        deserialize(is);
        is.close();
        return true;
    }

    void print_info(std::ostream& os) {
        os << "Trace info" << std::endl;
        os << "Flags:";
        if (m_flags & flags::kImperfect) {
            os << " IMPERFECT";
        }
        os << std::endl;
        os << "Number of calls: " << m_calls.size() << std::endl;
        os << "Output memory requirements: " << output_memory_requirements()
           << " bytes" << std::endl;
    }

    void print_stats(std::ostream& os) const;

    const std::vector<Call>& calls() const { return m_calls; }

private:
    uint32_t m_flags;
    std::vector<Call> m_calls;
};
