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

// Copyright 2018 The clvk authors.
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

#define COMPILER_AVAILABLE 1
#define ENABLE_FAILING 0
#define ENABLE_UNIMPLEMENTED 0
#define ENABLE_UNSUPPORTED 0

#define CL_TARGET_OPENCL_VERSION 300
#define CL_USE_DEPRECATED_OPENCL_1_0_APIS
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#define CL_USE_DEPRECATED_OPENCL_2_1_APIS
#define CL_USE_DEPRECATED_OPENCL_2_2_APIS

#include "CL/cl.h"
#include "CL/cl_ext.h"

// clang-format off
static inline const char* cl_code_to_string(cl_int code) {
#define code_case(X)                                                           \
    case X:                                                                    \
        return #X;
    switch (code) {
    code_case(CL_SUCCESS)
    code_case(CL_DEVICE_NOT_FOUND)
    code_case(CL_DEVICE_NOT_AVAILABLE)
    code_case(CL_COMPILER_NOT_AVAILABLE)
    code_case(CL_MEM_OBJECT_ALLOCATION_FAILURE)
    code_case(CL_OUT_OF_RESOURCES)
    code_case(CL_OUT_OF_HOST_MEMORY)
    code_case(CL_PROFILING_INFO_NOT_AVAILABLE)
    code_case(CL_MEM_COPY_OVERLAP)
    code_case(CL_IMAGE_FORMAT_MISMATCH)
    code_case(CL_IMAGE_FORMAT_NOT_SUPPORTED)
    code_case(CL_BUILD_PROGRAM_FAILURE)
    code_case(CL_MAP_FAILURE)
    code_case(CL_MISALIGNED_SUB_BUFFER_OFFSET)
    code_case(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST)
    code_case(CL_COMPILE_PROGRAM_FAILURE)
    code_case(CL_LINKER_NOT_AVAILABLE)
    code_case(CL_LINK_PROGRAM_FAILURE)
    code_case(CL_DEVICE_PARTITION_FAILED)
    code_case(CL_KERNEL_ARG_INFO_NOT_AVAILABLE)
    code_case(CL_INVALID_VALUE)
    code_case(CL_INVALID_DEVICE_TYPE)
    code_case(CL_INVALID_PLATFORM)
    code_case(CL_INVALID_DEVICE)
    code_case(CL_INVALID_CONTEXT)
    code_case(CL_INVALID_QUEUE_PROPERTIES)
    code_case(CL_INVALID_COMMAND_QUEUE)
    code_case(CL_INVALID_HOST_PTR)
    code_case(CL_INVALID_MEM_OBJECT)
    code_case(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)
    code_case(CL_INVALID_IMAGE_SIZE)
    code_case(CL_INVALID_SAMPLER)
    code_case(CL_INVALID_BINARY)
    code_case(CL_INVALID_BUILD_OPTIONS)
    code_case(CL_INVALID_PROGRAM)
    code_case(CL_INVALID_PROGRAM_EXECUTABLE)
    code_case(CL_INVALID_KERNEL_NAME)
    code_case(CL_INVALID_KERNEL_DEFINITION)
    code_case(CL_INVALID_KERNEL)
    code_case(CL_INVALID_ARG_INDEX)
    code_case(CL_INVALID_ARG_VALUE)
    code_case(CL_INVALID_ARG_SIZE)
    code_case(CL_INVALID_KERNEL_ARGS)
    code_case(CL_INVALID_WORK_DIMENSION)
    code_case(CL_INVALID_WORK_GROUP_SIZE)
    code_case(CL_INVALID_WORK_ITEM_SIZE)
    code_case(CL_INVALID_GLOBAL_OFFSET)
    code_case(CL_INVALID_EVENT_WAIT_LIST)
    code_case(CL_INVALID_EVENT)
    code_case(CL_INVALID_OPERATION)
    code_case(CL_INVALID_GL_OBJECT)
    code_case(CL_INVALID_BUFFER_SIZE)
    code_case(CL_INVALID_MIP_LEVEL)
    code_case(CL_INVALID_GLOBAL_WORK_SIZE)
    code_case(CL_INVALID_PROPERTY)
    code_case(CL_INVALID_IMAGE_DESCRIPTOR)
    code_case(CL_INVALID_COMPILER_OPTIONS)
    code_case(CL_INVALID_LINKER_OPTIONS)
    code_case(CL_INVALID_DEVICE_PARTITION_COUNT)
    }
#undef code_case
    return "Unknown";
}
// clang-format on

#include <chrono>

static inline uint64_t sampleTime() {
    auto now = std::chrono::steady_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(duration)
        .count();
}

