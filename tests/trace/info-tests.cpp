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

static const char* info_test_source = R"(
kernel void info_test_kernel_1(global float* a, global float* b, global float* c)
{
    int gid = get_global_id(0);
    c[gid] = a[gid] + b[gid];
}

kernel void info_test_kernel_2(global float* x, global float* y, global float* z)
{
    int gid = get_global_id(0);
    z[gid] = x[gid] * y[gid];
}
)";

TEST_F(WithCommandQueue, clGetPlatformIDsTest) {
    cl_uint num_platforms;
    cl_uint err = clGetPlatformIDs(0, nullptr, &num_platforms);
    ASSERT_CL_SUCCESS(err);

    std::vector<cl_platform_id> platform_ids(num_platforms);
    err = clGetPlatformIDs(num_platforms, platform_ids.data(), nullptr);
    ASSERT_CL_SUCCESS(err);
    ASSERT_EQ(num_platforms, 1);
}

TEST_F(WithCommandQueue, clGetPlatFormInfoTest) {

    auto info_CL_PLATFORM_NUMERIC_VERSION =
        GetPlatformInfo<cl_version>(gPlatform, CL_PLATFORM_NUMERIC_VERSION);

    auto info_CL_PLATFORM_PROFILE =
        GetPlatformInfoString(gPlatform, CL_PLATFORM_PROFILE);

    auto info_CL_PLATFORM_EXTENSIONS_WITH_VERSION =
        GetPlatformInfoVec<cl_name_version>(
            gPlatform, CL_PLATFORM_EXTENSIONS_WITH_VERSION);
}

TEST_F(WithCommandQueue, clGetDeviceIDsTest) {
    cl_uint num_devices;
    cl_uint err =
        clGetDeviceIDs(gPlatform, CL_DEVICE_TYPE_ALL, 0, nullptr, &num_devices);
    ASSERT_CL_SUCCESS(err);

    std::vector<cl_device_id> device_ids(num_devices);
    err = clGetDeviceIDs(gPlatform, CL_DEVICE_TYPE_ALL, num_devices,
                         device_ids.data(), nullptr);
    ASSERT_CL_SUCCESS(err);
    ASSERT_EQ(num_devices, 1);
}

TEST_F(WithCommandQueue, clGetDeviceInfoTest) {
    auto info_CL_DEVICE_TYPE =
        GetDeviceInfo<cl_device_type>(gDevice, CL_DEVICE_TYPE);

    auto info_CL_DEVICE_MAX_WORK_ITEM_SIZES =
        GetDeviceInfoVec<size_t>(gDevice, CL_DEVICE_MAX_WORK_ITEM_SIZES);
}

TEST_F(WithCommandQueue, clGetContextInfoTest) {
    auto info_CL_CONTEXT_REFERENCE_COUNT =
        GetContextInfo<cl_uint>(CL_CONTEXT_REFERENCE_COUNT);

    auto info_CL_CONTEXT_DEVICES =
        GetContextInfoVec<cl_device_id>(CL_CONTEXT_DEVICES);
}

TEST_F(WithCommandQueue, clGetProgramInfoTest) {
    auto program = CreateAndBuildProgram(info_test_source);

    auto info_CL_PROGRAM_REFERENCE_COUNT =
        GetProgramInfo<cl_uint>(program, CL_PROGRAM_REFERENCE_COUNT);

    auto info_CL_PROGRAM_DEVICES =
        GetProgramInfoVec<cl_device_id>(program, CL_PROGRAM_DEVICES);

    auto info_CL_PROGRAM_SOURCE =
        GetProgramInfoString(program, CL_PROGRAM_SOURCE);
}

TEST_F(WithCommandQueue, clGetProgramBuildInfoTest) {
    auto program = CreateAndBuildProgram(info_test_source);

    auto info_CL_PROGRAM_BUILD_LOG = GetProgramBuildLog(program);
}

TEST_F(WithCommandQueue, clGetKernelInfoTest) {
    auto kernel = CreateKernel(info_test_source, "info_test_kernel_1");

    auto info_CL_KERNEL_FUNCTION_NAME =
        GetKernelInfoString(kernel, CL_KERNEL_FUNCTION_NAME);

    auto info_CL_KERNEL_NUM_ARGS =
        GetKernelInfo<cl_uint>(kernel, CL_KERNEL_NUM_ARGS);

    auto info_CL_KERNEL_ATTRIBUTES =
        GetKernelInfoString(kernel, CL_KERNEL_ATTRIBUTES);
}

TEST_F(WithCommandQueue, clGetKernelArgInfoTest) {
    auto kernel = CreateKernel(info_test_source, "info_test_kernel_1");

    auto info_CL_KERNEL_ARG_ADDRESS_QUALIFIER =
        GetKernelArgInfo<cl_kernel_arg_address_qualifier>(
            kernel, 0, CL_KERNEL_ARG_ADDRESS_QUALIFIER);

    auto info_CL_KERNEL_ARG_TYPE_NAME =
        GetKernelArgInfoString(kernel, 0, CL_KERNEL_ARG_TYPE_NAME);
}

TEST_F(WithCommandQueue, clGetKernelWorkGroupInfoTest) {
    auto kernel = CreateKernel(info_test_source, "info_test_kernel_1");

    auto info_CL_KERNEL_WORK_GROUP_SIZE =
        GetKernelWorkGroupInfo<size_t>(kernel, CL_KERNEL_WORK_GROUP_SIZE);

    auto info_CL_KERNEL_LOCAL_MEM_SIZE =
        GetKernelWorkGroupInfo<cl_ulong>(kernel, CL_KERNEL_LOCAL_MEM_SIZE);

    auto info_CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE =
        GetKernelWorkGroupInfo<size_t>(
            kernel, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE);

    auto info_CL_KERNEL_PRIVATE_MEM_SIZE =
        GetKernelWorkGroupInfo<cl_ulong>(kernel, CL_KERNEL_PRIVATE_MEM_SIZE);
}

