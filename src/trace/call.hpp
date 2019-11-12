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

#include "cl_headers.hpp"

#include <iostream>
#include <cstring>

#include "ocl-api.hpp"

#include "serialize.hpp"
#include "log.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

using namespace oclapi;

//
// Object tracking
//

template<typename T>
class ObjectTracker {

public:
    ObjectTracker() : m_instances(0) {}

    uint64_t add(const T& obj) {
        debug("Object Tracker: tracking %p as #%llu\n", obj, (unsigned long long)m_instances);
        m_objects[obj] = m_instances;
        return m_instances++;
    }

    uint64_t get(const T& obj) const {
        if (m_objects.find(obj) != m_objects.end()) {
            auto val = m_objects.at(obj);
            debug("Object Tracker: getting instance for %p => #%llu\n", obj, (unsigned long long)val);
            return val;
        }
        fatal("Object Tracker: %p is not tracked\n", obj);
        return 0;
    }

    bool is_tracked(const T& obj) const {
        return m_objects.find(obj) != m_objects.end();
    }

private:
    uint64_t m_instances;
    std::unordered_map<T, uint64_t> m_objects;
};

template<typename T>
class ReplayObjectTracker {

public:
    ReplayObjectTracker() {}

    void add(uint64_t id, T obj) {
        debug("Replay object tracker: ID #%llu now alive as %p\n", static_cast<unsigned long long>(id), obj);
        m_objects[id] = obj;
    }

    T get(uint64_t id) const {
        if (m_objects.find(id) != m_objects.end()) {
            auto val = m_objects.at(id);
            debug("Replay object tracker: getting pointer for #%llu => %p\n", static_cast<unsigned long long>(id), val);
            return val;
        }
        fatal("Unknown instance for #%llu\n", static_cast<unsigned long long>(id));
        return 0;
    }

private:
    std::unordered_map<uint64_t, T> m_objects;
};

static ObjectTracker<cl_platform_id> gTracker_platforms;
static ObjectTracker<cl_device_id> gTracker_devices;
static ObjectTracker<cl_context> gTracker_contexts;
static ObjectTracker<cl_command_queue> gTracker_queues;
static ObjectTracker<cl_program> gTracker_programs;
static ObjectTracker<cl_kernel> gTracker_kernels;
static ObjectTracker<cl_mem> gTracker_mems;
static ObjectTracker<cl_event> gTracker_events;

template<typename T> auto& object_capture_tracker() = delete;
template<> inline auto& object_capture_tracker<cl_platform_id>() { return gTracker_platforms; }
template<> inline auto& object_capture_tracker<cl_device_id>() { return gTracker_devices; }
template<> inline auto& object_capture_tracker<const cl_device_id>() { return gTracker_devices; }
template<> inline auto& object_capture_tracker<cl_context>() { return gTracker_contexts; }
template<> inline auto& object_capture_tracker<cl_command_queue>() { return gTracker_queues; }
template<> inline auto& object_capture_tracker<cl_program>() { return gTracker_programs; }
template<> inline auto& object_capture_tracker<cl_kernel>() { return gTracker_kernels; }
template<> inline auto& object_capture_tracker<cl_mem>() { return gTracker_mems; }
template<> inline auto& object_capture_tracker<cl_event>() { return gTracker_events; }

static ReplayObjectTracker<cl_platform_id> gReplayTracker_platforms;

template<typename T> auto& object_replay_tracker() = delete;
template<> inline auto& object_replay_tracker<cl_platform_id>() { return gReplayTracker_platforms; }

//
// Mapped memory tracking
//

struct MemObjectMappingTracker {
    MemObjectMappingTracker() : m_mapping_id(0) {}

    uint64_t add(void* ptr) {
        debug("Map pointer tracker: tracking %p as #%llu\n", ptr, (unsigned long long)m_mapping_id);
        m_pointers[ptr] = m_mapping_id;
        return m_mapping_id++;
    }

    uint64_t get(void* ptr) const {
        if (m_pointers.count(ptr) != 0) {
            auto id  = m_pointers.at(ptr);
            debug("Map pointer tracker: getting id for %p => #%llu\n", ptr, (unsigned long long)id);
            return id;
        }
        fatal("Unknown map pointer %p\n", ptr);
        return 0;
    }

    bool is_tracked(void* ptr) const {
        return m_pointers.count(ptr) != 0;
    }
private:
    uint64_t m_mapping_id;
    std::unordered_map<void*, uint64_t> m_pointers;
};

