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

#include <memory>

#include "call.hpp"

struct TraceVisitor {
   
    void visit(const Trace &trace) {
        preVisit(trace);
        visitHeader();
        for (auto &call : trace.calls()) {
            visitCall(call);
            for (auto &param : call.params()) {
                visitCallParam(param);
            }
        }
        postVisit();
    }

    virtual void preVisit(const Trace &trace) {};
    virtual void postVisit() {};
    virtual void visitHeader() {}; // TODO define header
    virtual void visitCall(const Call &call) {};
    virtual void visitCallParam(const std::unique_ptr<CallParam> &param) {};
};