extern cl_device_id gDevice;
extern cl_platform_id gPlatform;

#include "gtest/gtest.h"

#define ASSERT_CL_SUCCESS(X) ASSERT_EQ(X, CL_SUCCESS) << cl_code_to_string(X)
#define EXPECT_CL_SUCCESS(X) EXPECT_EQ(X, CL_SUCCESS) << cl_code_to_string(X)

template <typename T> T GetExtensionFunction(std::string func_name) {
    void* func =
        clGetExtensionFunctionAddressForPlatform(gPlatform, func_name.c_str());
    EXPECT_NE(func, nullptr);
    return reinterpret_cast<T>(func);
}

#define GET_EXTENSION_FUNC(FUNC_NAME)                                          \
    GetExtensionFunction<FUNC_NAME##_fn>(#FUNC_NAME)

#define TEST_BUFFER_SIZE 1024

static const cl_image_format TEST_IMAGE_FORMAT = {CL_RGBA, CL_UNORM_INT8};

static inline cl_image_desc get_image1D_desc(size_t image_size) {
    cl_image_desc desc;
    memset(&desc, 0, sizeof(desc));
    desc.image_type = CL_MEM_OBJECT_IMAGE1D;
    desc.image_width = image_size;
    return desc;
}

template <typename T> struct holder {
    holder(T obj) : m_obj(obj) {}
    ~holder() {
        if (m_obj != nullptr) {
            deleter();
        }
    }
    void deleter() { assert(false); }
    operator T() { return m_obj; }
    operator T*() { return &m_obj; }
    T release() {
        T ret = m_obj;
        m_obj = nullptr;
        return ret;
    }

private:
    T m_obj;
};

template <> inline void holder<cl_mem>::deleter() {
    auto err = clReleaseMemObject(m_obj);
    ASSERT_CL_SUCCESS(err);
}

template <> inline void holder<cl_kernel>::deleter() {
    auto err = clReleaseKernel(m_obj);
    ASSERT_CL_SUCCESS(err);
}

template <> inline void holder<cl_sampler>::deleter() {
    auto err = clReleaseSampler(m_obj);
    ASSERT_CL_SUCCESS(err);
}

template <> inline void holder<cl_program>::deleter() {
    auto err = clReleaseProgram(m_obj);
    ASSERT_CL_SUCCESS(err);
}

template <> inline void holder<cl_event>::deleter() {
    auto err = clReleaseEvent(m_obj);
    ASSERT_CL_SUCCESS(err);
}

template <> inline void holder<cl_command_queue>::deleter() {
    auto err = clReleaseCommandQueue(m_obj);
    ASSERT_CL_SUCCESS(err);
}

template <typename T>
T GetPlatformInfo(cl_platform_id platform, cl_platform_info info) {
    T val;
    auto err = clGetPlatformInfo(platform, info, sizeof(val), &val, nullptr);
    EXPECT_CL_SUCCESS(err);
    return val;
}

static std::string GetPlatformInfoString(cl_platform_id platform,
                                         cl_platform_info info) {
    size_t size;
    cl_int err = clGetPlatformInfo(platform, info, 0, nullptr, &size);
    EXPECT_CL_SUCCESS(err);

    std::string val("\0", size);
    err = clGetPlatformInfo(platform, info, size, val.data(), nullptr);
    EXPECT_CL_SUCCESS(err);

    return val;
}

template <typename T>
std::vector<T> GetPlatformInfoVec(cl_platform_id platform,
                                  cl_platform_info info) {
    size_t size;
    cl_int err = clGetPlatformInfo(platform, info, 0, nullptr, &size);
    EXPECT_CL_SUCCESS(err);
    std::vector<T> val(size / sizeof(T));

    err = clGetPlatformInfo(platform, info, size, val.data(), nullptr);
    EXPECT_CL_SUCCESS(err);

    return val;
}

template <typename T>
T GetDeviceInfo(cl_device_id device, cl_device_info info) {
    T val;
    auto err = clGetDeviceInfo(device, info, sizeof(val), &val, nullptr);
    EXPECT_CL_SUCCESS(err);
    return val;
}

static std::string GetDeviceInfoString(cl_device_id device,
                                       cl_device_info info) {
    size_t size;
    cl_int err = clGetDeviceInfo(device, info, 0, nullptr, &size);
    EXPECT_CL_SUCCESS(err);

    std::string val("\0", size);
    err = clGetDeviceInfo(device, info, size, val.data(), nullptr);
    EXPECT_CL_SUCCESS(err);

    return val;
}

