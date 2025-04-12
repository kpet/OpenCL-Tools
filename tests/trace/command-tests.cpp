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

#define CL_USE_DEPRECATED_OPENCL_1_0_APIS

#include "testcl.hpp"
#include <gtest/gtest.h>

#define BUFFER_SIZE 1024

static const char* command_queue_test_source = R"(
kernel void command_queue_test_source_1(){}

kernel void command_queue_test_source_2(global float* a)
{    
    int gid = get_global_id(0);
    a[gid] = 1.0f;
}
)";

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clSetDefaultDeviceCommandQueueTest) {

    cl_queue_properties properties[] = {
        CL_QUEUE_PROPERTIES,
        CL_QUEUE_ON_DEVICE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, 0};

    cl_int err;
    cl_command_queue queue = clCreateCommandQueueWithProperties(
        m_context, gDevice, &properties[0], &err);
    ASSERT_CL_SUCCESS(err);

    err = clSetDefaultDeviceCommandQueue(m_context, gDevice, queue);
    ASSERT_CL_SUCCESS(err);

    err = clReleaseCommandQueue(queue);
    ASSERT_CL_SUCCESS(err);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clSetCommandQueuePropertyTest) {
    cl_command_queue_properties old_props;

    cl_int err = clSetCommandQueueProperty(
        m_queue, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, true, &old_props);
    ASSERT_CL_SUCCESS(err);
}
#endif

#if ENABLE_UNSUPPORTED
TEST_F(WithCommandQueue, clEnqueueNativeKernelTest) {
    // Unsupported by mali cl implementation
}
#endif

TEST_F(WithCommandQueue, clEnqueueNDRangeKernelTest) {
    auto kernel =
        CreateKernel(command_queue_test_source, "command_queue_test_source_2");

    auto buffer = CreateBuffer(CL_MEM_WRITE_ONLY, BUFFER_SIZE, nullptr);

    size_t gws = BUFFER_SIZE / sizeof(cl_float);
    size_t lws = 1;

    SetKernelArg(kernel, 0, buffer);
    EnqueueNDRangeKernel(kernel, 1, nullptr, &gws, &lws);

    Finish();
}

TEST_F(WithCommandQueue, clEnqueueTaskTest) {
    auto kernel =
        CreateKernel(command_queue_test_source, "command_queue_test_source_1");

    EnqueueTask(kernel);
    Finish();
}

TEST_F(WithCommandQueue, clFlushTest) { Flush(); }

TEST_F(WithCommandQueue, clFinishTest) { Finish(); }