static MemObjectMappingTracker gMemObjectMappingTracker;


//
// Call parameters
//

enum CallParamType : uint32_t {
    CALL_PARAM_VALUE,
    CALL_PARAM_OPTIONAL_OBJECT_CREATION,
    CALL_PARAM_VALUE_OUT_BY_REF,
    CALL_PARAM_OBJECT_USE,
    CALL_PARAM_PROPERTIES,
    CALL_PARAM_CALLBACK,
    CALL_PARAM_CALLBACK_DATA,
    CALL_PARAM_ARRAY,
    CALL_PARAM_PROGRAM_SOURCE,
    CALL_PARAM_STRING,
    CALL_PARAM_MAP_POINTER_CREATION,
    CALL_PARAM_MAP_POINTER_USE,
};

enum CallParamTemplateType : uint32_t {
    CALL_PARAM_TEMPLATE_TYPE_NONE,
    CALL_PARAM_TEMPLATE_TYPE_VOID,
    CALL_PARAM_TEMPLATE_TYPE_CHAR,
    CALL_PARAM_TEMPLATE_TYPE_INTPTR_T,
    CALL_PARAM_TEMPLATE_TYPE_SIZET,
    CALL_PARAM_TEMPLATE_TYPE_CL_INT,
    CALL_PARAM_TEMPLATE_TYPE_CL_UINT,
    CALL_PARAM_TEMPLATE_TYPE_CL_LONG,
    CALL_PARAM_TEMPLATE_TYPE_CL_ULONG,
    CALL_PARAM_TEMPLATE_TYPE_CL_PLATFORM_ID,
    CALL_PARAM_TEMPLATE_TYPE_CL_DEVICE_ID,
    CALL_PARAM_TEMPLATE_TYPE_CL_CONTEXT,
    CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE,
    CALL_PARAM_TEMPLATE_TYPE_CL_PROGRAM,
    CALL_PARAM_TEMPLATE_TYPE_CL_KERNEL,
    CALL_PARAM_TEMPLATE_TYPE_CL_MEM,
    CALL_PARAM_TEMPLATE_TYPE_CL_EVENT,
    CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_FORMAT,
    CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_DESC,
};

template<typename T> CallParamTemplateType call_param_template_type() = delete;
template<> inline CallParamTemplateType call_param_template_type<void>() { return CALL_PARAM_TEMPLATE_TYPE_VOID; }
template<> inline CallParamTemplateType call_param_template_type<char>() { return CALL_PARAM_TEMPLATE_TYPE_CHAR; }
template<> inline CallParamTemplateType call_param_template_type<cl_int>() { return CALL_PARAM_TEMPLATE_TYPE_CL_INT; }
template<> inline CallParamTemplateType call_param_template_type<cl_uint>() { return CALL_PARAM_TEMPLATE_TYPE_CL_UINT; }
template<> inline CallParamTemplateType call_param_template_type<cl_long>() { return CALL_PARAM_TEMPLATE_TYPE_CL_LONG; }
template<> inline CallParamTemplateType call_param_template_type<cl_ulong>() { return CALL_PARAM_TEMPLATE_TYPE_CL_ULONG; }
template<> inline CallParamTemplateType call_param_template_type<cl_platform_id>() { return CALL_PARAM_TEMPLATE_TYPE_CL_PLATFORM_ID; }
template<> inline CallParamTemplateType call_param_template_type<cl_device_id>() { return CALL_PARAM_TEMPLATE_TYPE_CL_DEVICE_ID; }
template<> inline CallParamTemplateType call_param_template_type<const cl_device_id>() { return CALL_PARAM_TEMPLATE_TYPE_CL_DEVICE_ID; }
template<> inline CallParamTemplateType call_param_template_type<cl_context>() { return CALL_PARAM_TEMPLATE_TYPE_CL_CONTEXT; }
template<> inline CallParamTemplateType call_param_template_type<cl_command_queue>() { return CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE; }
template<> inline CallParamTemplateType call_param_template_type<cl_program>() { return CALL_PARAM_TEMPLATE_TYPE_CL_PROGRAM; }
template<> inline CallParamTemplateType call_param_template_type<cl_kernel>() { return CALL_PARAM_TEMPLATE_TYPE_CL_KERNEL; }
template<> inline CallParamTemplateType call_param_template_type<cl_mem>() { return CALL_PARAM_TEMPLATE_TYPE_CL_MEM; }
template<> inline CallParamTemplateType call_param_template_type<cl_event>() { return CALL_PARAM_TEMPLATE_TYPE_CL_EVENT; }
template<> inline CallParamTemplateType call_param_template_type<cl_image_format>() { return CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_FORMAT; }
template<> inline CallParamTemplateType call_param_template_type<cl_image_desc>() { return CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_DESC; }