TEST_F(WithCommandQueue, clGetKernelSubGroupInfoTest) {
    auto kernel = CreateKernel(info_test_source, "info_test_kernel_1");

    size_t max_subgroups;
    cl_int err;
    err = clGetKernelSubGroupInfo(kernel, device(),
                                  CL_KERNEL_MAX_NUM_SUB_GROUPS, 0, nullptr,
                                  sizeof(size_t), &max_subgroups, nullptr);
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clGetKernelSubGroupInfoKHRTest) {
    auto kernel = CreateKernel(info_test_source, "info_test_kernel_1");

    size_t max_subgroups;
    cl_int err;
    auto clGetKernelSubGroupInfoKHR_f =
        GET_EXTENSION_FUNC(clGetKernelSubGroupInfoKHR);
    err = clGetKernelSubGroupInfoKHR_f(kernel, device(),
                                       CL_KERNEL_MAX_NUM_SUB_GROUPS, 0, nullptr,
                                       sizeof(size_t), &max_subgroups, nullptr);
}
#endif

TEST_F(WithCommandQueue, clGetImageInfoTest) {
    const size_t BUFFER_SIZE = 1024;

    cl_image_format format;
    format.image_channel_data_type = CL_UNORM_INT8;
    format.image_channel_order = CL_RGBA;

    cl_image_desc desc;
    memset(&desc, 0, sizeof(desc));
    desc.image_type = CL_MEM_OBJECT_IMAGE1D;
    desc.image_width = BUFFER_SIZE;

    auto image = CreateImage(CL_MEM_READ_WRITE, &format, &desc, nullptr);
    auto info_CL_IMAGE_FORMAT =
        GetImageInfo<cl_image_format>(image, CL_IMAGE_FORMAT);
}

TEST_F(WithCommandQueue, clGetMemObjectInfoTest) {
    auto buffer = CreateBuffer(CL_MEM_READ_WRITE, 1024);
    cl_mem_object_type info_CL_MEM_TYPE =
        GetMemObjectInfo<cl_mem_object_type>(buffer, CL_MEM_TYPE);
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clGetPipeInfoTest) {
    auto pipe = CreatePipe(0, 4, 16, nullptr);
    cl_uint info_CL_PIPE_PACKET_SIZE =
        GetPipeInfo<cl_uint>(pipe, CL_PIPE_PACKET_SIZE);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clGetSamplerInfoTest) {
    auto sampler = CreateSampler(true, CL_ADDRESS_CLAMP, CL_FILTER_LINEAR);
    cl_uint info_CL_SAMPLER_REFERENCE_COUNT =
        GetSamplerInfo<cl_uint>(sampler, CL_SAMPLER_REFERENCE_COUNT);
}
#endif

TEST_F(WithCommandQueue, clGetCommandQueueInfoTest) {
    auto context = GetCommandQueueInfo<cl_context>(CL_QUEUE_CONTEXT);
}

TEST_F(WithProfiledCommandQueue, clGetEventAndEventProfilingInfoTest) {
    const size_t BUFFER_SIZE = 1024;
    auto kernel = CreateKernel(info_test_source, "info_test_kernel_1");

    auto buffer_1 = CreateBuffer(CL_MEM_READ_ONLY, BUFFER_SIZE);
    auto buffer_2 = CreateBuffer(CL_MEM_READ_ONLY, BUFFER_SIZE);
    auto buffer_3 = CreateBuffer(CL_MEM_WRITE_ONLY, BUFFER_SIZE);

    size_t gws = BUFFER_SIZE / sizeof(cl_int);
    size_t lws = 1;

    SetKernelArg(kernel, 0, buffer_1);
    SetKernelArg(kernel, 1, buffer_2);
    SetKernelArg(kernel, 2, buffer_3);
    cl_event event;
    EnqueueNDRangeKernel(kernel, 1, nullptr, &gws, &lws, 0, nullptr, &event);

    Finish();

    // Tests: clGetEventInfo
    auto status =
        GetEventInfo<cl_int>(event, CL_EVENT_COMMAND_EXECUTION_STATUS);

    // Tests: clGetEventProfilingInfo
    auto time =
        GetEventProfilingInfo<cl_ulong>(event, CL_PROFILING_COMMAND_QUEUED);

    cl_int err = clReleaseEvent(event);
    ASSERT_CL_SUCCESS(err);
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithContext, clGetHostTimerTest) {
    cl_ulong host_timestamp;
    cl_int err = clGetHostTimer(gDevice, &host_timestamp);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithContext, clGetDeviceAndHostTimerTest) {
    cl_ulong device_timestamp;
    cl_ulong host_timestamp;

    cl_int err =
        clGetDeviceAndHostTimer(gDevice, &device_timestamp, &host_timestamp);
    ASSERT_CL_SUCCESS(err);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST(InfoTest, clGetExtensionFunctionAddressTest) {
    void* func = clGetExtensionFunctionAddress("clRetainSemaphoreKHR");
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST(InfoTest, clGetExtensionFunctionAddressForPlatformTest) {
    void* func = clGetExtensionFunctionAddressForPlatform(
        gPlatform, "clRetainSemaphoreKHR");
}
#endif
