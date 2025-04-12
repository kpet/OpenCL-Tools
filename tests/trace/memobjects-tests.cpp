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

TEST_F(WithCommandQueue, clCreateBufferTest) {
    auto buffer = CreateBuffer(0, TEST_BUFFER_SIZE);
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithContext, clCreateBufferWithProperties) {
    cl_int err;
    cl_mem buffer = clCreateBufferWithProperties(
        m_context, nullptr, 0, TEST_BUFFER_SIZE, nullptr, &err);
    ASSERT_CL_SUCCESS(err);

    err = clReleaseMemObject(buffer);
    ASSERT_CL_SUCCESS(err);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clSubBufferTest) {
    auto host_buffer =
        std::vector<cl_float>(TEST_BUFFER_SIZE / sizeof(cl_float));

    auto buffer = CreateBuffer(CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
                               TEST_BUFFER_SIZE, host_buffer.data());

    cl_buffer_region region1;
    region1.origin = 0;
    region1.size = TEST_BUFFER_SIZE / 2;
    cl_buffer_region region2;
    region2.origin = TEST_BUFFER_SIZE / 2;
    region2.size = TEST_BUFFER_SIZE / 2;
    auto sub_buffer_1 = CreateSubBuffer(buffer, 0, &region1);
    auto sub_buffer_2 = CreateSubBuffer(buffer, 0, &region2);

    size_t size_1 = GetMemObjectInfo<size_t>(sub_buffer_1, CL_MEM_SIZE);
    ASSERT_EQ(size_1, TEST_BUFFER_SIZE / 2);

    size_t size_2 = GetMemObjectInfo<size_t>(sub_buffer_2, CL_MEM_SIZE);
    ASSERT_EQ(size_2, TEST_BUFFER_SIZE / 2);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clCreatePipeTest) {
    auto pipe = CreatePipe(CL_MEM_READ_WRITE, 4, 16, nullptr);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueFillBufferTest) {
    auto buffer = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);

    std::vector<cl_int> pattern = {1};
    cl_int err =
        clEnqueueFillBuffer(m_queue, buffer, pattern.data(), sizeof(cl_int), 0,
                            TEST_BUFFER_SIZE, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}
#endif

TEST_F(WithCommandQueue, clEnqueueMapUnmapBufferTest) {
    std::vector<char> host_buffer_1(TEST_BUFFER_SIZE, 'a');
    std::vector<char> host_buffer_2(TEST_BUFFER_SIZE, 'b');

    auto device_buffer_1 = CreateBuffer(CL_MEM_COPY_HOST_PTR, TEST_BUFFER_SIZE,
                                        host_buffer_1.data());
    auto device_buffer_2 = CreateBuffer(CL_MEM_COPY_HOST_PTR, TEST_BUFFER_SIZE,
                                        host_buffer_2.data());

    char* mapped_buffer_1 = EnqueueMapBuffer<char>(
        device_buffer_1, true, CL_MAP_READ | CL_MAP_WRITE, 0, TEST_BUFFER_SIZE);
    char* mapped_buffer_2 = EnqueueMapBuffer<char>(
        device_buffer_2, true, CL_MAP_READ | CL_MAP_WRITE, 0, TEST_BUFFER_SIZE);

    for (size_t i = 0; i < TEST_BUFFER_SIZE; ++i) {
        mapped_buffer_1[i]++;
        mapped_buffer_2[i]++;
    }

    EnqueueUnmapMemObject(device_buffer_1, mapped_buffer_1);
    EnqueueUnmapMemObject(device_buffer_2, mapped_buffer_2);

// NOTE: Fails round trip test
#if ENABLE_FAILING
    std::vector<char> host_buffer_3(TEST_BUFFER_SIZE);
    std::vector<char> host_buffer_4(TEST_BUFFER_SIZE);

    EnqueueReadBuffer(device_buffer_1, true, 0, TEST_BUFFER_SIZE,
                      host_buffer_3.data());
    EnqueueReadBuffer(device_buffer_2, true, 0, TEST_BUFFER_SIZE,
                      host_buffer_4.data());

    for (size_t i = 0; i < TEST_BUFFER_SIZE; ++i) {
        ASSERT_EQ(host_buffer_1[i] + 1, host_buffer_3[i]);
        ASSERT_EQ(host_buffer_2[i] + 1, host_buffer_4[i]);
    }
#endif
}