static std::unordered_map<CallParamTemplateType, const char*> gTypeEnumToName = {
    { CALL_PARAM_TEMPLATE_TYPE_VOID, "void" },
    { CALL_PARAM_TEMPLATE_TYPE_CHAR, "char" },
    { CALL_PARAM_TEMPLATE_TYPE_SIZET, "size_t" },
    { CALL_PARAM_TEMPLATE_TYPE_INTPTR_T, "intptr_t" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_INT, "cl_int" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_UINT, "cl_uint" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_LONG, "cl_long" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_ULONG, "cl_ulong" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_PLATFORM_ID, "cl_platform_id" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_DEVICE_ID, "cl_device_id" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_CONTEXT, "cl_context" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE, "cl_command_queue" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_PROGRAM, "cl_program" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_KERNEL, "cl_kernel" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_MEM, "cl_mem" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_EVENT, "cl_event" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_FORMAT, "cl_image_format" },
    { CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_DESC, "cl_image_desc" },
};

static const char* call_param_template_type_name(CallParamTemplateType type) {
    return gTypeEnumToName.at(type);
}

static CallParamTemplateType tracked_kernel_argument_object_type(void *obj) {
    // TODO samplers
    if (object_capture_tracker<cl_mem>().is_tracked(static_cast<cl_mem>(obj))) {
        return CALL_PARAM_TEMPLATE_TYPE_CL_MEM;
    } else if (object_capture_tracker<cl_command_queue>().is_tracked(static_cast<cl_command_queue>(obj))) {
        return CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE;
    } else {
        return CALL_PARAM_TEMPLATE_TYPE_NONE;
    }
}

struct CallParam {

    CallParam(CallParamType type, CallParamTemplateType ttype) : m_type(type), m_ttype(ttype) {}

    CallParamType type() const {
        return m_type;
    }

    CallParamTemplateType ttype() const {
        return m_ttype;
    }

    virtual void print(std::ostream& out) const = 0;

    virtual void serialize(std::ostream &os) const = 0;

    virtual size_t output_memory_requirements() const { return 0; }
private:
    CallParamType m_type;
    CallParamTemplateType m_ttype;
};

template<typename T>
struct CallParamValue : public CallParam {

    CallParamValue(std::istream &is) : CallParamValue() {
        m_value = ::deserialize<T>(is);
    }

    CallParamValue(T &value) : CallParamValue() {
        m_value = value;
    }

    T value() const { return m_value; }

    void print(std::ostream& out) const override {
        out << "Value param: " << m_value << std::endl;
    }

    void serialize(std::ostream &os) const override {
        ::serialize(os, CALL_PARAM_VALUE);
        ::serialize(os, call_param_template_type<T>());
        ::serialize(os, m_value);
    }

private:
    CallParamValue() : CallParam(CALL_PARAM_VALUE, call_param_template_type<T>()) {}
    T m_value;
};

template<typename T>
struct CallParamOptionalObjectCreation : public CallParam {

    CallParamOptionalObjectCreation(std::istream &is) : CallParamOptionalObjectCreation() {
        m_create = ::deserialize<bool>(is);
        uint32_t num_ids = ::deserialize<uint32_t>(is);
        for (uint32_t i = 0; i < num_ids; i++) {
            uint64_t id = ::deserialize<uint64_t>(is);
            m_object_ids.push_back(id);
        }
    }

    CallParamOptionalObjectCreation(bool create, std::vector<uint64_t> &&object_ids)
    : CallParamOptionalObjectCreation() {
        m_create = create;
        m_object_ids = std::move(object_ids);
    }

    size_t output_memory_requirements() const { return m_object_ids.size() * sizeof(void*); }

    const std::vector<uint64_t>& object_ids() const { return m_object_ids; }

    bool create() const { return m_create; }

    void print(std::ostream& out) const override {
        out << "Object creation param: num = " << m_object_ids.size() << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_OPTIONAL_OBJECT_CREATION);
        ::serialize(os, call_param_template_type<T>());
        ::serialize(os, m_create);
        uint32_t num_ids = static_cast<uint32_t>(m_object_ids.size());
        ::serialize(os, num_ids);
        for (auto id : m_object_ids) {
            ::serialize(os, id);
        }
    }