template <typename T>
std::vector<T> GetDeviceInfoVec(cl_device_id device, cl_device_info info) {
    size_t size;
    cl_int err = clGetDeviceInfo(device, info, 0, nullptr, &size);
    EXPECT_CL_SUCCESS(err);
    std::vector<T> val(size / sizeof(T));

    err = clGetDeviceInfo(device, info, size, val.data(), nullptr);
    EXPECT_CL_SUCCESS(err);

    return val;
}

static void GetDeviceAndHostTimer(cl_device_id device, cl_ulong* device_ts,
                                  cl_ulong* host_ts) {
    auto err = clGetDeviceAndHostTimer(device, device_ts, host_ts);
    ASSERT_CL_SUCCESS(err);
}

static bool IsSupportedExtension(std::string extension_name) {
    std::string extensions =
        GetPlatformInfoString(gPlatform, CL_PLATFORM_EXTENSIONS);
    return extensions.find(extension_name) != std::string::npos;
}

class WithContext : public ::testing::Test {
protected:
    cl_context m_context;

    cl_platform_id platform() const { return gPlatform; }

    void SetUp() override {
        cl_int err;
        m_context =
            clCreateContext(nullptr, 1, &gDevice, nullptr, nullptr, &err);
        ASSERT_CL_SUCCESS(err);
    }

    void TearDown() override {
        cl_int err = clReleaseContext(m_context);
        ASSERT_CL_SUCCESS(err);
    }

