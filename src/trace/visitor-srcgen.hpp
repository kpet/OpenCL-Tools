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

#include <cassert>
#include <cstdarg>
#include <sstream>

static void __attribute__((noreturn)) unimplemented(const char* fmt, ...) {
    fprintf(stdout, "UNIMPLEMENTED in SRCGEN: ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
    fprintf(stdout, "\n");
    fflush(stdout);
    exit(EXIT_FAILURE);
}

std::string call_param_value_print(CallParam* param) {
    auto ptype = param->type();
    auto ttype = param->ttype();

    switch (ttype) {
    case CALL_PARAM_TEMPLATE_TYPE_CL_INT: {
        auto p = static_cast<CallParamValue<cl_int>*>(param);
        return std::to_string(p->value());
    }
    case CALL_PARAM_TEMPLATE_TYPE_CL_UINT: {
        auto p = static_cast<CallParamValue<cl_uint>*>(param);
        return std::to_string(p->value());
    }
    case CALL_PARAM_TEMPLATE_TYPE_CL_LONG: {
        auto p = static_cast<CallParamValue<cl_long>*>(param);
        return std::to_string(p->value());
    }
    case CALL_PARAM_TEMPLATE_TYPE_CL_ULONG: {
        auto p = static_cast<CallParamValue<cl_long>*>(param);
        return std::to_string(p->value());
    }
    }

    unimplemented("call_param_value_print");
}

const std::vector<uint64_t>& call_param_object_use_ids(CallParam* param) {
    auto ptype = param->type();
    auto ttype = param->ttype();
    assert(ptype = CALL_PARAM_OBJECT_USE);
    switch (ttype) {
    case CALL_PARAM_TEMPLATE_TYPE_CL_PLATFORM_ID:
        return static_cast<CallParamObjectUse<cl_platform_id>*>(param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_DEVICE_ID:
        return static_cast<CallParamObjectUse<cl_device_id>*>(param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_CONTEXT:
        return static_cast<CallParamObjectUse<cl_context>*>(param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_PROGRAM:
        return static_cast<CallParamObjectUse<cl_program>*>(param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_KERNEL:
        return static_cast<CallParamObjectUse<cl_kernel>*>(param)->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_MEM:
        return static_cast<CallParamObjectUse<cl_mem>*>(param)->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_EVENT:
        return static_cast<CallParamObjectUse<cl_event>*>(param)->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE:
        return static_cast<CallParamObjectUse<cl_command_queue>*>(param)
            ->object_ids();
    }

    unimplemented("call_param_object_use_ids");
}

bool call_param_object_use_multiple(CallParam* param) {
    auto ptype = param->type();
    auto ttype = param->ttype();
    assert(ptype = CALL_PARAM_OBJECT_USE);
    switch (ttype) {
    case CALL_PARAM_TEMPLATE_TYPE_CL_PLATFORM_ID:
        return static_cast<CallParamObjectUse<cl_platform_id>*>(param)
            ->multiple();
    case CALL_PARAM_TEMPLATE_TYPE_CL_DEVICE_ID:
        return static_cast<CallParamObjectUse<cl_device_id>*>(param)
            ->multiple();
    case CALL_PARAM_TEMPLATE_TYPE_CL_CONTEXT:
        return static_cast<CallParamObjectUse<cl_context>*>(param)->multiple();
    case CALL_PARAM_TEMPLATE_TYPE_CL_PROGRAM:
        return static_cast<CallParamObjectUse<cl_program>*>(param)->multiple();
    case CALL_PARAM_TEMPLATE_TYPE_CL_KERNEL:
        return static_cast<CallParamObjectUse<cl_kernel>*>(param)->multiple();
    case CALL_PARAM_TEMPLATE_TYPE_CL_MEM:
        return static_cast<CallParamObjectUse<cl_mem>*>(param)->multiple();
    case CALL_PARAM_TEMPLATE_TYPE_CL_EVENT:
        return static_cast<CallParamObjectUse<cl_event>*>(param)->multiple();
    case CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE:
        return static_cast<CallParamObjectUse<cl_command_queue>*>(param)
            ->multiple();
    }

    unimplemented("call_param_object_use_multiple");
}

const std::vector<uint64_t> call_param_object_creation_ids(CallParam* param) {
    auto ptype = param->type();
    auto ttype = param->ttype();
    assert(ptype = CALL_PARAM_OPTIONAL_OBJECT_CREATION);
    switch (ttype) {
    case CALL_PARAM_TEMPLATE_TYPE_CL_PLATFORM_ID:
        return static_cast<CallParamOptionalObjectCreation<cl_platform_id>*>(
                   param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_DEVICE_ID:
        return static_cast<CallParamOptionalObjectCreation<cl_device_id>*>(
                   param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_CONTEXT:
        return static_cast<CallParamOptionalObjectCreation<cl_context>*>(param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE:
        return static_cast<CallParamOptionalObjectCreation<cl_command_queue>*>(
                   param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_PROGRAM:
        return static_cast<CallParamOptionalObjectCreation<cl_program>*>(param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_KERNEL:
        return static_cast<CallParamOptionalObjectCreation<cl_kernel>*>(param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_MEM:
        return static_cast<CallParamOptionalObjectCreation<cl_mem>*>(param)
            ->object_ids();
    case CALL_PARAM_TEMPLATE_TYPE_CL_EVENT:
        return static_cast<CallParamOptionalObjectCreation<cl_event>*>(param)
            ->object_ids();
    }

    unimplemented("call_param_object_creation_ids");
}

bool call_param_array_null_pointer(CallParam* param) {
    auto ptype = param->type();
    auto ttype = param->ttype();
    assert(ptype = CALL_PARAM_ARRAY);
    switch (ttype) {
    case CALL_PARAM_TEMPLATE_TYPE_CL_ULONG:
        return static_cast<CallParamArray<cl_ulong>*>(param)->null_pointer();
    case CALL_PARAM_TEMPLATE_TYPE_CHAR:
        return static_cast<CallParamArray<char>*>(param)->null_pointer();
    case CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_FORMAT:
        return static_cast<CallParamArray<cl_image_format>*>(param)
            ->null_pointer();
    case CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_DESC:
        return static_cast<CallParamArray<cl_image_desc>*>(param)
            ->null_pointer();
    }

    unimplemented("call_param_array_null, ttype = %u", ttype);
}

std::string call_param_array_initialiser(CallParam* param) {
    auto ptype = param->type();
    auto ttype = param->ttype();
    assert(ptype = CALL_PARAM_ARRAY);
    std::string ret;
    switch (ttype) {
    case CALL_PARAM_TEMPLATE_TYPE_CL_ULONG: {
        auto p = static_cast<CallParamArray<cl_ulong>*>(param);
        auto& values = p->values();
        const char* sep = "";
        for (auto val : values) {
            ret += sep;
            ret += std::to_string(val);
            sep = ",";
        }
        break;
    }
    case CALL_PARAM_TEMPLATE_TYPE_CHAR: {
        auto p = static_cast<CallParamArray<char>*>(param);
        auto& values = p->values();
        const char* sep = "";
        for (auto val : values) {
            ret += sep;
            ret += std::to_string(val);
            sep = ",";
        }
        break;
    }
    case CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_FORMAT: {
        auto p = static_cast<CallParamArray<cl_image_format>*>(param);
        auto& values = p->values();
        const char* sep = "";
        for (auto val : values) {
            ret += sep;
            ret += "{" + std::to_string(val.image_channel_order) + "," +
                   std::to_string(val.image_channel_data_type) + "}";
            sep = ",";
        }
        break;
    }
    case CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_DESC: {
        auto p = static_cast<CallParamArray<cl_image_desc>*>(param);
        auto& values = p->values();
        const char* sep = "";
        for (auto val : values) {
            ret += sep;
            ret += "{";
            ret += std::to_string(val.image_type) + ",";
            ret += std::to_string(val.image_width) + ",";
            ret += std::to_string(val.image_height) + ",";
            ret += std::to_string(val.image_depth) + ",";
            ret += std::to_string(val.image_array_size) + ",";
            ret += std::to_string(val.image_row_pitch) + ",";
            ret += std::to_string(val.image_slice_pitch) + ",";
            ret += std::to_string(val.num_mip_levels) + ",";
            ret += std::to_string(val.num_samples) + ",";
            ret += "nullptr,";
            ret += "}";
            sep = ",";
        }
        break;
    }
    default:
        unimplemented("call_param_array_initialiser, ttype = %u", ttype);
        break;
    }

    return ret;
}

struct TraceSourceGenerationVisitor : public TraceVisitor {
private:
    std::string makeCallParamVarName(int param_num) const {
        std::string ret{"call_"};
        ret += std::to_string(m_call_num);
        ret += "_p";
        ret += std::to_string(param_num);
        return ret;
    }

    std::string makeObjectCreationVarName(CallParamTemplateType ttype,
                                          uint32_t num) const {
        std::string ret{"OBJECT_"};
        ret += call_param_template_type_name(ttype);
        ret += "_CREATION_" + std::to_string(num);
        return ret;
    }

    std::string makeVectorType(CallParamTemplateType ttype) const {
        std::string ret{"std::vector<"};
        ret += call_param_template_type_name(ttype);
        ret += ">";
        return ret;
    }

    using object_variables_tracker =
        std::unordered_map<uint64_t, std::pair<uint32_t, uint32_t>>;

    object_variables_tracker&
    selectObjectVariableTracker(CallParamTemplateType ttype) {
        switch (ttype) {
        case CALL_PARAM_TEMPLATE_TYPE_CL_PLATFORM_ID:
            return m_platform_object_variables;
        case CALL_PARAM_TEMPLATE_TYPE_CL_DEVICE_ID:
            return m_device_object_variables;
        case CALL_PARAM_TEMPLATE_TYPE_CL_CONTEXT:
            return m_context_object_variables;
        case CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE:
            return m_queue_object_variables;
        case CALL_PARAM_TEMPLATE_TYPE_CL_PROGRAM:
            return m_program_object_variables;
        case CALL_PARAM_TEMPLATE_TYPE_CL_KERNEL:
            return m_kernel_object_variables;
        case CALL_PARAM_TEMPLATE_TYPE_CL_MEM:
            return m_mem_object_variables;
        case CALL_PARAM_TEMPLATE_TYPE_CL_EVENT:
            return m_event_object_variables;
        }
        unimplemented("object variable tracker selection");
    }

    std::string handleObjectCreation(CallParamTemplateType ttype,
                                     const std::vector<uint64_t>& object_ids) {
        auto& vtracker = selectObjectVariableTracker(ttype);
        auto varname = makeObjectCreationVarName(ttype, m_object_creation_num);
        uint32_t object_cnt = 0;
        for (auto id : object_ids) {
            vtracker[id] = std::make_pair(m_object_creation_num, object_cnt++);
        }
        m_object_creation_num++;
        return varname;
    }

public:
    TraceSourceGenerationVisitor() : m_call_num(0), m_object_creation_num(0) {}

    const std::string source() const { return m_src.str(); }

    void preVisit(const Trace& trace) override {
        m_src << R"(
#include <vector>

#include <CL/cl.h>

int main(int argc, char* argv[]) {
)";
    }

    void visitCall(const Call& call) override {
        m_src << std::endl << "// Call " << m_call_num << std::endl;
        std::vector<std::string> call_var_values;
        // Declare variables for output parameters and object creation
        int param_num = 0;
        for (auto& par : call.params()) {
            auto param = par.get();
            auto ptype = param->type();
            auto ttype = param->ttype();
            std::string pstr;
            if (ptype == CALL_PARAM_OPTIONAL_OBJECT_CREATION) {
                auto object_ids = call_param_object_creation_ids(param);
                auto varname = handleObjectCreation(ttype, object_ids);
                if (object_ids.size() > 1) {
                    m_src << makeVectorType(ttype) << " " << varname << "("
                          << object_ids.size() << ");" << std::endl;
                    pstr = varname + ".data()";
                } else {
                    m_src << call_param_template_type_name(ttype) << " "
                          << varname << ";" << std::endl;
                    pstr = "&" + varname;
                }
            } else if (ptype == CALL_PARAM_VALUE) {
                pstr = makeCallParamVarName(param_num);
                m_src << call_param_template_type_name(ttype) << " " << pstr
                      << " = " << call_param_value_print(param) << ";"
                      << std::endl;
            } else if (ptype == CALL_PARAM_OBJECT_USE) {

                auto obj_type_name = call_param_template_type_name(ttype);
                auto ids = call_param_object_use_ids(param);

                if (call_param_object_use_multiple(param)) {
                    if (ids.size() > 1) {

                        // Declare vector to serve as input parameter
                        auto pvar = makeCallParamVarName(param_num);
                        m_src << makeVectorType(ttype) << " " << pvar << ";"
                              << std::endl;

                        // Assign objects to vector
                        uint32_t objcnt = 0;
                        for (auto id : ids) {
                            auto creation_index =
                                selectObjectVariableTracker(ttype).at(id);
                            auto cvar = makeObjectCreationVarName(
                                ttype, creation_index.first);
                            auto cval = cvar + "[" +
                                        std::to_string(creation_index.second) +
                                        "]";
                            m_src << pvar << "[" << objcnt << "] = " << cval
                                  << ";" << std::endl;
                        }
                        // Prepare parameter
                        pstr = pvar + ".data()";
                    } else if (ids.size() == 1) {
                        // Declare var to serve as input parameter
                        auto pvar = makeCallParamVarName(param_num);
                        auto creation_index =
                            selectObjectVariableTracker(ttype).at(ids[0]);
                        auto cvar = makeObjectCreationVarName(
                            ttype, creation_index.first);
                        m_src << obj_type_name << " " << pvar << " = " << cvar
                              << ";" << std::endl;
                        pstr = "&" + pvar;
                    } else {
                        pstr = "nullptr";
                    }
                } else {
                    // Declare var to serve as input parameter
                    auto pvar = makeCallParamVarName(param_num);
                    auto creation_index =
                        selectObjectVariableTracker(ttype).at(ids[0]);
                    auto cvar =
                        makeObjectCreationVarName(ttype, creation_index.first);
                    m_src << obj_type_name << " " << pvar << " = " << cvar
                          << ";" << std::endl;
                    pstr = pvar;
                }

            } else if (ptype == CALL_PARAM_VALUE_OUT_BY_REF) {
                auto varname = makeCallParamVarName(param_num);

                if (ttype == CALL_PARAM_TEMPLATE_TYPE_VOID) {
                    auto p = static_cast<CallParamValueOutByRef<void>*>(param);
                    m_src << "std::vector<char> " << varname << "("
                          << p->output_memory_requirements() << ");"
                          << std::endl;
                    pstr = varname + ".data()";
                } else {
                    m_src << call_param_template_type_name(ttype) << " "
                          << varname << ";" << std::endl;
                    pstr = "&" + varname;
                }
            } else if (ptype == CALL_PARAM_PROPERTIES) {
                auto varname = makeCallParamVarName(param_num);
                auto pcasted = static_cast<CallParamProperties*>(param);
                auto props = pcasted->properties();
                if (pcasted->has_list()) {
                    auto vectype =
                        makeVectorType(CALL_PARAM_TEMPLATE_TYPE_INTPTR_T);
                    m_src << vectype << " " << varname << " = {";
                    std::string sep;
                    for (auto prop : props) {
                        m_src << sep << prop;
                        sep = ", ";
                    }
                    m_src << sep << "0";
                    m_src << "};" << std::endl;

                    pstr = varname + ".data()";
                } else {
                    pstr = "nullptr";
                }
            } else if (ptype == CALL_PARAM_CALLBACK) {
                auto present =
                    static_cast<CallParamCallback*>(param)->present();
                if (present) {
                    abort(); // TODO
                } else {
                    pstr = "nullptr";
                }
            } else if (ptype == CALL_PARAM_CALLBACK_DATA) {
                auto present =
                    static_cast<CallParamCallbackData*>(param)->present();
                if (present) {
                    abort(); // TODO
                } else {
                    pstr = "nullptr";
                }
            } else if (ptype == CALL_PARAM_ARRAY) {
                auto null = call_param_array_null_pointer(param);
                if (null) {
                    pstr = "nullptr";
                } else {
                    auto init = call_param_array_initialiser(param);
                    auto varname = makeCallParamVarName(param_num);
                    m_src << makeVectorType(ttype) << " " << varname << " = {"
                          << init << "};" << std::endl;
                    pstr = varname + ".data()";
                }
            } else if (ptype == CALL_PARAM_PROGRAM_SOURCE) {
                auto varname = makeCallParamVarName(param_num);
                m_src << "std::vector<const char*> " << varname << " = {";
                for (auto& src :
                     static_cast<CallParamProgramSource*>(param)->sources()) {
                    m_src << "R\"(" << src << ")\",";
                }
                m_src << "};" << std::endl;
                pstr = varname + ".data()";
            } else if (ptype == CALL_PARAM_STRING) {
                auto varname = makeCallParamVarName(param_num);
                auto cps = static_cast<CallParamString*>(param);
                auto present = cps->present();
                if (present) {
                    pstr = "\"" + cps->str() + "\"";
                } else {
                    pstr = "nullptr";
                }
            } else if (ptype == CALL_PARAM_MAP_POINTER_USE) {
                auto mpu = static_cast<CallParamMapPointerUse*>(param);
                pstr = "MAP_PTR_" + std::to_string(mpu->id());
            } else {
                unimplemented("call param type in params");
            }
            param_num++;
            call_var_values.push_back(pstr);
        }

        // Return value
        auto& retval = call.retval();
        auto ttype = retval->ttype();
        switch (retval->type()) {
        case CALL_PARAM_VALUE:
            m_src << call_param_template_type_name(ttype) << " "
                  << "call_" << m_call_num << "_ret"
                  << " = ";
            break;
        case CALL_PARAM_OPTIONAL_OBJECT_CREATION: {
            auto& object_ids = call_param_object_creation_ids(retval.get());
            auto varname = handleObjectCreation(ttype, object_ids);
            m_src << call_param_template_type_name(ttype) << " " << varname
                  << " = ";
            break;
        }
        case CALL_PARAM_MAP_POINTER_CREATION: {
            auto p = static_cast<CallParamMapPointerCreation*>(retval.get());
            uint64_t id = p->id();
            m_src << "void* MAP_PTR_" << id << " = ";
            break;
        }
        default:
            unimplemented("call param type in return");
        }
        // TODO (optionally) check the return value is the same as at trace
        // capture time

        // Call
        std::string callfn{oclapi::command_name(call.id())};
        m_src << callfn << "(";
        std::string sep = "";
        for (auto& v : call_var_values) {
            m_src << sep << v;
            sep = ", ";
        }
        m_src << ");" << std::endl;
        m_call_num++;
    }

    void postVisit() override {
        m_src << std::endl << "}" << std::endl;
        m_src.flush();
    }

private:
    object_variables_tracker m_platform_object_variables;
    object_variables_tracker m_device_object_variables;
    object_variables_tracker m_context_object_variables;
    object_variables_tracker m_queue_object_variables;
    object_variables_tracker m_program_object_variables;
    object_variables_tracker m_kernel_object_variables;
    object_variables_tracker m_mem_object_variables;
    object_variables_tracker m_event_object_variables;
    uint32_t m_object_creation_num;
    uint32_t m_call_num;
    std::stringstream m_src;
};
