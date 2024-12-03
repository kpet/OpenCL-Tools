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

static const char* command_buffer_test_source = R"(
kernel void command_buffer_test_kernel(global float* a)
{
    int gid = get_global_id(0);
    a[gid] = 1.0f;
}
)";

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clCreateCommandBufferKHRTest) {
    std::vector<cl_command_buffer_properties_khr> props;
    props.push_back(CL_COMMAND_BUFFER_FLAGS_KHR);
    props.push_back(CL_COMMAND_BUFFER_MUTABLE_KHR);
    props.push_back(0);

    auto command_buffer = CreateCommandBuffer(1, &m_queue, props.data());
}

TEST_F(WithCommandQueue, clFinalizeCommandBufferKHRTest) {
    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);
    auto clFinalizeCommandBufferKHR_f =
        GET_EXTENSION_FUNC(clFinalizeCommandBufferKHR);
    cl_int err = clFinalizeCommandBufferKHR_f(command_buffer);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clRetainReleaseCommandBufferKHRTest) {
    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);
    auto clRetainCommandBufferKHR_f =
        GET_EXTENSION_FUNC(clRetainCommandBufferKHR);
    cl_int err = clRetainCommandBufferKHR_f(command_buffer);
    ASSERT_CL_SUCCESS(err);

    auto clReleaseCommandBufferKHR_f =
        GET_EXTENSION_FUNC(clReleaseCommandBufferKHR);
    err = clReleaseCommandBufferKHR_f(command_buffer);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clEnqueueCommandBufferKHRTest) {
    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);

    auto clFinalizeCommandBufferKHR_f =
        GET_EXTENSION_FUNC(clFinalizeCommandBufferKHR);
    cl_int err = clFinalizeCommandBufferKHR_f(command_buffer);

    auto clEnqueueCommandBufferKHR_f =
        GET_EXTENSION_FUNC(clEnqueueCommandBufferKHR);
    err = clEnqueueCommandBufferKHR_f(1, &m_queue, command_buffer, 0, nullptr,
                                      nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clCommandBarrierWithWaitListKHRTest) {
    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);

    auto clCommandBarrierWithWaitListKHR_f =
        GET_EXTENSION_FUNC(clCommandBarrierWithWaitListKHR);
    // TODO: Test tracing of sync_point_wait_list
    cl_int err = clCommandBarrierWithWaitListKHR_f(
        command_buffer, nullptr, nullptr, 0, nullptr, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clCommandCopyBufferKHRTest) {
    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);
    auto buffer_1 = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);
    auto buffer_2 = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);

    auto clCommandCopyBufferKHR_f = GET_EXTENSION_FUNC(clCommandCopyBufferKHR);
    cl_int err = clCommandCopyBufferKHR_f(
        command_buffer, nullptr, nullptr, buffer_1, buffer_2, 0, 0,
        TEST_BUFFER_SIZE, 0, nullptr, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clCommandCopyBufferRectKHRTest) {
    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);
    auto buffer_1 = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);
    auto buffer_2 = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);

    std::vector<size_t> origin_1({TEST_BUFFER_SIZE / 2, 0, 0});
    std::vector<size_t> origin_2({0, 0, 0});
    std::vector<size_t> region({TEST_BUFFER_SIZE / 2, 1, 1});

    auto clCommandCopyBufferRectKHR_f =
        GET_EXTENSION_FUNC(clCommandCopyBufferRectKHR);
    cl_int err = clCommandCopyBufferRectKHR_f(
        command_buffer, nullptr, nullptr, buffer_1, buffer_2, origin_1.data(),
        origin_2.data(), region.data(), 0, 0, 0, 0, 0, nullptr, nullptr,
        nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clCommandCopyBufferToImageKHRTest) {
    const size_t IMAGE_SIZE = 1024;
    const size_t bytes_per_pixel = 4;
    cl_image_desc desc = get_image1D_desc(IMAGE_SIZE);

    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);
    auto image =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);
    auto buffer = CreateBuffer(CL_MEM_READ_WRITE, IMAGE_SIZE * bytes_per_pixel);
    std::vector<size_t> origin({0, 0, 0});
    std::vector<size_t> region({IMAGE_SIZE, 1, 1});

    auto clCommandCopyBufferToImageKHR_f =
        GET_EXTENSION_FUNC(clCommandCopyBufferToImageKHR);
    cl_int err = clCommandCopyBufferToImageKHR_f(
        command_buffer, nullptr, nullptr, buffer, image, 0, origin.data(),
        region.data(), 0, nullptr, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clCommandCopyImageKHRTest) {
    const size_t IMAGE_SIZE = 1024;
    cl_image_desc desc = get_image1D_desc(IMAGE_SIZE);

    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);
    auto image1 =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);
    auto image2 =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);

    std::vector<size_t> origin({0, 0, 0});
    std::vector<size_t> region({IMAGE_SIZE, 1, 1});

    auto clCommandCopyImageKHR_f = GET_EXTENSION_FUNC(clCommandCopyImageKHR);
    cl_int err = clCommandCopyImageKHR_f(
        command_buffer, nullptr, nullptr, image1, image2, origin.data(),
        origin.data(), region.data(), 0, nullptr, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clCommandCopyImageToBufferKHRTest) {
    const size_t IMAGE_SIZE = 1024;
    const size_t bytes_per_pixel = 4;
    cl_image_desc desc = get_image1D_desc(IMAGE_SIZE);

    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);
    auto image =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);
    auto buffer = CreateBuffer(CL_MEM_READ_WRITE, IMAGE_SIZE * bytes_per_pixel);

    std::vector<size_t> origin({0, 0, 0});
    std::vector<size_t> region({IMAGE_SIZE, 1, 1});

    auto clCommandCopyImageToBufferKHR_f =
        GET_EXTENSION_FUNC(clCommandCopyImageToBufferKHR);
    cl_int err = clCommandCopyImageToBufferKHR_f(
        command_buffer, nullptr, nullptr, image, buffer, origin.data(),
        region.data(), 0, 0, nullptr, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clCommandFillBufferKHRTest) {
    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);
    auto buffer = CreateBuffer(CL_MEM_READ_WRITE, TEST_BUFFER_SIZE, nullptr);

    std::vector<cl_int> pattern = {1, 2, 3, 4};
    auto clCommandFillBufferKHR_f = GET_EXTENSION_FUNC(clCommandFillBufferKHR);
    cl_int err = clCommandFillBufferKHR_f(
        command_buffer, nullptr, nullptr, buffer, pattern.data(),
        pattern.size() * sizeof(cl_int), 0, TEST_BUFFER_SIZE, 0, nullptr,
        nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clCommandFillImageKHRTest) {
    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);

    const size_t IMAGE_SIZE = 1024;
    cl_image_desc desc = get_image1D_desc(IMAGE_SIZE);
    auto image =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);

    std::vector<uint8_t> fill_color({255, 255, 255, 255});

    // Fill upper half of the image
    std::vector<size_t> origin({IMAGE_SIZE / 2, 0, 0});
    std::vector<size_t> region({IMAGE_SIZE / 2, 1, 1});

    auto clCommandFillImageKHR_f = GET_EXTENSION_FUNC(clCommandFillImageKHR);
    cl_int err = clCommandFillImageKHR_f(
        command_buffer, nullptr, nullptr, image, fill_color.data(),
        origin.data(), region.data(), 0, nullptr, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clCommandNDRangeKernelKHRTest) {
    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);

    auto kernel =
        CreateKernel(command_buffer_test_source, "command_buffer_test_kernel");
    auto buffer = CreateBuffer(CL_MEM_WRITE_ONLY, TEST_BUFFER_SIZE, nullptr);

    size_t gws = TEST_BUFFER_SIZE / sizeof(cl_float);
    size_t lws = 1;

    SetKernelArg(kernel, 0, buffer);

    auto clCommandNDRangeKernelKHR_f =
        GET_EXTENSION_FUNC(clCommandNDRangeKernelKHR);
    cl_int err = clCommandNDRangeKernelKHR_f(command_buffer, nullptr, nullptr,
                                             kernel, 1, nullptr, &gws, &lws, 0,
                                             nullptr, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
}

TEST_F(WithCommandQueue, clGetCommandBufferInfoKHRTest) {
    auto command_buffer = CreateCommandBuffer(1, &m_queue, nullptr);

    GetCommandBufferInfo<cl_uint>(command_buffer,
                                  CL_COMMAND_BUFFER_NUM_QUEUES_KHR);
    GetCommandBufferInfoVec<cl_command_queue>(command_buffer,
                                              CL_COMMAND_BUFFER_QUEUES_KHR);
}

TEST_F(WithCommandQueue, clUpdateMutableCommandsKHRTest) {
    auto clUpdateMutableCommandsKHR_f =
        GET_EXTENSION_FUNC(clUpdateMutableCommandsKHR);
}

TEST_F(WithCommandQueue, clGetMutableCommandInfoKHRTest) {
    if (!IsSupportedExtension("cl_khr_command_buffer_mutable_dispatch")) {
        GTEST_SKIP();
    }

    auto clGetMutableCommandInfoKHR_f =
        GET_EXTENSION_FUNC(clGetMutableCommandInfoKHR);
}
#endif