    template <typename T> T GetContextInfo(cl_context_info info) {
        T val;
        auto err =
            clGetContextInfo(m_context, info, sizeof(val), &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    template <typename T>
    std::vector<T> GetContextInfoVec(cl_context_info info) {
        size_t size;
        cl_int err = clGetContextInfo(m_context, info, 0, nullptr, &size);
        EXPECT_CL_SUCCESS(err);
        std::vector<T> val(size / sizeof(T));

        err = clGetContextInfo(m_context, info, size, val.data(), nullptr);
        EXPECT_CL_SUCCESS(err);

        return val;
    }

    holder<cl_program> CreateProgram(const char* source) {
        cl_int err;

        auto program =
            clCreateProgramWithSource(m_context, 1, &source, nullptr, &err);
        EXPECT_CL_SUCCESS(err);

        return program;
    }

    void BuildProgram(cl_program program, const char* options = nullptr) {
        cl_int err =
            clBuildProgram(program, 1, &gDevice, options, nullptr, nullptr);
        EXPECT_CL_SUCCESS(err);

        if (err != CL_SUCCESS) {
            std::string build_log = GetProgramBuildLog(program);
            printf("Build log:\n%s\n", build_log.c_str());
        }
    }

    holder<cl_program> CreateAndBuildProgram(const char* source,
                                             const char* options = nullptr) {
        auto program = CreateProgram(source);

        BuildProgram(program, options);

        return program;
    }

    holder<cl_program>
    CreateProgramWithBinary(const std::vector<uint8_t>& binary) {
        cl_int err;
        const size_t size = binary.size();
        const unsigned char* data = binary.data();
        cl_int binary_status;
        auto program = clCreateProgramWithBinary(m_context, 1, &gDevice, &size,
                                                 &data, &binary_status, &err);
        EXPECT_CL_SUCCESS(err);
        EXPECT_EQ(binary_status, CL_BUILD_SUCCESS);
        return program;
    }

    holder<cl_program>
    CreateAndBuildProgramWithBinary(const std::vector<uint8_t>& binary,
                                    const char* options = nullptr) {
        auto program = CreateProgramWithBinary(binary);

        BuildProgram(program, options);

        return program;
    }

    holder<cl_program> CreateProgramWithIL(std::vector<uint8_t> il) {
        cl_int err;
        auto program =
            clCreateProgramWithIL(m_context, il.data(), il.size(), &err);
        EXPECT_CL_SUCCESS(err);
        return program;
    }

    holder<cl_program>
    CreateAndBuildProgramWithIL(std::vector<uint8_t> il,
                                const char* options = nullptr) {
        auto program = CreateProgramWithIL(il);

        BuildProgram(program, options);

        return program;
    }

    void CompileProgram(cl_program program, const char* options = nullptr) {
        cl_int err = clCompileProgram(program, 1, &gDevice, options, 0, nullptr,
                                      nullptr, nullptr, nullptr);
        EXPECT_CL_SUCCESS(err);

        if (err != CL_SUCCESS) {
            std::string build_log = GetProgramBuildLog(program);
            printf("Build log:\n%s\n", build_log.c_str());
        }
    }

    void CompileProgram(cl_program program, const char* options,
                        cl_uint num_input_headers,
                        const cl_program* input_headers,
                        const char** header_include_names) {
        cl_int err = clCompileProgram(program, 1, &gDevice, options,
                                      num_input_headers, input_headers,
                                      header_include_names, nullptr, nullptr);
        EXPECT_CL_SUCCESS(err);

        if (err != CL_SUCCESS) {
            std::string build_log = GetProgramBuildLog(program);
            printf("Build log:\n%s\n", build_log.c_str());
        }
    }

    holder<cl_program> LinkProgram(cl_uint num_input_programs,
                                   cl_program* input_programs,
                                   const char* options = nullptr) {
        cl_int err;
        auto program =
            clLinkProgram(m_context, 1, &gDevice, options, num_input_programs,
                          input_programs, nullptr, nullptr, &err);
        EXPECT_CL_SUCCESS(err);

        if (err != CL_SUCCESS) {
            std::string build_log = GetProgramBuildLog(program);
            printf("Build log:\n%s\n", build_log.c_str());
        }

        return program;
    }

    cl_program_binary_type GetProgramBinaryType(cl_program program) {
        return GetProgramBuildInfo<cl_program_binary_type>(
            program, CL_PROGRAM_BINARY_TYPE);
    }

    template <typename T>
    T GetProgramInfo(cl_program program, cl_program_info info) {
        T val;
        auto err = clGetProgramInfo(program, info, sizeof(val), &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    static std::string GetProgramInfoString(cl_program program,
                                            cl_program_info info) {
        size_t size;
        cl_int err = clGetProgramInfo(program, info, 0, nullptr, &size);
        EXPECT_CL_SUCCESS(err);

        std::vector<char> val(size);
        err = clGetProgramInfo(program, info, size, val.data(), nullptr);
        EXPECT_CL_SUCCESS(err);
        if (val.begin() == val.end()) {
            return std::string("");
        }
        return std::string(val.begin(), val.end() - 1);
    }

    template <typename T>
    std::vector<T> GetProgramInfoVec(cl_program program, cl_program_info info) {
        size_t size;
        cl_int err = clGetProgramInfo(program, info, 0, nullptr, &size);
        EXPECT_CL_SUCCESS(err);
        std::vector<T> val(size / sizeof(T));

        err = clGetProgramInfo(program, info, size, val.data(), nullptr);
        EXPECT_CL_SUCCESS(err);

        return val;
    }

    template <typename T>
    T GetProgramBuildInfo(cl_program program, cl_program_build_info info) {
        T val;
        auto err = clGetProgramBuildInfo(program, gDevice, info, sizeof(val),
                                         &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    std::string GetProgramBuildInfoString(cl_program program,
                                          cl_program_build_info info) {
        size_t size;
        cl_int err =
            clGetProgramBuildInfo(program, gDevice, info, 0, nullptr, &size);
        EXPECT_CL_SUCCESS(err);

        std::string val("\0", size);
        err = clGetProgramBuildInfo(program, gDevice, info, size, val.data(),
                                    nullptr);
        EXPECT_CL_SUCCESS(err);

        return val;
    }

    std::vector<uint8_t> GetProgramBinary(cl_program program) {
        size_t size_of_binary_sizes;
        clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, 0, nullptr,
                         &size_of_binary_sizes);

        std::vector<size_t> binary_sizes(size_of_binary_sizes / sizeof(size_t));
        clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES, size_of_binary_sizes,
                         binary_sizes.data(), nullptr);

        size_t size;
        clGetProgramInfo(program, CL_PROGRAM_BINARIES, 0, nullptr, &size);
        std::vector<uint8_t> binary(binary_sizes[0]);
        unsigned char* data = binary.data();

        clGetProgramInfo(program, CL_PROGRAM_BINARIES, size, &data, nullptr);
        return binary;
    }

    std::string GetProgramBuildLog(cl_program program) {
        return GetProgramBuildInfoString(program, CL_PROGRAM_BUILD_LOG);
    }

    holder<cl_kernel> CreateKernel(const char* source, const char* name) {
        auto program = CreateAndBuildProgram(source);

        cl_int err;
        auto kernel = clCreateKernel(program, name, &err);
        EXPECT_CL_SUCCESS(err);

        return kernel;
    }

    holder<cl_kernel> CreateKernel(const char* source, const char* options,
                                   const char* name) {
        auto program = CreateAndBuildProgram(source, options);

        cl_int err;
        auto kernel = clCreateKernel(program, name, &err);
        EXPECT_CL_SUCCESS(err);

        return kernel;
    }

    holder<cl_kernel> CreateKernel(cl_program program, const char* name) {
        cl_int err;
        auto kernel = clCreateKernel(program, name, &err);
        EXPECT_CL_SUCCESS(err);
        return kernel;
    }

    holder<cl_kernel> CloneKernel(cl_kernel kernel) {
        cl_int err;
        auto cloned_kernel = clCloneKernel(kernel, &err);
        EXPECT_CL_SUCCESS(err);
        return cloned_kernel;
    }

    template <typename T>
    T GetKernelInfo(cl_kernel kernel, cl_kernel_info info) {
        T val;
        auto err = clGetKernelInfo(kernel, info, sizeof(val), &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    std::string GetKernelInfoString(cl_kernel kernel, cl_kernel_info info) {
        size_t size;
        cl_int err = clGetKernelInfo(kernel, info, 0, nullptr, &size);
        EXPECT_CL_SUCCESS(err);

        std::string val("\0", size);
        err = clGetKernelInfo(kernel, info, size, val.data(), nullptr);
        EXPECT_CL_SUCCESS(err);

        return val;
    }

    template <typename T>
    T GetKernelArgInfo(cl_kernel kernel, cl_uint arg_index,
                       cl_kernel_arg_info info) {
        T val;
        auto err = clGetKernelArgInfo(kernel, arg_index, info, sizeof(val),
                                      &val, nullptr);
        if (CL_KERNEL_ARG_INFO_NOT_AVAILABLE != err) {
            EXPECT_CL_SUCCESS(err);
        }
        return val;
    }

    std::string GetKernelArgInfoString(cl_kernel kernel, cl_uint arg_index,
                                       cl_kernel_arg_info info) {
        size_t size;
        cl_int err =
            clGetKernelArgInfo(kernel, arg_index, info, 0, nullptr, &size);
        if (CL_KERNEL_ARG_INFO_NOT_AVAILABLE == err) {
            return std::string("");
        }
        EXPECT_CL_SUCCESS(err);

        std::string val("\0", size);
        err = clGetKernelArgInfo(kernel, arg_index, info, size, val.data(),
                                 nullptr);
        if (CL_KERNEL_ARG_INFO_NOT_AVAILABLE == err) {
            return std::string("");
        }
        EXPECT_CL_SUCCESS(err);

        return val;
    }

    template <typename T>
    T GetKernelWorkGroupInfo(cl_kernel kernel, cl_kernel_work_group_info info) {
        T val;
        auto err = clGetKernelWorkGroupInfo(kernel, gDevice, info, sizeof(val),
                                            &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    holder<cl_command_queue>
    CreateCommandQueue(cl_device_id device,
                       cl_command_queue_properties properties) {
        cl_int err;
        auto queue = clCreateCommandQueue(m_context, device, properties, &err);
        EXPECT_CL_SUCCESS(err);
        return queue;
    }

    void Finish(cl_command_queue queue) {
        cl_int err = clFinish(queue);
        ASSERT_CL_SUCCESS(err);
    }

    void ReleaseCommandQueue(cl_command_queue queue) {
        cl_int err = clReleaseCommandQueue(queue);
        ASSERT_CL_SUCCESS(err);
    }

    holder<cl_event> CreateUserEvent() {
        cl_int err;
        auto event = clCreateUserEvent(m_context, &err);
        EXPECT_CL_SUCCESS(err);
        return event;
    }

    void SetUserEventStatus(cl_event event, cl_int status) {
        auto err = clSetUserEventStatus(event, status);
        ASSERT_CL_SUCCESS(err);
    }

    holder<cl_mem> CreateBuffer(cl_mem_flags flags, size_t size,
                                void* host_ptr = nullptr) {
        cl_int err;
        auto mem = clCreateBuffer(m_context, flags, size, host_ptr, &err);
        EXPECT_CL_SUCCESS(err);
        return mem;
    }

    holder<cl_mem> CreateSubBuffer(cl_mem buffer, cl_mem_flags flags,
                                   cl_buffer_region* sub_buffer_info) {
        cl_int err;
        auto sub_buffer = clCreateSubBuffer(
            buffer, flags, CL_BUFFER_CREATE_TYPE_REGION, sub_buffer_info, &err);
        EXPECT_CL_SUCCESS(err);
        return sub_buffer;
    }

    holder<cl_mem> CreatePipe(cl_mem_flags flags, cl_uint pipe_packet_size,
                              cl_uint pipe_max_packets,
                              const cl_pipe_properties* properties) {
        cl_int err;
        auto pipe = clCreatePipe(m_context, flags, pipe_packet_size,
                                 pipe_max_packets, properties, &err);
        EXPECT_CL_SUCCESS(err);
        return pipe;
    }

    holder<cl_sampler> CreateSampler(cl_bool normalized_coords,
                                     cl_addressing_mode addressing_mode,
                                     cl_filter_mode filter_mode) {
        cl_int err;
        auto sampler = clCreateSampler(m_context, normalized_coords,
                                       addressing_mode, filter_mode, &err);
        EXPECT_CL_SUCCESS(err);
        return sampler;
    }

    holder<cl_sampler>
    CreateSamplerWithProperties(const cl_sampler_properties* properties) {
        cl_int err;
        auto sampler =
            clCreateSamplerWithProperties(m_context, properties, &err);
        EXPECT_CL_SUCCESS(err);
        return sampler;
    }

    holder<cl_mem> CreateImage(cl_mem_flags flags,
                               const cl_image_format* image_format,
                               const cl_image_desc* image_desc,
                               void* host_ptr = nullptr) {
        cl_int err;
        auto mem = clCreateImage(m_context, flags, image_format, image_desc,
                                 host_ptr, &err);
        EXPECT_CL_SUCCESS(err);
        return mem;
    }

    holder<cl_mem> CreateImageWithProperties(
        const cl_mem_properties* properties, cl_mem_flags flags,
        const cl_image_format* image_format, const cl_image_desc* image_desc,
        void* host_ptr = nullptr) {
        cl_int err;
        auto mem = clCreateImageWithProperties(m_context, properties, flags,
                                               image_format, image_desc,
                                               host_ptr, &err);
        EXPECT_CL_SUCCESS(err);
        return mem;
    }

    template <typename T> T GetImageInfo(cl_mem image, cl_image_info info) {
        T val;
        cl_int err = clGetImageInfo(image, info, sizeof(T), &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    template <typename T>
    T GetSamplerInfo(cl_sampler sampler, cl_sampler_info info) {
        T val;
        cl_int err = clGetSamplerInfo(sampler, info, sizeof(T), &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    template <typename T> T GetPipeInfo(cl_mem mem, cl_pipe_info info) {
        T val;
        auto err = clGetPipeInfo(mem, info, sizeof(val), &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    template <typename T> T GetMemObjectInfo(cl_mem mem, cl_mem_info info) {
        T val;
        auto err = clGetMemObjectInfo(mem, info, sizeof(val), &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    std::vector<cl_image_format>
    GetSupportedImageFormats(cl_mem_object_type image_type,
                             cl_mem_flags flags) {

        cl_uint num_image_formats = 0;
        cl_int err = clGetSupportedImageFormats(m_context, flags, image_type, 0,
                                                nullptr, &num_image_formats);
        EXPECT_CL_SUCCESS(err);

        std::vector<cl_image_format> val(num_image_formats);
        err =
            clGetSupportedImageFormats(m_context, flags, image_type,
                                       num_image_formats, val.data(), nullptr);
        EXPECT_CL_SUCCESS(err);

        return val;
    }

    void SetKernelArg(cl_kernel kernel, cl_uint arg_index, size_t arg_size,
                      const void* arg_value) {
        cl_int err = clSetKernelArg(kernel, arg_index, arg_size, arg_value);
        ASSERT_CL_SUCCESS(err);
    }

    void SetKernelArg(cl_kernel kernel, cl_uint arg_index, cl_mem memobj) {
        SetKernelArg(kernel, arg_index, sizeof(cl_mem), &memobj);
    }

    void SetKernelArg(cl_kernel kernel, cl_uint arg_index, cl_sampler sampler) {
        SetKernelArg(kernel, arg_index, sizeof(cl_sampler), &sampler);
    }

    void SetKernelArg(cl_kernel kernel, cl_uint arg_index, cl_int* val) {
        SetKernelArg(kernel, arg_index, sizeof(*val), val);
    }

    void SetKernelArg(cl_kernel kernel, cl_uint arg_index, cl_uint* val) {
        SetKernelArg(kernel, arg_index, sizeof(*val), val);
    }

    void* SVMAlloc(cl_svm_mem_flags flags, size_t size, cl_uint allignment) {
        void* buffer = clSVMAlloc(m_context, flags, size, allignment);
        EXPECT_NE(nullptr, buffer);

        return buffer;
    }
    void SVMFree(void* svm_pointer) { clSVMFree(m_context, svm_pointer); }
};

class WithCommandQueue : public WithContext {
protected:
    cl_command_queue m_queue;

    cl_device_id device() const { return gDevice; }

    void SetUpQueue(cl_command_queue_properties properties) {
#ifndef COMPILER_AVAILABLE
        GTEST_SKIP();
#endif
        WithContext::SetUp();
        auto queue = CreateCommandQueue(device(), properties);
        m_queue = queue.release();
    }

    void SetUp() override { SetUpQueue(0); }

    void TearDown() override {
#ifdef COMPILER_AVAILABLE
        ReleaseCommandQueue(m_queue);
        WithContext::TearDown();
#endif
    }

    using WithContext::Finish;
    void Finish() { Finish(m_queue); }

    void Flush() {
        cl_int err = clFlush(m_queue);
        ASSERT_CL_SUCCESS(err);
    }

    void WaitForEvents(cl_uint num_events, const cl_event* event_list) {
        auto err = clWaitForEvents(num_events, event_list);
        ASSERT_CL_SUCCESS(err);
    }

    void WaitForEvent(cl_event event) { WaitForEvents(1, &event); }

    template <typename T> T GetEventInfo(cl_event event, cl_event_info info) {
        T val;
        cl_int err = clGetEventInfo(event, info, sizeof(T), &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    template <typename T>
    T GetEventProfilingInfo(cl_event event, cl_profiling_info info) {
        T val;
        cl_int err =
            clGetEventProfilingInfo(event, info, sizeof(T), &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    template <typename T> T GetCommandQueueInfo(cl_command_queue_info info) {
        T val;
        cl_int err =
            clGetCommandQueueInfo(m_queue, info, sizeof(T), &val, nullptr);
        EXPECT_CL_SUCCESS(err);
        return val;
    }

    void EnqueueNDRangeKernel(cl_kernel kernel, cl_uint work_dim,
                              const size_t* global_work_offset,
                              const size_t* global_work_size,
                              const size_t* local_work_size,
                              cl_uint num_events_in_wait_list,
                              const cl_event* event_wait_list,
                              cl_event* event) {
        auto err = clEnqueueNDRangeKernel(
            m_queue, kernel, work_dim, global_work_offset, global_work_size,
            local_work_size, num_events_in_wait_list, event_wait_list, event);
        ASSERT_CL_SUCCESS(err);
    }

    void EnqueueNDRangeKernel(cl_kernel kernel, cl_uint work_dim,
                              const size_t* global_work_offset,
                              const size_t* global_work_size,
                              const size_t* local_work_size) {
        EnqueueNDRangeKernel(kernel, work_dim, global_work_offset,
                             global_work_size, local_work_size, 0, nullptr,
                             nullptr);
    }

    void EnqueueTask(cl_kernel kernel, cl_uint num_events_in_wait_list,
                     const cl_event* event_wait_list, cl_event* event) {
        auto err = clEnqueueTask(m_queue, kernel, num_events_in_wait_list,
                                 event_wait_list, event);
        ASSERT_CL_SUCCESS(err);
    }

    void EnqueueTask(cl_kernel kernel) {
        EnqueueTask(kernel, 0, nullptr, nullptr);
    }

    template <typename T>
    T* EnqueueMapBuffer(cl_mem buffer, cl_bool blocking_map,
                        cl_map_flags map_flags, size_t offset, size_t size,
                        cl_uint num_events_in_wait_list,
                        const cl_event* event_wait_list, cl_event* event) {
        cl_int err;
        auto ptr = clEnqueueMapBuffer(m_queue, buffer, blocking_map, map_flags,
                                      offset, size, num_events_in_wait_list,
                                      event_wait_list, event, &err);
        EXPECT_CL_SUCCESS(err);
        return static_cast<T*>(ptr);
    }

    template <typename T>
    T* EnqueueMapBuffer(cl_mem buffer, cl_bool blocking_map,
                        cl_map_flags map_flags, size_t offset, size_t size) {
        return EnqueueMapBuffer<T>(buffer, blocking_map, map_flags, offset,
                                   size, 0, nullptr, nullptr);
    }

    template <typename T>
    T* EnqueueMapImage(cl_mem image, cl_bool blocking_map,
                       cl_map_flags map_flags, const size_t* origin,
                       const size_t* region, size_t* image_row_pitch,
                       size_t* image_slice_pitch,
                       cl_uint num_events_in_wait_list,
                       const cl_event* event_wait_list, cl_event* event) {
        cl_int err;
        auto ptr = clEnqueueMapImage(m_queue, image, blocking_map, map_flags,
                                     origin, region, image_row_pitch,
                                     image_slice_pitch, num_events_in_wait_list,
                                     event_wait_list, event, &err);
        EXPECT_CL_SUCCESS(err);
        return static_cast<T*>(ptr);
    }

    template <typename T>
    T* EnqueueMapImage(cl_mem image, cl_bool blocking_map,
                       cl_map_flags map_flags, const size_t* origin,
                       const size_t* region, size_t* image_row_pitch,
                       size_t* image_slice_pitch) {
        return EnqueueMapImage<T>(image, blocking_map, map_flags, origin,
                                  region, image_row_pitch, image_slice_pitch, 0,
                                  nullptr, nullptr);
    }

    void EnqueueUnmapMemObject(cl_mem memobj, void* mapped_ptr,
                               cl_uint num_events_in_wait_list,
                               const cl_event* event_wait_list,
                               cl_event* event) {
        auto err = clEnqueueUnmapMemObject(m_queue, memobj, mapped_ptr,
                                           num_events_in_wait_list,
                                           event_wait_list, event);
        ASSERT_CL_SUCCESS(err);
    }

    void EnqueueUnmapMemObject(cl_mem memobj, void* mapped_ptr) {
        EnqueueUnmapMemObject(memobj, mapped_ptr, 0, nullptr, nullptr);
    }

    void EnqueueReadBuffer(cl_mem buffer, cl_bool blocking_read, size_t offset,
                           size_t size, void* ptr,
                           cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list, cl_event* event) {
        auto err = clEnqueueReadBuffer(m_queue, buffer, blocking_read, offset,
                                       size, ptr, num_events_in_wait_list,
                                       event_wait_list, event);
        ASSERT_CL_SUCCESS(err);
    }

    void EnqueueReadBuffer(cl_mem buffer, cl_bool blocking_read, size_t offset,
                           size_t size, void* ptr) {
        EnqueueReadBuffer(buffer, blocking_read, offset, size, ptr, 0, nullptr,
                          nullptr);
    }

    void EnqueueWriteBuffer(cl_mem buffer, cl_bool blocking_write,
                            size_t offset, size_t size, const void* ptr,
                            cl_uint num_events_in_wait_list,
                            const cl_event* event_wait_list, cl_event* event) {
        auto err = clEnqueueWriteBuffer(m_queue, buffer, blocking_write, offset,
                                        size, ptr, num_events_in_wait_list,
                                        event_wait_list, event);
        ASSERT_CL_SUCCESS(err);
    }

    void EnqueueWriteBuffer(cl_mem buffer, cl_bool blocking_write,
                            size_t offset, size_t size, const void* ptr) {
        EnqueueWriteBuffer(buffer, blocking_write, offset, size, ptr, 0,
                           nullptr, nullptr);
    }

    void EnqueueReadImage(cl_mem image, cl_bool blocking_read,
                          const size_t* origin, const size_t* region,
                          size_t row_pitch, size_t slice_pitch, void* ptr,
                          cl_uint num_events_in_wait_list,
                          const cl_event* event_wait_list, cl_event* event) {
        auto err = clEnqueueReadImage(
            m_queue, image, blocking_read, origin, region, row_pitch,
            slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
        ASSERT_CL_SUCCESS(err);
    }

    void EnqueueReadImage(cl_mem image, cl_bool blocking_read,
                          const size_t* origin, const size_t* region,
                          size_t row_pitch, size_t slice_pitch, void* ptr) {
        EnqueueReadImage(image, blocking_read, origin, region, row_pitch,
                         slice_pitch, ptr, 0, nullptr, nullptr);
    }

    void EnqueueWriteImage(cl_mem image, cl_bool blocking_write,
                           const size_t* origin, const size_t* region,
                           size_t input_row_pitch, size_t input_slice_pitch,
                           const void* ptr, cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list, cl_event* event) {
        auto err = clEnqueueWriteImage(
            m_queue, image, blocking_write, origin, region, input_row_pitch,
            input_slice_pitch, ptr, num_events_in_wait_list, event_wait_list,
            event);
        ASSERT_CL_SUCCESS(err);
    }

    void EnqueueWriteImage(cl_mem image, cl_bool blocking_write,
                           const size_t* origin, const size_t* region,
                           size_t input_row_pitch, size_t input_slice_pitch,
                           const void* ptr) {
        EnqueueWriteImage(image, blocking_write, origin, region,
                          input_row_pitch, input_slice_pitch, ptr, 0, nullptr,
                          nullptr);
    }

    void EnqueueFillImage(cl_mem image, const void* fill_color,
                          const size_t* origin, const size_t* region,
                          cl_uint num_events_in_wait_list,
                          const cl_event* event_wait_list, cl_event* event) {
        auto err =
            clEnqueueFillImage(m_queue, image, fill_color, origin, region,
                               num_events_in_wait_list, event_wait_list, event);
        ASSERT_CL_SUCCESS(err);
    }

    void EnqueueFillImage(cl_mem image, const void* fill_color,
                          const size_t* origin, const size_t* region) {
        EnqueueFillImage(image, fill_color, origin, region, 0, nullptr,
                         nullptr);
    }
};

class WithProfiledCommandQueue : public WithCommandQueue {
protected:
    void SetUp() override { SetUpQueue(CL_QUEUE_PROFILING_ENABLE); }
};
