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
#include <cstddef>
#include <gtest/gtest.h>

static const char* kernel_test_source_1 = R"(
kernel void kernel_test_kernel_1(global float* a, global float* b, global float* c)
{    
    int gid = get_global_id(0);
    c[gid] = a[gid] + b[gid];
}

kernel void kernel_test_kernel_2(global float* x, global float* y, global float* z)
{    
    int gid = get_global_id(0);
    z[gid] = x[gid] * y[gid];
}
)";

TEST_F(WithCommandQueue, clCreateKernelTest) {
    auto kernel1 = CreateKernel(kernel_test_source_1, "kernel_test_kernel_1");
    auto kernel2 = CreateKernel(kernel_test_source_1, "kernel_test_kernel_2");
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clCloneKernelTest) {
    auto kernel = CreateKernel(kernel_test_source_1, "kernel_test_kernel_1");
    auto cloned_kernel = CloneKernel(kernel);
}
#endif

TEST_F(WithCommandQueue, clCreateKernelsInProgramTest) {
    auto program = CreateAndBuildProgram(kernel_test_source_1);

    cl_uint num_kernels;
    cl_int err = clCreateKernelsInProgram(program, 0, nullptr, &num_kernels);
    ASSERT_CL_SUCCESS(err);

// NOTE: Round trip bug due to vector not being indexed properly in generated
// code
#if ENABLE_FAILING
    std::vector<cl_kernel> kernels(num_kernels);
    err =
        clCreateKernelsInProgram(program, num_kernels, kernels.data(), nullptr);
    ASSERT_CL_SUCCESS(err);

    std::string kernel_name_1 =
        GetKernelInfoString(kernels[0], CL_KERNEL_FUNCTION_NAME);
    std::string kernel_name_2 =
        GetKernelInfoString(kernels[1], CL_KERNEL_FUNCTION_NAME);
    printf("names %s \nnames %s\n", kernel_name_1.c_str(),
           kernel_name_2.c_str());

    for (cl_kernel kernel : kernels) {
        clReleaseKernel(kernel);
    }
#endif
}

TEST_F(WithCommandQueue, clSetKernelArgTest) {
    auto kernel = CreateKernel(kernel_test_source_1, "kernel_test_kernel_1");
    auto buffer = CreateBuffer(0, 1024);
    SetKernelArg(kernel, 0, buffer);
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clSetKernelArgSVMPointerTest) {
    const size_t BUFFER_SIZE = 1024;

    auto kernel = CreateKernel(kernel_test_source_1, "kernel_test_kernel_1");

    void* buffer = SVMAlloc(CL_MEM_READ_WRITE, BUFFER_SIZE, 0);
    cl_int err = clSetKernelArgSVMPointer(kernel, 0, buffer);
    ASSERT_CL_SUCCESS(err);

    SVMFree(buffer);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clSetKernelExecInfoTest) {
    auto kernel = CreateKernel(kernel_test_source_1, "kernel_test_kernel_1");
    cl_bool value = CL_FALSE;
    cl_int err =
        clSetKernelExecInfo(kernel, CL_KERNEL_EXEC_INFO_SVM_FINE_GRAIN_SYSTEM,
                            sizeof(cl_bool), &value);
    ASSERT_CL_SUCCESS(err);
}
#endif
