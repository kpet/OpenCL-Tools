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

static std::vector<cl_semaphore_properties_khr> default_props = {
    CL_SEMAPHORE_TYPE_KHR, CL_SEMAPHORE_TYPE_BINARY_KHR, 0};

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clCreateSemaphoreWithPropertiesKHRTest) {
    auto sema = CreateSemaphoreWithProperites(default_props.data());
}

TEST_F(WithCommandQueue, clEnqueueSignalSemaphoresKHRTest) {
    auto sema1 = CreateSemaphoreWithProperites(default_props.data());
    auto sema2 = CreateSemaphoreWithProperites(default_props.data());

    cl_semaphore_khr semaphores[] = {sema1, sema2};

    auto clEnqueueSignalSemaphoresKHR_f =
        GET_EXTENSION_FUNC(clEnqueueSignalSemaphoresKHR);
    cl_int err = clEnqueueSignalSemaphoresKHR_f(m_queue, 2, &semaphores[0],
                                                nullptr, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);

    Finish();
}

TEST_F(WithCommandQueue, clEnqueueWaitSemaphoresKHRTest) {
    auto sema1 = CreateSemaphoreWithProperites(default_props.data());
    auto sema2 = CreateSemaphoreWithProperites(default_props.data());

    cl_semaphore_khr semaphores[] = {sema1, sema2};

    auto clEnqueueSignalSemaphoresKHR_f =
        GET_EXTENSION_FUNC(clEnqueueSignalSemaphoresKHR);
    cl_int err = clEnqueueSignalSemaphoresKHR_f(m_queue, 2, &semaphores[0],
                                                nullptr, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);

    auto clEnqueueWaitSemaphoresKHR_f =
        GET_EXTENSION_FUNC(clEnqueueWaitSemaphoresKHR);
    err = clEnqueueWaitSemaphoresKHR_f(m_queue, 2, &semaphores[0], nullptr, 0,
                                       nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);

    Finish();
}

TEST_F(WithCommandQueue, clRetainReleaseSemaphoreKHRTest) {
    auto sema = CreateSemaphoreWithProperites(default_props.data());
    auto clRetainSemaphoreKHR_f = GET_EXTENSION_FUNC(clRetainSemaphoreKHR);
    auto clReleaseSemaphoreKHR_f = GET_EXTENSION_FUNC(clReleaseSemaphoreKHR);

    cl_int err = clRetainSemaphoreKHR_f(sema);
    ASSERT_CL_SUCCESS(err);

    err = clReleaseSemaphoreKHR_f(sema);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clGetSemaphoreInfoKHR) {
    auto sema = CreateSemaphoreWithProperites(default_props.data());

    GetSemaphoreInfo<cl_context>(sema, CL_SEMAPHORE_CONTEXT_KHR);

    GetSemaphoreInfoVec<cl_external_semaphore_handle_type_khr>(
        sema, CL_SEMAPHORE_EXPORT_HANDLE_TYPES_KHR);
}

TEST_F(WithCommandQueue, clGetSemaphoreHandleForTypeKHRTest) {
    std::vector<cl_semaphore_properties_khr> sema_props;

    sema_props.push_back(CL_SEMAPHORE_TYPE_KHR);
    sema_props.push_back(CL_SEMAPHORE_TYPE_BINARY_KHR);
    sema_props.push_back(CL_SEMAPHORE_EXPORT_HANDLE_TYPES_KHR);
    sema_props.push_back(CL_SEMAPHORE_HANDLE_SYNC_FD_KHR);
    sema_props.push_back(CL_SEMAPHORE_EXPORT_HANDLE_TYPES_LIST_END_KHR);
    sema_props.push_back(0);

    auto sema = CreateSemaphoreWithProperites(sema_props.data());

    auto clEnqueueSignalSemaphoresKHR_f =
        GET_EXTENSION_FUNC(clEnqueueSignalSemaphoresKHR);
    cl_int err = clEnqueueSignalSemaphoresKHR_f(m_queue, 1, sema, nullptr, 0,
                                                nullptr, nullptr);

    auto clGetSemaphoreHandleForTypeKHR_f =
        GET_EXTENSION_FUNC(clGetSemaphoreHandleForTypeKHR);
    int handle;
    err = clGetSemaphoreHandleForTypeKHR_f(sema, gDevice,
                                           CL_SEMAPHORE_HANDLE_SYNC_FD_KHR,
                                           sizeof(int), &handle, nullptr);
    ASSERT_CL_SUCCESS(err);
}
#endif
