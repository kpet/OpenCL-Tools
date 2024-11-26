// Copyright 2024 The OpenCL-Tools authors.
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

#include "testcl.hpp"
#include <gtest/gtest.h>

static const char* api_object_test_source = R"(
kernel void kernel_1() {}
)";

#if ENABLE_UNSUPPORTED
TEST_F(WithCommandQueue, CreateSubDevicesTest) {
    cl_device_partition_property properties[] = {
        CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN,
        CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE, 0};
    cl_uint num_devices;
    cl_int err =
        clCreateSubDevices(device(), properties, 0, NULL, &num_devices);
    ASSERT_CL_SUCCESS(err);
    std::vector<cl_device_id> out_devices(num_devices);
    err = clCreateSubDevices(device(), properties, num_devices,
                             out_devices.data(), nullptr);
    ASSERT_CL_SUCCESS(err);
}
#endif

TEST(APIObjectTests, DeviceRetainReleaseTest) {
    cl_int err = clRetainDevice(gDevice);
    ASSERT_CL_SUCCESS(err);
    err = clReleaseDevice(gDevice);
    ASSERT_CL_SUCCESS(err);
}

TEST(APIObjectTests, CreateContext) {
    cl_int err;
    cl_context context =
        clCreateContext(nullptr, 1, &gDevice, nullptr, nullptr, &err);
    ASSERT_CL_SUCCESS(err);
    err = clReleaseContext(context);
    ASSERT_CL_SUCCESS(err);
}

TEST(APIObjectTests, CreateContextFromTypeTest_1) {
    cl_int err;
    cl_context context = clCreateContextFromType(
        nullptr, CL_DEVICE_TYPE_DEFAULT, nullptr, nullptr, &err);
    ASSERT_CL_SUCCESS(err);
    err = clReleaseContext(context);
    ASSERT_CL_SUCCESS(err);
}

#if ENABLE_FAILING
struct create_context_from_type_user_data {};

void create_context_from_type_notify_func(const char* errinfo,
                                          const void* private_info, size_t cb,
                                          void* user_data) {}

TEST(APIObjectTests, CreateContextFromTypeTest_2) {
    create_context_from_type_user_data data;
    cl_int err;
    cl_context context = clCreateContextFromType(
        nullptr, CL_DEVICE_TYPE_DEFAULT, create_context_from_type_notify_func,
        &data, &err);
    ASSERT_CL_SUCCESS(err);

    err = clReleaseContext(context);
    ASSERT_CL_SUCCESS(err);
}
#endif

TEST_F(WithContext, ContextRetainReleaseTest) {
    clRetainContext(m_context);
    clReleaseContext(m_context);
}

TEST_F(WithContext, CreateCommandQueueTest) {
    CreateCommandQueue(gDevice, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);
}

// NOTE: fails source gen compilation
#if ENABLE_FAILING
TEST_F(WithContext, CreateCommandQueueWithPropertiesTest) {
    cl_queue_properties properties[] = {
        CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, 0};

    cl_int err;
    cl_command_queue command_queue = clCreateCommandQueueWithProperties(
        m_context, gDevice, &properties[0], &err);
    ASSERT_CL_SUCCESS(err);

    err = clReleaseCommandQueue(command_queue);
    ASSERT_CL_SUCCESS(err);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithContext, CreateCommandQueueWithPropertiesKHRTest) {
    cl_queue_properties properties[] = {
        CL_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, 0};

    cl_int err;
    auto clCreateCommandQueueWithPropertiesKHR_f = GET_EXTENSION_FUNC(clCreateCommandQueueWithPropertiesKHR);
    cl_command_queue command_queue = clCreateCommandQueueWithPropertiesKHR_f(
        m_context, gDevice, &properties[0], &err);
    ASSERT_CL_SUCCESS(err);

    err = clReleaseCommandQueue(command_queue);
    ASSERT_CL_SUCCESS(err);
}
#endif

TEST_F(WithCommandQueue, CommandQueueRetainReleaseTest) {
    clRetainCommandQueue(m_queue);
    clReleaseCommandQueue(m_queue);
}

TEST_F(WithContext, MemObjectRetainReleaseTest) {
    auto buffer = CreateBuffer(CL_MEM_READ_WRITE, 1024);
    clRetainMemObject(buffer);
    clReleaseMemObject(buffer);
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithContext, SamplerRetainReleaseTest) {
    auto sampler = CreateSamplerWithProperties(nullptr);
    clRetainSampler(sampler);
    clReleaseSampler(sampler);
}
#endif

TEST_F(WithContext, ProgramRetainReleaseTest) {
    auto program = CreateProgram(api_object_test_source);
    clRetainProgram(program);
    clReleaseProgram(program);
}

// TODO: test the call back with a semaphore
// NOTE: Failing round trip

struct callback_user_data {};

void CL_CALLBACK program_callback_func(cl_program program, void* user_data) {}

void CL_CALLBACK context_callback_func(cl_context context, void* user_data) {}

void CL_CALLBACK mem_object_callback_func(cl_mem mem_object, void* user_data) {}

#if ENABLE_UNSUPPORTED
TEST_F(WithCommandQueue, SetProgramReleaseCallbackTest) {

    {
        holder<cl_program> program = CreateProgram(api_object_test_source);
        callback_user_data user_data;

        cl_int err = clSetProgramReleaseCallback(
            program, program_callback_func, static_cast<void*>(&user_data));
        ASSERT_CL_SUCCESS(err);
    }
    // Program released
}
#endif

#if ENABLE_UNSUPPORTED
TEST_F(WithCommandQueue, SetContextDestructorCallbackTest) {
    cl_int err;
    cl_context context =
        clCreateContext(nullptr, 1, &gDevice, nullptr, nullptr, &err);
    ASSERT_CL_SUCCESS(err);

    callback_user_data user_data;
    err = clSetContextDestructorCallback(context, context_callback_func,
                                         static_cast<void*>(&user_data));
    ASSERT_CL_SUCCESS(err);

    err = clReleaseContext(context);
    ASSERT_CL_SUCCESS(err);
}
#endif

// NOTE: fails source gen compilation
#if ENABLE_FAILING
TEST_F(WithCommandQueue, SetMemObjectDestructorCallbackTest) {
    {
        const size_t BUFFER_SIZE = 1024;
        holder<cl_mem> buffer = CreateBuffer(CL_MEM_READ_WRITE, BUFFER_SIZE);

        callback_user_data user_data;
        clSetMemObjectDestructorCallback(buffer, mem_object_callback_func,
                                         static_cast<void*>(&user_data));
    }
    // Buffer released
}
#endif

TEST_F(WithCommandQueue, KernelRetainReleaseTest) {
    auto kernel = CreateKernel(api_object_test_source, "kernel_1");
    clRetainKernel(kernel);
    clReleaseKernel(kernel);
}

TEST_F(WithCommandQueue, EventRetainReleaseTest) {
    auto event = CreateUserEvent();
    clRetainEvent(event);
    clReleaseEvent(event);
}