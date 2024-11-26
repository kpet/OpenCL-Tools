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

// NOTE: Fails round trip test
#if ENABLE_FAILING
TEST_F(WithCommandQueue, clGetSupportedImageFormatsTest) {
    auto formats =
        GetSupportedImageFormats(CL_MEM_OBJECT_IMAGE1D, CL_MEM_READ_ONLY);
}
#endif

TEST_F(WithCommandQueue, clCreateImageTest) {
    cl_image_desc desc = get_image1D_desc(1024);
    auto image =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, CreateImageWithPropertiesTest) {
    cl_image_desc desc = get_image1D_desc(1024);
    auto image = CreateImageWithProperties(nullptr, CL_MEM_READ_WRITE,
                                           &TEST_IMAGE_FORMAT, &desc, nullptr);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clCreateImage2DTest) {
    const size_t image_width = 1024;
    const size_t image_height = 1024;
    const size_t bytes_per_pixel = 4;
    std::vector<uint8_t> buffer(image_width * image_height * bytes_per_pixel);
    cl_int err;
    cl_mem image =
        clCreateImage2D(m_context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
                        &TEST_IMAGE_FORMAT, image_width, image_height,
                        image_height * bytes_per_pixel, buffer.data(), &err);
    ASSERT_CL_SUCCESS(err);

    err = clReleaseMemObject(image);
    ASSERT_CL_SUCCESS(err);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clCreateImage3DTest) {
    const size_t image_width = 128;
    const size_t image_height = 128;
    const size_t image_depth = 128;

    const size_t bytes_per_pixel = 4;
    std::vector<uint8_t> buffer(image_width * image_height * image_depth *
                                bytes_per_pixel);
    cl_int err;
    cl_mem image = clCreateImage3D(
        m_context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, &TEST_IMAGE_FORMAT,
        image_width, image_height, image_depth, image_height * bytes_per_pixel,
        image_height * image_width * bytes_per_pixel, buffer.data(), &err);
    ASSERT_CL_SUCCESS(err);

    err = clReleaseMemObject(image);
    ASSERT_CL_SUCCESS(err);
}
#endif

TEST_F(WithCommandQueue, clEnqueueImageReadWriteTest) {
    const size_t IMAGE_SIZE = 1024;
    cl_image_desc desc = get_image1D_desc(IMAGE_SIZE);

    auto image =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);

    size_t elem_size = GetImageInfo<size_t>(image, CL_IMAGE_ELEMENT_SIZE);
    size_t BUFFER_SIZE = IMAGE_SIZE * elem_size;

    std::vector<uint8_t> host_buffer_1(BUFFER_SIZE);
    std::vector<uint8_t> host_buffer_2(BUFFER_SIZE);

    size_t origin[3] = {0, 0, 0};
    size_t region[3] = {IMAGE_SIZE, 1, 1};

    EnqueueWriteImage(image, true, &origin[0], &region[0], 0, 0,
                      host_buffer_1.data());
    EnqueueReadImage(image, true, &origin[0], &region[0], 0, 0,
                     host_buffer_2.data());
    Finish();
}

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueFillImageTest) {
    const size_t IMAGE_SIZE = 1024;
    cl_image_desc desc = get_image1D_desc(IMAGE_SIZE);
    auto image =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);

    std::vector<float> fill_color({1.0f, 1.0f, 1.0f, 1.0f});
    std::vector<size_t> origin({0, 0, 0});
    std::vector<size_t> region({IMAGE_SIZE, 1, 1});

    cl_int err =
        clEnqueueFillImage(m_queue, image, fill_color.data(), origin.data(),
                           region.data(), 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
    Finish();
}
#endif