private:
    CallParamOptionalObjectCreation() : CallParam(CALL_PARAM_OPTIONAL_OBJECT_CREATION, call_param_template_type<T>()) {}
    bool m_create;
    std::vector<uint64_t> m_object_ids;
};

template<typename T>
struct CallParamValueOutByRef : public CallParam {

    CallParamValueOutByRef(std::istream &is) : CallParamValueOutByRef() {
        m_null_pointer = ::deserialize<bool>(is);
        uint32_t size = ::deserialize<uint32_t>(is);
        for (uint32_t i = 0; i < size; i++) {
            m_memory.push_back(::deserialize<char>(is)); // FIXME this will be terribly slow
        }
    }

    CallParamValueOutByRef(T* pointer, size_t size) : CallParamValueOutByRef() {
        m_null_pointer = pointer == nullptr;
        if (pointer != nullptr) {
            m_memory.resize(size);
            memcpy(m_memory.data(), pointer, size);
        }
    }

    bool null_pointer() const { return m_null_pointer; }

    size_t output_memory_requirements() const { return m_memory.size(); }

    void print(std::ostream& out) const override {
        out << "Value out by ref param: null pointer = " << m_null_pointer << ", size = " << m_memory.size() << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_VALUE_OUT_BY_REF);
        ::serialize(os, call_param_template_type<T>());
        ::serialize(os, m_null_pointer);

        uint32_t size = static_cast<uint32_t>(m_memory.size());
        ::serialize(os, size);
        for (uint32_t i = 0; i < size; i++) {
            ::serialize(os, m_memory[i]); // FIXME performance
        }
    }

private:
    CallParamValueOutByRef() : CallParam(CALL_PARAM_VALUE_OUT_BY_REF, call_param_template_type<T>()) {}
    bool m_null_pointer;
    std::vector<char> m_memory;
};

template<typename T>
struct CallParamObjectUse : public CallParam {

    CallParamObjectUse(std::istream &is) : CallParamObjectUse() {
        m_multiple = ::deserialize<bool>(is);
        auto num_objects = ::deserialize<uint32_t>(is);
        for (uint32_t i = 0; i < num_objects; i++) {
            m_object_ids.push_back(::deserialize<uint64_t>(is));
        }
    }

    CallParamObjectUse(T object) : CallParamObjectUse() {
        m_multiple = false;
        auto id = object_capture_tracker<T>().get(object);
        m_object_ids.push_back(id);
    }

    CallParamObjectUse(uint32_t num_objects, T* objects) : CallParamObjectUse() {
        m_multiple = true;
        for (uint32_t i = 0; i < num_objects; i++) {
            auto id = object_capture_tracker<T>().get(objects[i]);
            m_object_ids.push_back(id);
        }
    }

    const std::vector<uint64_t>& object_ids() const { return m_object_ids; }
    bool multiple() const { return m_multiple; }

    void print(std::ostream& out) const override {
        out << "Object use: multiple " << m_multiple << ", [";
        const char* sep = "";
        for (auto id : m_object_ids) {
            out << sep << id;
            sep = ",";
        }
        out << "]"  << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_OBJECT_USE);
        ::serialize(os, call_param_template_type<T>());
        ::serialize(os, m_multiple);
        auto num_objects = static_cast<uint32_t>(m_object_ids.size());
        ::serialize(os, num_objects);
        for (auto id : m_object_ids) {
            ::serialize(os, id);
        }
    }
private:
    CallParamObjectUse() : CallParam(CALL_PARAM_OBJECT_USE, call_param_template_type<T>()) {}
    bool m_multiple;
    std::vector<uint64_t> m_object_ids;
};

struct CallParamProperties : public CallParam {
    CallParamProperties(std::istream &is) : CallParamProperties(false) {
        m_has_list = ::deserialize<bool>(is);
        auto count = ::deserialize<uint32_t>(is);
        for (uint32_t i = 0; i < count; i++) {
            m_properties.push_back(::deserialize<intptr_t>(is));
        }
    }

    CallParamProperties(std::vector<intptr_t> &&props) : CallParamProperties(true) {
        m_properties = std::move(props);
    }

    CallParamProperties() : CallParamProperties(false) {}

    const std::vector<intptr_t>& properties() const {
        return m_properties;
    }

    bool has_list() const { return m_has_list; }

