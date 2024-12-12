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

static const char* KHR_test_source = R"(
kernel void KHR_test_kernel(global float* a)
{    
    int gid = get_global_id(0);
    a[gid] = 1.0f;
}
)";

#if ENABLE_UNIMPLEMENTED
TEST(KHRTests, clIcdGetPlatformIDsKHRTest) {
    auto clIcdGetPlatformIDsKHR_f = GET_EXTENSION_FUNC(clIcdGetPlatformIDsKHR);

    cl_uint num_platforms;
    cl_int err = clIcdGetPlatformIDsKHR_f(0, nullptr, &num_platforms);
    ASSERT_CL_SUCCESS(err);

    std::vector<cl_platform_id> platforms(num_platforms);
    err = clIcdGetPlatformIDsKHR_f(num_platforms, platforms.data(),
                                   &num_platforms);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithContext, clTerminateContextKHRTest) {
    if (!IsSupportedExtension("cl_khr_terminate_context")) {
        GTEST_SKIP();
    }
    auto clTerminateContextKHR_f = GET_EXTENSION_FUNC(clTerminateContextKHR);
    cl_int err = clTerminateContextKHR_f(m_context);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clGetKernelSuggestedLocalWorkSizeKHRTest) {
    auto clGetKernelSuggestedLocalWorkSizeKHR_f =
        GET_EXTENSION_FUNC(clGetKernelSuggestedLocalWorkSizeKHR);

    size_t gws = TEST_BUFFER_SIZE / sizeof(cl_float);
    std::vector<size_t> suggested_lws(1);

    auto kernel = CreateKernel(KHR_test_source, "KHR_test_kernel");
    cl_int err = clGetKernelSuggestedLocalWorkSizeKHR_f(
        m_queue, kernel, 1, nullptr, &gws, suggested_lws.data());
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clEnqueueAcquireExternalMemObjectsKHRTest) {
    auto clEnqueueAcquireExternalMemObjectsKHR_f =
        GET_EXTENSION_FUNC(clEnqueueAcquireExternalMemObjectsKHR);
}

TEST_F(WithCommandQueue, clEnqueueReleaseExternalMemObjectsKHRTest) {
    auto clEnqueueReleaseExternalMemObjectsKHR_f =
        GET_EXTENSION_FUNC(clEnqueueReleaseExternalMemObjectsKHR);
}
#endif