#if ENABLE_UNIMPLEMENTEDL
TEST_F(WithCommandQueue, clEnqueueImageMapUnmapTest) {
    const size_t IMAGE_SIZE = 1024;
    cl_image_desc desc = get_image1D_desc(IMAGE_SIZE);

    auto image_1 =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);
    auto image_2 =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);

    size_t elem_size = GetImageInfo<size_t>(image_1, CL_IMAGE_ELEMENT_SIZE);
    size_t BUFFER_SIZE = IMAGE_SIZE * elem_size;

    std::vector<char> host_buffer_1(BUFFER_SIZE, 'a');
    std::vector<char> host_buffer_2(BUFFER_SIZE, 'b');

    size_t origin[3] = {0, 0, 0};
    size_t region[3] = {IMAGE_SIZE, 1, 1};

    EnqueueWriteImage(image_1, true, &origin[0], &region[0], 0, 0,
                      host_buffer_1.data());
    EnqueueWriteImage(image_2, true, &origin[0], &region[0], 0, 0,
                      host_buffer_2.data());

    size_t image_row_pitch;
    size_t image_slice_pitch;

    char* mapped_image_1 = EnqueueMapImage<char>(
        image_1, true, CL_MAP_READ | CL_MAP_WRITE, &origin[0], &region[0],
        &image_row_pitch, &image_slice_pitch);
    char* mapped_image_2 = EnqueueMapImage<char>(
        image_2, true, CL_MAP_READ | CL_MAP_WRITE, &origin[0], &region[0],
        &image_row_pitch, &image_slice_pitch);

    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        mapped_image_1[i]++;
        mapped_image_2[i]++;
    }

    EnqueueUnmapMemObject(image_1, mapped_image_1);
    EnqueueUnmapMemObject(image_2, mapped_image_2);

    Finish();
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueCopyImageTest) {
    const size_t IMAGE_SIZE = 1024;

    cl_image_desc desc = get_image1D_desc(IMAGE_SIZE);
    auto image1 =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);
    auto image2 =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);

    std::vector<size_t> origin({0, 0, 0});
    std::vector<size_t> region({IMAGE_SIZE, 1, 1});

    cl_int err =
        clEnqueueCopyImage(m_queue, image1, image2, origin.data(),
                           origin.data(), region.data(), 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
    Finish();
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueCopyBufferToImageTest) {
    const size_t IMAGE_SIZE = 1024;
    const size_t bytes_per_pixel = 4;
    cl_image_desc desc = get_image1D_desc(IMAGE_SIZE);
    auto image =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);
    auto buffer = CreateBuffer(CL_MEM_READ_WRITE, IMAGE_SIZE * bytes_per_pixel);
    std::vector<size_t> origin({0, 0, 0});
    std::vector<size_t> region({IMAGE_SIZE, 1, 1});

    cl_int err =
        clEnqueueCopyBufferToImage(m_queue, buffer, image, 0, origin.data(),
                                   region.data(), 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
    Finish();
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clEnqueueCopyImageToBufferTest) {
    const size_t IMAGE_SIZE = 1024;
    const size_t bytes_per_pixel = 4;
    cl_image_desc desc = get_image1D_desc(IMAGE_SIZE);
    auto image =
        CreateImage(CL_MEM_READ_WRITE, &TEST_IMAGE_FORMAT, &desc, nullptr);
    auto buffer = CreateBuffer(CL_MEM_READ_WRITE, IMAGE_SIZE * bytes_per_pixel);
    std::vector<size_t> origin({0, 0, 0});
    std::vector<size_t> region({IMAGE_SIZE, 1, 1});

    cl_int err =
        clEnqueueCopyImageToBuffer(m_queue, image, buffer, origin.data(),
                                   region.data(), 0, 0, nullptr, nullptr);
    ASSERT_CL_SUCCESS(err);
    Finish();
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clCreateSamplerTest) {
    auto sampler = CreateSampler(true, CL_ADDRESS_CLAMP, CL_FILTER_LINEAR);
}
#endif

#if ENABLE_UNIMPLEMENTED
TEST_F(WithCommandQueue, clCreateSamplerWithPropertiesTest) {
    auto sampler = CreateSamplerWithProperties(nullptr);
}
#endif