    void print(std::ostream& out) const override {
        out << "Properties (TODO)" << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_PROPERTIES);
        ::serialize(os, call_param_template_type<intptr_t>());
        ::serialize(os, m_has_list);
        auto count = static_cast<uint32_t>(m_properties.size());
        ::serialize(os, count);
        for (auto prop : m_properties) {
            ::serialize(os, prop);
        }
    }

private:
    CallParamProperties(bool has_list) :
        CallParam(CALL_PARAM_PROPERTIES, call_param_template_type<intptr_t>()) {
        m_has_list = has_list;
    }
    bool m_has_list;
    std::vector<intptr_t> m_properties;
};

struct CallParamCallback : public CallParam {
    CallParamCallback(std::istream &is) : CallParamCallback() {
        m_cbtype = ::deserialize<ocl_callback>(is);
        m_present = ::deserialize<bool>(is);
    }

    CallParamCallback(ocl_callback cbtype, bool present) : CallParamCallback() {
        m_cbtype = cbtype;
        m_present = present;
    }

    bool present() const { return m_present; }
    ocl_callback callback_type() const  {
        return m_cbtype;
    }

    void print(std::ostream& out) const override {
        out << "Callback (TODO)" << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_CALLBACK);
        ::serialize(os, CALL_PARAM_TEMPLATE_TYPE_NONE);
        ::serialize(os, m_cbtype);
        ::serialize(os, m_present);
    }

private:
    CallParamCallback() : CallParam(CALL_PARAM_CALLBACK, CALL_PARAM_TEMPLATE_TYPE_NONE) {}
    ocl_callback m_cbtype;
    bool m_present;
};

struct CallParamCallbackData : public CallParam {
    CallParamCallbackData(std::istream &is) : CallParamCallbackData() {
        m_present = ::deserialize<bool>(is);
    }

    CallParamCallbackData(bool present) : CallParamCallbackData() {
        m_present = present;
    }

    bool present() const { return m_present; }

    void print(std::ostream& out) const override {
        out << "Callback data (TODO)" << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_CALLBACK_DATA);
        ::serialize(os, CALL_PARAM_TEMPLATE_TYPE_NONE);
        ::serialize(os, m_present);
    }

private:
    CallParamCallbackData() : CallParam(CALL_PARAM_CALLBACK_DATA, CALL_PARAM_TEMPLATE_TYPE_NONE) {}
    bool m_present;
};

template<typename T>
struct CallParamArray : public CallParam {

    CallParamArray(std::istream &is) : CallParamArray() {
        m_null_pointer = ::deserialize<bool>(is);
        uint32_t size = ::deserialize<uint32_t>(is);
        for (uint32_t i = 0; i < size; i++) {
            m_elements.push_back(::deserialize<T>(is)); // FIXME this will be terribly slow
        }
    }

    CallParamArray(const T* pointer, size_t size) : CallParamArray() {
        m_null_pointer = pointer == nullptr;
        if (pointer != nullptr) {
            m_elements.resize(size);
            memcpy(m_elements.data(), pointer, size * sizeof(T));
        }
    }

    bool null_pointer() const { return m_null_pointer; }

    const std::vector<T>& values() const { return m_elements; }

    void print(std::ostream& out) const override {
        out << "Array param: null pointer = " << m_null_pointer << ", size = " << m_elements.size() << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_ARRAY);
        ::serialize(os, call_param_template_type<T>());
        ::serialize(os, m_null_pointer);

        uint32_t size = static_cast<uint32_t>(m_elements.size());
        ::serialize(os, size);
        for (uint32_t i = 0; i < size; i++) {
            ::serialize(os, m_elements[i]); // FIXME performance
        }
    }

private:
    CallParamArray() : CallParam(CALL_PARAM_ARRAY, call_param_template_type<T>()) {}
    bool m_null_pointer;
    std::vector<T> m_elements;
};

struct CallParamProgramSource : public CallParam {

    CallParamProgramSource(std::istream &is) : CallParamProgramSource() {
        uint32_t num_sources = ::deserialize<uint32_t>(is);
        for (uint32_t i = 0; i < num_sources; i++) {
            m_sources.push_back(::deserialize<std::string>(is));
        }
    }

    CallParamProgramSource(size_t count, const size_t* lengths, const char** strings) : CallParamProgramSource() {
        for (size_t i = 0; i < count; i++) {
            size_t len = (lengths != nullptr) ? lengths[i] : 0;
            std::string src;
            if (len != 0) {
                src.append(strings[i], len);
            } else {
                src.append(strings[i]);
            }
            m_sources.push_back(src);
        }
    }

    const std::vector<std::string>& sources() const { return m_sources; }

