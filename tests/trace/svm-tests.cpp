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

static const int BUFFER_SIZE = 64;

#if ENABLE_UNIMPLEMENTED

TEST_F(WithCommandQueue, clSVMAllocFreeTest) {
    void* buffer = SVMAlloc(CL_MEM_READ_WRITE, BUFFER_SIZE, 0);
    SVMFree(buffer);
}

TEST_F(WithCommandQueue, clEnqueueSVMFreeTest) {
    void* buffer1 = SVMAlloc(CL_MEM_READ_WRITE, BUFFER_SIZE, 0);
    void* buffer2 = SVMAlloc(CL_MEM_READ_WRITE, BUFFER_SIZE, 0);

    std::vector<void*> svm_ptrs({buffer1, buffer2});

    cl_uint err = clEnqueueSVMFree(m_queue, 2, svm_ptrs.data(), nullptr,
                                   nullptr, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clEnqueueSVMMemcpyTest) {
    void* buffer1 = SVMAlloc(CL_MEM_READ_WRITE, BUFFER_SIZE, 0);
    void* buffer2 = SVMAlloc(CL_MEM_READ_WRITE, BUFFER_SIZE, 0);

    cl_uint err = clEnqueueSVMMemcpy(m_queue, true, buffer1, buffer2,
                                     BUFFER_SIZE, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);

    err = clFinish(m_queue);

    SVMFree(buffer1);
    SVMFree(buffer2);
}

TEST_F(WithCommandQueue, clEnqueueSVMMemFillTest) {
    void* buffer = SVMAlloc(CL_MEM_READ_WRITE, BUFFER_SIZE, 0);

    std::vector<cl_int> pattern = {1};
    cl_uint err =
        clEnqueueSVMMemFill(m_queue, buffer, pattern.data(), sizeof(cl_int),
                            BUFFER_SIZE, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);

    err = clFinish(m_queue);
    ASSERT_CL_SUCCESS(err);

    SVMFree(buffer);
}

TEST_F(WithCommandQueue, clEnqueueSVMMapUnmapTest) {
    void* buffer = SVMAlloc(CL_MEM_READ_WRITE, BUFFER_SIZE, 0);
    cl_uint err = clEnqueueSVMMap(m_queue, true, CL_MAP_READ, buffer,
                                  BUFFER_SIZE, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);

    err = clEnqueueSVMUnmap(m_queue, buffer, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);

    err = clFinish(m_queue);
    ASSERT_CL_SUCCESS(err);

    SVMFree(buffer);
}

TEST_F(WithCommandQueue, clEnqueueSVMMigrateMemTest) {
    void* buffer1 = SVMAlloc(CL_MEM_READ_WRITE, BUFFER_SIZE, 0);
    void* buffer2 = SVMAlloc(CL_MEM_READ_WRITE, BUFFER_SIZE, 0);

    std::vector<void*> buffers = {buffer1, buffer2};
    std::vector<size_t> sizes = {BUFFER_SIZE, BUFFER_SIZE};
    cl_int err = clEnqueueSVMMigrateMem(
        m_queue, 2, const_cast<const void**>(buffers.data()), sizes.data(),
        CL_MIGRATE_MEM_OBJECT_HOST, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);

    err = clFinish(m_queue);
    ASSERT_CL_SUCCESS(err);

    SVMFree(buffer1);
    SVMFree(buffer2);
}

#endif