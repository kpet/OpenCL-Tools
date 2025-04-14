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
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

static const char* build_test_source_1 = R"(
kernel void build_test_kernel_1(global float* a, global float* b, global float* c)
{
    int gid = get_global_id(0);
    c[gid] = a[gid] + b[gid];
}

kernel void build_test_kernel_2(global float* x, global float* y, global float* z)
{
    int gid = get_global_id(0);
    z[gid] = x[gid] * y[gid];
}
)";

static const char* build_test_source_2 = R"(
kernel void build_test_kernel_3(global float* a, global float* b, global float* c)
{
    int gid = get_global_id(0);
    c[gid] = a[gid] - b[gid];
}

kernel void build_test_kernel_4(global float* x, global float* y, global float* z)
{
    int gid = get_global_id(0);
    z[gid] = x[gid] / y[gid];
}
)";

TEST_F(WithCommandQueue, clCreateProgramWithSourceTest) {
    auto program = CreateAndBuildProgram(build_test_source_1);

    std::string src = GetProgramInfoString(program, CL_PROGRAM_SOURCE);
    EXPECT_EQ(src, std::string(build_test_source_1));
}

TEST_F(WithCommandQueue, clBuildProgramWithOptionsTest) {
    auto program =
        CreateAndBuildProgram(build_test_source_1, "-cl-opt-disable");

    std::string options =
        GetProgramBuildInfoString(program, CL_PROGRAM_BUILD_OPTIONS);
}

TEST_F(WithCommandQueue, clBuildProgramWithMultipleSourceTest) {
    std::vector<const char*> sources(2);
    std::vector<size_t> sizes(2);
    std::string source_1(build_test_source_1);
    std::string source_2(build_test_source_2);

    sources[0] = source_1.data();
    sizes[0] = strlen(build_test_source_1);
    sources[1] = source_2.data();
    sizes[1] = strlen(build_test_source_2);

    cl_int err;

    cl_program program = clCreateProgramWithSource(
        m_context, sources.size(), sources.data(), sizes.data(), &err);
    ASSERT_CL_SUCCESS(err);

    BuildProgram(program);

    err = clReleaseProgram(program);
    ASSERT_CL_SUCCESS(err);
}

/*  To generate SPIR-V IL run the following on a valid test.cl program
 *  clang -c -target spir -O0 -emit-llvm -o test.bc test.cl
 *  llvm-spirv test.bc -o test.spv
 */
std::vector<uint8_t> read_IL_file(std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        printf("%s\n", std::filesystem::current_path().string().c_str());
        printf("TEST NOT RAN, GENERATE %s\n", filename.c_str());
        return std::vector<uint8_t>();
    }

    file.seekg(0, std::ios::end);
    std::streampos size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> source_IL(size);
    file.read(source_IL.data(), size);
    file.close();

    return std::vector<uint8_t>(source_IL.begin(), source_IL.end());
}

TEST_F(WithCommandQueue, clBuildProgramWithILTest) {
    std::vector<uint8_t> IL = read_IL_file("test.spv");
    if (0 == IL.size()) {
        GTEST_SKIP();
    }
    auto program = CreateAndBuildProgramWithIL(IL);
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clBuildProgramWithILKHRTest) {
    std::vector<uint8_t> IL = read_IL_file("test.spv");
    if (0 == IL.size()) {
        GTEST_SKIP();
    }
    auto clCreateProgramWithILKHR_f =
        GET_EXTENSION_FUNC(clCreateProgramWithILKHR);
    cl_int err;
    auto program =
        clCreateProgramWithILKHR_f(m_context, IL.data(), IL.size(), &err);
    ASSERT_CL_SUCCESS(err);

    BuildProgram(program);

    err = clReleaseProgram(program);
    ASSERT_CL_SUCCESS(err);
}
#endif

TEST_F(WithCommandQueue, clBuildProgramWithBuiltInKernels) {

    std::string kernels =
        GetDeviceInfoString(gDevice, CL_DEVICE_BUILT_IN_KERNELS);
    if (kernels.size() > 1) {
        cl_int err;
        cl_program program = clCreateProgramWithBuiltInKernels(
            m_context, 1, &gDevice, kernels.data(), &err);
        ASSERT_CL_SUCCESS(err);
        clReleaseProgram(program);
    } else {
        GTEST_SKIP();
    }
}

// NOTE: Failing round trip test
#if ENABLE_FAILING
TEST_F(WithCommandQueue, clGetProgramBinaryTest) {
    holder<cl_program> program = CreateAndBuildProgram(build_test_source_1);

    std::vector<uint8_t> binary = GetProgramBinary(program);
}
#endif

#if ENABLE_FAILING
TEST_F(WithCommandQueue, clCreateProgramWithBinaryTest) {
    holder<cl_program> program_1 = CreateAndBuildProgram(build_test_source_1);

    std::vector<uint8_t> binary = GetProgramBinary(program_1);

    auto program_2 = CreateAndBuildProgramWithBinary(binary);
}
#endif

#if ENABLE_FAILING
TEST_F(WithCommandQueue, CompileAndLinkProgramTest) {
    auto program = CreateProgram(build_test_source_1);
    cl_program program_object = (cl_program)program;
    CompileProgram(program, nullptr);
    LinkProgram(1, &program_object);
}
#endif

TEST_F(WithCommandQueue, clUnloadCompilerTest) {
    auto program_1 = CreateAndBuildProgram(build_test_source_1);

    clUnloadCompiler();

    auto program_2 = CreateAndBuildProgram(build_test_source_1);
    auto program_3 = CreateAndBuildProgram(build_test_source_2);

    std::string src_1 = GetProgramInfoString(program_2, CL_PROGRAM_SOURCE);
    std::string src_2 = GetProgramInfoString(program_3, CL_PROGRAM_SOURCE);

    EXPECT_EQ(src_1, std::string(build_test_source_1));
    EXPECT_EQ(src_2, std::string(build_test_source_2));
}

TEST_F(WithCommandQueue, clUnloadPlatformCompilerTest) {
    auto program_1 = CreateAndBuildProgram(build_test_source_1);

    clUnloadPlatformCompiler(gPlatform);

    auto program_2 = CreateAndBuildProgram(build_test_source_1);
    auto program_3 = CreateAndBuildProgram(build_test_source_2);

    std::string src_1 = GetProgramInfoString(program_2, CL_PROGRAM_SOURCE);
    std::string src_2 = GetProgramInfoString(program_3, CL_PROGRAM_SOURCE);

    EXPECT_EQ(src_1, std::string(build_test_source_1));
    EXPECT_EQ(src_2, std::string(build_test_source_2));
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clSetProgramSpecializationConstantTest) {
    std::vector<uint8_t> IL = read_IL_file("spec-const-test-as.spv");
    if (0 == IL.size()) {
        GTEST_SKIP();
    }
    auto program = CreateAndBuildProgramWithIL(IL);

    const cl_uint TEST_CONSTANT = 2;
    cl_int err = clSetProgramSpecializationConstant(program, 6, sizeof(cl_uint),
                                                    &TEST_CONSTANT);
    ASSERT_CL_SUCCESS(err);
}
#endif