    void print(std::ostream& out) const override {
        out << "Program source paramam: num sources = " << m_sources.size() << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_PROGRAM_SOURCE);
        ::serialize(os, CALL_PARAM_TEMPLATE_TYPE_NONE);

        uint32_t num_sources = static_cast<uint32_t>(m_sources.size());
        ::serialize(os, num_sources);
        for (uint32_t i = 0; i < num_sources; i++) {
            ::serialize(os, m_sources[i]);
        }
    }

private:
    CallParamProgramSource() : CallParam(CALL_PARAM_PROGRAM_SOURCE, CALL_PARAM_TEMPLATE_TYPE_NONE) {}
    std::vector<std::string> m_sources;
};

struct CallParamString : public CallParam {

    CallParamString(std::istream &is) : CallParamString() {
        m_present = ::deserialize<bool>(is);
        m_str = ::deserialize<std::string>(is);
    }

    CallParamString(const char* str) : CallParamString() {
        if (str != nullptr) {
            m_present = true;
            m_str = str;
        } else {
            m_present = false;
        }
    }

    bool present() const { return m_present; }
    const std::string& str() const { return m_str; }

    void print(std::ostream& out) const override {
        out << "String param: " << m_str << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_STRING);
        ::serialize(os, CALL_PARAM_TEMPLATE_TYPE_NONE);
        ::serialize(os, m_present);
        ::serialize(os, m_str);
    }

private:
    CallParamString() : CallParam(CALL_PARAM_STRING, CALL_PARAM_TEMPLATE_TYPE_NONE) {}
    bool m_present;
    std::string m_str;
};

struct CallParamMapPointerCreation : public CallParam {

    CallParamMapPointerCreation(std::istream &is) : CallParamMapPointerCreation() {
        m_id = ::deserialize<uint64_t>(is);
    }

    CallParamMapPointerCreation(uint64_t id)
    : CallParamMapPointerCreation() {
        m_id = id;
    }

    uint64_t id() const { return m_id; }

    void print(std::ostream& out) const override {
        out << "Map creation param" << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_MAP_POINTER_CREATION);
        ::serialize(os, CALL_PARAM_TEMPLATE_TYPE_NONE);
        ::serialize(os, m_id);
    }

private:
    CallParamMapPointerCreation() : CallParam(CALL_PARAM_MAP_POINTER_CREATION, CALL_PARAM_TEMPLATE_TYPE_NONE) {}
    uint64_t m_id;
};

struct CallParamMapPointerUse : public CallParam {

    CallParamMapPointerUse(std::istream &is) : CallParamMapPointerUse() {
        m_id = ::deserialize<uint64_t>(is);
    }

    CallParamMapPointerUse(uint64_t id)
    : CallParamMapPointerUse() {
        m_id = id;
    }

    uint64_t id() const { return m_id; }

    void print(std::ostream& out) const override {
        out << "Map use param" << std::endl;
    }

    void serialize(std::ostream &os) const {
        ::serialize(os, CALL_PARAM_MAP_POINTER_USE);
        ::serialize(os, CALL_PARAM_TEMPLATE_TYPE_NONE);
        ::serialize(os, m_id);
    }

private:
    CallParamMapPointerUse() : CallParam(CALL_PARAM_MAP_POINTER_USE, CALL_PARAM_TEMPLATE_TYPE_NONE) {}
    uint64_t m_id;
};

