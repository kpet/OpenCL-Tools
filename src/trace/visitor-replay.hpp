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


#include "visitor.hpp"
#include "ocltools-loader-gen.hpp"

struct TraceReplayVisitor : TraceVisitor {
    
    template<typename T>
    struct ValueReplayHandler {
        ValueReplayHandler(const std::unique_ptr<CallParam> &param, char *&memory) {
            m_param = static_cast<CallParamValue<T>*>(param.get());
            memory += m_param->output_memory_requirements();
        }
        T value() const {
            return m_param->value();
        }
    private:
        CallParamValue<T> *m_param;
    };

    template<typename T>
    struct OptionalObjectCreationReplayHandler {
        OptionalObjectCreationReplayHandler(const std::unique_ptr<CallParam> &param, char *&memory) {
            m_param = static_cast<CallParamOptionalObjectCreation<T>*>(param.get());
            m_object_pointer = reinterpret_cast<T*>(memory);
            if (!m_param->create()) {
                m_object_pointer = nullptr;
            }
            memory += m_param->output_memory_requirements();
        }

        ~OptionalObjectCreationReplayHandler() {
            auto objects_ids = m_param->object_ids();
            for (auto i = 0u; i < objects_ids.size(); i++) {
                object_replay_tracker<T>().add(objects_ids[i], m_object_pointer[i]);
            }
        }

        T* object_pointer() const {
            return m_object_pointer;
        }
    private:
        T *m_object_pointer;
        CallParamOptionalObjectCreation<T> *m_param;
    };

    template<typename T>
    struct ValueOutByRefReplayHandler {
        ValueOutByRefReplayHandler(const std::unique_ptr<CallParam> &param, char *&memory) {
            m_param = static_cast<CallParamValueOutByRef<T>*>(param.get());
            m_pointer = reinterpret_cast<T*>(memory);
            if (m_param->null_pointer()) {
                m_pointer = nullptr;
            }
            memory += m_param->output_memory_requirements();
        }

        T* pointer() const {
            return m_pointer;
        }
    private:
        T* m_pointer;
        CallParamValueOutByRef<T> *m_param;
    };

    template<typename T>
    struct ObjectUseReplayHandler {
        ObjectUseReplayHandler(const std::unique_ptr<CallParam> &param, char *&memory) {
            m_param = static_cast<CallParamObjectUse<T>*>(param.get());
            for (auto id : m_param->object_ids()) {
                m_objects.push_back(object_replay_tracker<T>().get(id));
            }
        }

        T const * objects() const {
            return m_objects.data();
        }

    private:
        CallParamObjectUse<T> *m_param;
        std::vector<T> m_objects;
    };


    void preVisit(const Trace &trace) override {
        // Get memory requirements
        auto size = trace.output_memory_requirements();

        // Allocate memory
        m_memory = static_cast<char*>(malloc(size));

    }

    void visitCall(const Call &call) override {
       
        auto id = call.id();
        auto &params = call.params();
        auto &retval = call.retval();

        info("Replaying %s...", oclapi::command_name(id));

        switch(call.id()) {
        case oclapi::command::GET_PLATFORM_IDS: {

            ValueReplayHandler<cl_uint> p0(params[0], m_memory);
            OptionalObjectCreationReplayHandler<cl_platform_id> p1(params[1], m_memory);
            ValueOutByRefReplayHandler<cl_uint> p2(params[2], m_memory);

            auto ret = PFN_clGetPlatformIDs(p0.value(), p1.object_pointer(), p2.pointer());

            ValueReplayHandler<decltype(ret)> retp(retval, m_memory);
            if (retp.value() != ret) {
                warn("Returned value (%d) different from captured value (%d)\n", retp.value(), ret);
            }

            info("returned %d", ret);
            break;
        }

        case oclapi::command::GET_PLATFORM_INFO: {

            ObjectUseReplayHandler<cl_platform_id> p0(params[0], m_memory);
            ValueReplayHandler<cl_platform_info> p1(params[1], m_memory);
            ValueReplayHandler<size_t> p2(params[2], m_memory);
            ValueOutByRefReplayHandler<void> p3(params[3], m_memory);
            ValueOutByRefReplayHandler<size_t> p4(params[4], m_memory);

            auto ret = PFN_clGetPlatformInfo(p0.objects()[0], p1.value(), p2.value(),
                                             p3.pointer(), p4.pointer());
            break;
        }
        default:
            fatal("NOT SUPPORTED IN REPLAY");
            break;
        }
    }

private:
    char *m_memory;
};