TEST_F(WithCommandQueue, clEnqueueReadWriteBufferTest) {
    std::vector<char> host_buffer_1(TEST_BUFFER_SIZE, 'a');
    std::vector<char> host_buffer_2(TEST_BUFFER_SIZE);

    auto device_buffer = CreateBuffer(0, TEST_BUFFER_SIZE);

    EnqueueWriteBuffer(device_buffer, true, 0, TEST_BUFFER_SIZE,
                       host_buffer_1.data());
    EnqueueReadBuffer(device_buffer, true, 0, TEST_BUFFER_SIZE,
                      host_buffer_2.data());

    ASSERT_EQ(host_buffer_1, host_buffer_2);
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueReadWriteBufferRectTest) {
    auto device_buffer =
        CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);

    std::vector<size_t> origin({0, 0, 0});
    std::vector<size_t> region({TEST_BUFFER_SIZE, 1, 1});
    std::vector<uint8_t> host_buffer(TEST_BUFFER_SIZE);
    cl_int err = clEnqueueReadBufferRect(
        m_queue, device_buffer, true, origin.data(), origin.data(),
        region.data(), 0, 0, 0, 0, host_buffer.data(), 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);

    err = clEnqueueWriteBufferRect(m_queue, device_buffer, true, origin.data(),
                                   origin.data(), region.data(), 0, 0, 0, 0,
                                   host_buffer.data(), 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clEnqueueCopyBufferTest) {
    auto buffer_1 = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);
    auto buffer_2 = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);

    cl_int err = clEnqueueCopyBuffer(m_queue, buffer_1, buffer_2, 0, 0,
                                     TEST_BUFFER_SIZE, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clEnqueueCopyBufferRectTest) {
    auto buffer_1 = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);
    auto buffer_2 = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);

    std::vector<size_t> origin({0, 0, 0});
    std::vector<size_t> region({TEST_BUFFER_SIZE, 1, 1});
    std::vector<uint8_t> host_buffer(TEST_BUFFER_SIZE);

    cl_int err = clEnqueueCopyBufferRect(
        m_queue, buffer_1, buffer_2, origin.data(), origin.data(),
        region.data(), 0, 0, 0, 0, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clEnqueueMigrateMemObjectsTest) {

    auto buffer_1 = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);
    auto buffer_2 = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);

    std::vector<cl_mem> buffers({buffer_1, buffer_2});
    cl_int err = clEnqueueMigrateMemObjects(m_queue, 2, buffers.data(),
                                            CL_MIGRATE_MEM_OBJECT_HOST, 0,
                                            nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
    Finish();
}
#endif

TEST_F(WithContext, clImportMemoryARMTest) {
    if (!IsSupportedExtension("cl_arm_import_memory")) {
        GTEST_SKIP();
    }

    std::vector<cl_import_properties_arm> props;
    props.push_back(CL_IMPORT_TYPE_ARM);
    props.push_back(CL_IMPORT_TYPE_HOST_ARM);
    props.push_back(0);

    uint8_t* host_memory = new (std::align_val_t(64)) uint8_t[TEST_BUFFER_SIZE];
    cl_int err;
    auto clImportMemoryARM_f = GET_EXTENSION_FUNC(clImportMemoryARM);
    cl_mem imported_memory =
        clImportMemoryARM_f(m_context, CL_MEM_READ_WRITE, nullptr, host_memory,
                            TEST_BUFFER_SIZE, &err);
    ASSERT_CL_SUCCESS(err);

    err = clReleaseMemObject(imported_memory);
    ASSERT_CL_SUCCESS(err);
    delete[] host_memory;
}