static CallParam* construct_call_param(std::istream &is) {
    CallParamType ptype = ::deserialize<CallParamType>(is);
    CallParamTemplateType ttype = ::deserialize<CallParamTemplateType>(is);

    switch(ptype) {
    case CALL_PARAM_VALUE:
        switch (ttype) {
        case CALL_PARAM_TEMPLATE_TYPE_INTPTR_T:
            return new CallParamValue<intptr_t>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_INT:
            return new CallParamValue<cl_int>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_UINT:
            return new CallParamValue<cl_uint>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_LONG:
            return new CallParamValue<cl_long>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_ULONG:
            return new CallParamValue<cl_ulong>(is);
        case CALL_PARAM_TEMPLATE_TYPE_NONE:
        case CALL_PARAM_TEMPLATE_TYPE_VOID:
            abort();
        }
        break;
    case CALL_PARAM_OPTIONAL_OBJECT_CREATION:
        switch (ttype) {
        case CALL_PARAM_TEMPLATE_TYPE_CL_PLATFORM_ID:
            return new CallParamOptionalObjectCreation<cl_platform_id>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_DEVICE_ID:
            return new CallParamOptionalObjectCreation<cl_device_id>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_CONTEXT:
            return new CallParamOptionalObjectCreation<cl_context>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE:
            return new CallParamOptionalObjectCreation<cl_command_queue>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_PROGRAM:
            return new CallParamOptionalObjectCreation<cl_program>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_KERNEL:
            return new CallParamOptionalObjectCreation<cl_kernel>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_MEM:
            return new CallParamOptionalObjectCreation<cl_mem>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_EVENT:
            return new CallParamOptionalObjectCreation<cl_event>(is);
        case CALL_PARAM_TEMPLATE_TYPE_NONE:
        case CALL_PARAM_TEMPLATE_TYPE_VOID:
            abort();
        }
        break;
    case CALL_PARAM_VALUE_OUT_BY_REF:
        switch (ttype) {
        case CALL_PARAM_TEMPLATE_TYPE_VOID:
            return new CallParamValueOutByRef<void>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_INT:
            return new CallParamValueOutByRef<cl_int>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_UINT:
            return new CallParamValueOutByRef<cl_uint>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_ULONG:
            return new CallParamValueOutByRef<cl_ulong>(is);
        case CALL_PARAM_TEMPLATE_TYPE_NONE:
            abort();
        }
        break;
    case CALL_PARAM_OBJECT_USE:
        switch (ttype) {
        case CALL_PARAM_TEMPLATE_TYPE_CL_PLATFORM_ID:
            return new CallParamObjectUse<cl_platform_id>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_DEVICE_ID:
            return new CallParamObjectUse<cl_device_id>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_CONTEXT:
            return new CallParamObjectUse<cl_context>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_PROGRAM:
            return new CallParamObjectUse<cl_program>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_KERNEL:
            return new CallParamObjectUse<cl_kernel>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_MEM:
            return new CallParamObjectUse<cl_mem>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_EVENT:
            return new CallParamObjectUse<cl_event>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE:
            return new CallParamObjectUse<cl_command_queue>(is);
        case CALL_PARAM_TEMPLATE_TYPE_NONE:
        case CALL_PARAM_TEMPLATE_TYPE_VOID:
            abort();
        }
        break;
    case CALL_PARAM_PROPERTIES:
        return new CallParamProperties(is);
    case CALL_PARAM_CALLBACK:
        return new CallParamCallback(is);
    case CALL_PARAM_CALLBACK_DATA:
        return new CallParamCallbackData(is);
    case CALL_PARAM_ARRAY:
        switch (ttype) {
        case CALL_PARAM_TEMPLATE_TYPE_CL_ULONG:
            return new CallParamArray<size_t>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CHAR:
            return new CallParamArray<char>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_FORMAT:
            return new CallParamArray<cl_image_format>(is);
        case CALL_PARAM_TEMPLATE_TYPE_CL_IMAGE_DESC:
            return new CallParamArray<cl_image_desc>(is);
        case CALL_PARAM_TEMPLATE_TYPE_NONE:
        case CALL_PARAM_TEMPLATE_TYPE_VOID:
            abort();
        }
        break;
    case CALL_PARAM_PROGRAM_SOURCE:
        return new CallParamProgramSource(is);
    case CALL_PARAM_STRING:
        return new CallParamString(is);
    case CALL_PARAM_MAP_POINTER_CREATION:
        return new CallParamMapPointerCreation(is);
    case CALL_PARAM_MAP_POINTER_USE:
        return new CallParamMapPointerUse(is);
    }

    fatal("Missing type in call param factory: ptype = %d, ttype = %d\n", ptype, ttype);
    return nullptr;
}

struct Call {

    Call(std::istream &is) {
        deserialize(is);
    }

    Call(oclapi::command command) : m_call_id(command) {}

    oclapi::command id() const { return m_call_id; }

    size_t output_memory_requirements() const {
        size_t size = 0;
        for (auto &param : m_params) {
            size += param->output_memory_requirements();
        }
        return size;
    }

    template<typename T>
    void record_value(T value) {
        m_params.push_back(std::make_unique<CallParamValue<T>>(value));
    }

    template<typename T>
    void record_object_use(T object) {
        m_params.push_back(std::make_unique<CallParamObjectUse<T>>(object));
    }

    template<typename T>
    void record_object_use(unsigned count, T* objects) {
        m_params.push_back(std::make_unique<CallParamObjectUse<T>>(count, objects));
    }

    template<typename T>
    void record_optional_object_creation(unsigned count, T* objects) {
        std::vector<uint64_t> ids;
        bool create = (objects != nullptr);
        for (unsigned i = 0; i < count; i++) {
            uint64_t id = object_capture_tracker<T>().add(objects[i]);
            ids.push_back(id);
        }
        m_params.push_back(std::make_unique<CallParamOptionalObjectCreation<T>>(create, std::move(ids)));
    }

    template<typename T>
    void record_value_out_by_reference(T* pointer, size_t size = sizeof(T)) {
        m_params.push_back(std::make_unique<CallParamValueOutByRef<T>>(pointer, size));
    }

    template<typename T>
    void record_null_terminated_property_list(T* properties) {
        std::vector<intptr_t> props;
        if (properties != nullptr) {
            while (*properties != 0) {
                props.push_back(*properties++);
            }
            m_params.push_back(std::make_unique<CallParamProperties>(std::move(props)));
        } else {
            m_params.push_back(std::make_unique<CallParamProperties>());
        }
    }

    template<typename T>
    void record_callback(ocl_callback cbtype, T* ptr) {
        bool present = ptr != nullptr;
        m_params.push_back(std::make_unique<CallParamCallback>(cbtype, present));
    }

    void record_callback_user_data(void* ptr) {
        bool present = ptr != nullptr;
        m_params.push_back(std::make_unique<CallParamCallbackData>(present));
    }

    template<typename T>
    void record_array(size_t size, const T* pointer) {
        m_params.push_back(std::make_unique<CallParamArray<T>>(pointer, size));
    }

    void record_program_source(size_t count, const size_t* lengths, const char** strings) {
        m_params.push_back(std::make_unique<CallParamProgramSource>(count, lengths, strings));
    }

    void record_string(const char* str) {
        m_params.push_back(std::make_unique<CallParamString>(str));
    }

    void record_map_pointer_use(void* ptr) {
        uint64_t id = gMemObjectMappingTracker.get(ptr);
        m_params.push_back(std::make_unique<CallParamMapPointerUse>(id));
    }

    template<typename T>
    void record_return_value(T value) {
        m_return = std::make_unique<CallParamValue<T>>(value);
    }

    template<typename T>
    void record_return_object_creation(T object) {
        std::vector<uint64_t> ids;
        uint64_t id = object_capture_tracker<T>().add(object);
        ids.push_back(id);
        m_return = std::make_unique<CallParamOptionalObjectCreation<T>>(true, std::move(ids));
    }

    void record_return_map_pointer_creation(void* ptr) {
        // FIXME optionally capture memory
        if (gMemObjectMappingTracker.is_tracked(ptr)) {
            fatal("Multiple memobj maps with the same pointer unsupported");
        }
        uint64_t id = gMemObjectMappingTracker.add(ptr);
        m_return = std::make_unique<CallParamMapPointerCreation>(id);
    }

    void print(std::ostream& out) {
        
        out << std::endl
            << "Call: " << oclapi::command_name(m_call_id)
            << "(" << static_cast<uint32_t>(m_call_id) << ")" << std::endl;

        unsigned pnum = 0;
        for (auto &param : m_params) {
            out << "  Param " << pnum++ << ": ";
            param->print(out);
        }

        out << "  Return : ";
        m_return->print(out);
    }

    void serialize(std::ostream &os) {
        // Call ID
        uint32_t call_id = static_cast<uint32_t>(m_call_id);
        ::serialize(os, call_id);

        // Return value
        m_return->serialize(os);

        // Parameters
        uint32_t num_params = static_cast<uint32_t>(m_params.size());
        ::serialize(os, num_params);
        for (auto &param : m_params) {
            param->serialize(os);
        }
    }

    void deserialize(std::istream &is) {
        // Call ID
        m_call_id = static_cast<oclapi::command>(::deserialize<uint32_t>(is));

        // Return Value
        m_return.reset(construct_call_param(is));

        // Parameters
        auto num_params = ::deserialize<uint32_t>(is);
        for (unsigned i = 0; i < num_params; i++) {
            auto p = construct_call_param(is);
            m_params.push_back(std::unique_ptr<CallParam>(p));
        }
    }

    const std::vector<std::unique_ptr<CallParam>>& params() const {
        return m_params;
    }

    const std::unique_ptr<CallParam>& retval() const {
        return m_return;
    }

private:
    oclapi::command m_call_id;
    std::vector<std::unique_ptr<CallParam>> m_params;
    std::unique_ptr<CallParam> m_return;
};

