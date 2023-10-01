// Copyright 2019-2023 The OpenCL-Tools authors.
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

#include <vector>

#include "ocl-api.hpp"

#include "log.hpp"
#include "trace.hpp"

#include <cstdio>
#include <iostream>

Trace trace;

#include "ocltools-loader-gen.hpp"

cl_int clGetPlatformIDs(cl_uint num_entries, cl_platform_id* platforms,
                        cl_uint* num_platforms) {
    auto ret = PFN_clGetPlatformIDs(num_entries, platforms, num_platforms);

    Call call(oclapi::command::GET_PLATFORM_IDS);
    call.record_value(num_entries);
    call.record_optional_object_creation(num_entries, platforms);
    call.record_value_out_by_reference(num_platforms);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetPlatformInfo(cl_platform_id platform, cl_platform_info param_name,
                         size_t param_value_size, void* param_value,
                         size_t* param_value_size_ret) {
    auto ret = PFN_clGetPlatformInfo(platform, param_name, param_value_size,
                                     param_value, param_value_size_ret);

    Call call(oclapi::command::GET_PLATFORM_INFO);
    call.record_object_use(platform);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetDeviceIDs(cl_platform_id platform, cl_device_type device_type,
                      cl_uint num_entries, cl_device_id* devices,
                      cl_uint* num_devices) {
    auto ret = PFN_clGetDeviceIDs(platform, device_type, num_entries, devices,
                                  num_devices);

    Call call(oclapi::command::GET_DEVICE_IDS);
    call.record_object_use(platform);
    call.record_value(device_type);
    call.record_value(num_entries);
    call.record_optional_object_creation(num_entries, devices);
    call.record_value_out_by_reference(num_devices);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetDeviceInfo(cl_device_id device, cl_device_info param_name,
                       size_t param_value_size, void* param_value,
                       size_t* param_value_size_ret) {
    auto ret = PFN_clGetDeviceInfo(device, param_name, param_value_size,
                                   param_value, param_value_size_ret);

    Call call(oclapi::command::GET_DEVICE_INFO);
    call.record_object_use(device);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clRetainDevice(cl_device_id device) {
    auto ret = PFN_clRetainDevice(device);

    Call call(oclapi::command::RETAIN_DEVICE);
    call.record_object_use(device);
    call.record_return_value(ret);
    trace.record(call);

    return ret;
}

cl_int clReleaseDevice(cl_device_id device) {
    auto ret = PFN_clReleaseDevice(device);

    Call call(oclapi::command::RELEASE_DEVICE);
    call.record_object_use(device);
    call.record_return_value(ret);
    trace.record(call);

    return ret;
}

cl_context clCreateContext(const cl_context_properties* properties,
                           cl_uint num_devices, const cl_device_id* devices,
                           void(CL_CALLBACK* pfn_notify)(const char*,
                                                         const void*, size_t,
                                                         void*),
                           void* user_data, cl_int* errcode_ret) {
    auto ret = PFN_clCreateContext(properties, num_devices, devices, pfn_notify,
                                   user_data, errcode_ret);

    Call call(oclapi::command::CREATE_CONTEXT);
    call.record_null_terminated_property_list(properties);
    call.record_value(num_devices);
    call.record_object_use(num_devices, devices);
    call.record_callback(OCL_CALLBACK_CONTEXT_NOTIFICATION, pfn_notify);
    call.record_callback_user_data(user_data);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_context clCreateContextFromType(
    const cl_context_properties* properties, cl_device_type device_type,
    void(CL_CALLBACK* pfn_notify)(const char*, const void*, size_t, void*),
    void* user_data, cl_int* errcode_ret) {
    auto ret = PFN_clCreateContextFromType(properties, device_type, pfn_notify,
                                           user_data, errcode_ret);

    Call call(oclapi::command::CREATE_CONTEXT_FROM_TYPE);
    call.record_null_terminated_property_list(properties);
    call.record_value(device_type);
    call.record_callback(OCL_CALLBACK_CONTEXT_NOTIFICATION, pfn_notify);
    call.record_callback_user_data(user_data);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_int clGetContextInfo(cl_context context, cl_context_info param_name,
                        size_t param_value_size, void* param_value,
                        size_t* param_value_size_ret) {
    auto ret = PFN_clGetContextInfo(context, param_name, param_value_size,
                                    param_value, param_value_size_ret);

    Call call(oclapi::command::GET_CONTEXT_INFO);
    call.record_object_use(context);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clRetainContext(cl_context context) {
    auto ret = PFN_clRetainContext(context);

    Call call(oclapi::command::RETAIN_CONTEXT);
    call.record_object_use(context);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clReleaseContext(cl_context context) {
    auto ret = PFN_clReleaseContext(context);

    Call call(oclapi::command::RELEASE_CONTEXT);
    call.record_object_use(context);
    call.record_return_value(ret);
    trace.record(call);

    return ret;
}

cl_program clCreateProgramWithSource(cl_context context, cl_uint count,
                                     const char** strings,
                                     const size_t* lengths,
                                     cl_int* errcode_ret) {
    auto ret = PFN_clCreateProgramWithSource(context, count, strings, lengths,
                                             errcode_ret);

    Call call(oclapi::command::CREATE_PROGRAM_WITH_SOURCE);

    call.record_object_use(context);
    call.record_value(count);
    call.record_program_source(count, lengths, strings);
    call.record_array(count, lengths);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_program clCreateProgramWithBuiltInKernels(cl_context context,
                                             cl_uint num_devices,
                                             const cl_device_id* device_list,
                                             const char* kernel_names,
                                             cl_int* errcode_ret) {
    auto ret = PFN_clCreateProgramWithBuiltInKernels(
        context, num_devices, device_list, kernel_names, errcode_ret);

    Call call(oclapi::command::CREATE_PROGRAM_WITH_BUILT_IN_KERNELS);

    call.record_object_use(context);
    call.record_value(num_devices);
    call.record_optional_object_creation(num_devices, device_list);
    call.record_string(kernel_names);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_program clCreateProgramWithIL(cl_context context, const void* il,
                                 size_t length, cl_int* errcode_ret) {
    auto ret = PFN_clCreateProgramWithIL(context, il, length, errcode_ret);

    Call call(oclapi::command::CREATE_PROGRAM_WITH_IL);

    call.record_object_use(context);
    call.record_array(length, static_cast<const char*>(il));
    call.record_value(length);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_int clRetainProgram(cl_program program) {
    auto ret = PFN_clRetainProgram(program);

    Call call(oclapi::command::RETAIN_PROGRAM);

    call.record_object_use(program);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clReleaseProgram(cl_program program) {
    auto ret = PFN_clReleaseProgram(program);

    Call call(oclapi::command::RELEASE_PROGRAM);
    call.record_object_use(program);
    call.record_return_value(ret);
    trace.record(call);

    return ret;
}

cl_int clGetProgramInfo(cl_program program, cl_program_info param_name,
                        size_t param_value_size, void* param_value,
                        size_t* param_value_size_ret) {
    auto ret = PFN_clGetProgramInfo(program, param_name, param_value_size,
                                    param_value, param_value_size_ret);

    Call call(oclapi::command::GET_PROGRAM_INFO);

    call.record_object_use(program);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clBuildProgram(cl_program program, cl_uint num_devices,
                      const cl_device_id* device_list, const char* options,
                      void(CL_CALLBACK* pfn_notify)(cl_program, void*),
                      void* user_data) {
    auto ret = PFN_clBuildProgram(program, num_devices, device_list, options,
                                  pfn_notify, user_data);

    Call call(oclapi::command::BUILD_PROGRAM);

    call.record_object_use(program);
    call.record_value(num_devices);
    call.record_object_use(num_devices, device_list);
    call.record_string(options);
    call.record_callback(OCL_CALLBACK_PROGRAM_BUILD, pfn_notify);
    call.record_callback_user_data(user_data);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_program clLinkProgram(cl_context context, cl_uint num_devices,
                         const cl_device_id* device_list, const char* options,
                         cl_uint num_input_programs,
                         const cl_program* input_programs,
                         void(CL_CALLBACK* pfn_notify)(cl_program, void*),
                         void* user_data, cl_int* errcode_ret) {
    auto ret = PFN_clLinkProgram(context, num_devices, device_list, options,
                                 num_input_programs, input_programs, pfn_notify,
                                 user_data, errcode_ret);

    Call call(oclapi::command::LINK_PROGRAM);

    call.record_object_use(context);
    call.record_value(num_devices);
    call.record_object_use(num_devices, device_list);
    call.record_string(options);
    call.record_value(num_input_programs);
    call.record_array(num_input_programs, input_programs);
    call.record_callback(OCL_CALLBACK_PROGRAM_BUILD, pfn_notify);
    call.record_callback_user_data(user_data);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_int clGetProgramBuildInfo(cl_program program, cl_device_id device,
                             cl_program_build_info param_name,
                             size_t param_value_size, void* param_value,
                             size_t* param_value_size_ret) {
    auto ret =
        PFN_clGetProgramBuildInfo(program, device, param_name, param_value_size,
                                  param_value, param_value_size_ret);

    Call call(oclapi::command::GET_PROGRAM_BUILD_INFO);
    call.record_object_use(program);
    call.record_object_use(device);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_kernel clCreateKernel(cl_program program, const char* kernel_name,
                         cl_int* errcode_ret) {
    auto ret = PFN_clCreateKernel(program, kernel_name, errcode_ret);

    Call call(oclapi::command::CREATE_KERNEL);

    call.record_object_use(program);
    call.record_string(kernel_name);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_int clCreateKernelsInProgram(cl_program program, cl_uint num_kernels,
                                cl_kernel* kernels, cl_uint* num_kernels_ret) {
    auto ret = PFN_clCreateKernelsInProgram(program, num_kernels, kernels,
                                            num_kernels_ret);

    Call call(oclapi::command::CREATE_KERNELS_IN_PROGRAM);

    call.record_object_use(program);
    call.record_value(num_kernels);
    call.record_value_out_by_reference(kernels,
                                       num_kernels * sizeof(cl_kernel));
    call.record_value_out_by_reference(num_kernels_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clRetainKernel(cl_kernel kernel) {
    auto ret = PFN_clRetainKernel(kernel);

    Call call(oclapi::command::RETAIN_KERNEL);

    call.record_object_use(kernel);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clReleaseKernel(cl_kernel kernel) {
    auto ret = PFN_clReleaseKernel(kernel);

    Call call(oclapi::command::RELEASE_KERNEL);
    call.record_object_use(kernel);
    call.record_return_value(ret);
    trace.record(call);

    return ret;
}

cl_int clSetKernelArg(cl_kernel kernel, cl_uint arg_index, size_t arg_size,
                      const void* arg_value) {
    auto ret = PFN_clSetKernelArg(kernel, arg_index, arg_size, arg_value);

    Call call(oclapi::command::SET_KERNEL_ARG);

    call.record_object_use(kernel);
    call.record_value(arg_index);
    call.record_value(arg_size);
    if (arg_size == sizeof(void*)) {
        // FIXME broken for pointer-sized values that happen to match the
        // pointer for a tracked object
        // TODO support samplers
        void* value = const_cast<void*>(arg_value);
        void* obj = *reinterpret_cast<void**>(value);
        auto ttype = tracked_kernel_argument_object_type(obj);
        if (ttype == CALL_PARAM_TEMPLATE_TYPE_CL_MEM) {
            call.record_object_use(1, static_cast<cl_mem*>(value));
        } else if (ttype == CALL_PARAM_TEMPLATE_TYPE_CL_COMMANDQUEUE) {
            call.record_object_use(1, static_cast<cl_command_queue*>(value));
        } else {
            call.record_array(arg_size, static_cast<const char*>(arg_value));
        }
    } else {
        call.record_array(arg_size, static_cast<const char*>(arg_value));
    }
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetKernelArgInfo(cl_kernel kernel, cl_uint arg_index,
                          cl_kernel_arg_info param_name,
                          size_t param_value_size, void* param_value,
                          size_t* param_value_size_ret) {
    auto ret =
        PFN_clGetKernelArgInfo(kernel, arg_index, param_name, param_value_size,
                               param_value, param_value_size_ret);

    Call call(oclapi::command::GET_KERNEL_ARG_INFO);

    call.record_object_use(kernel);
    call.record_value(arg_index);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetKernelInfo(cl_kernel kernel, cl_kernel_info param_name,
                       size_t param_value_size, void* param_value,
                       size_t* param_value_size_ret) {
    auto ret = PFN_clGetKernelInfo(kernel, param_name, param_value_size,
                                   param_value, param_value_size_ret);

    Call call(oclapi::command::GET_KERNEL_INFO);

    call.record_object_use(kernel);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetKernelWorkGroupInfo(cl_kernel kernel, cl_device_id device,
                                cl_kernel_work_group_info param_name,
                                size_t param_value_size, void* param_value,
                                size_t* param_value_size_ret) {
    auto ret = PFN_clGetKernelWorkGroupInfo(kernel, device, param_name,
                                            param_value_size, param_value,
                                            param_value_size_ret);

    Call call(oclapi::command::GET_KERNEL_WORK_GROUP_INFO);

    call.record_object_use(kernel);
    call.record_object_use(device);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetKernelSubGroupInfo(cl_kernel kernel, cl_device_id device,
                               cl_kernel_sub_group_info param_name,
                               size_t input_value_size, const void* input_value,
                               size_t param_value_size, void* param_value,
                               size_t* param_value_size_ret) {
    auto ret = PFN_clGetKernelSubGroupInfo(
        kernel, device, param_name, input_value_size, input_value,
        param_value_size, param_value, param_value_size_ret);

    Call call(oclapi::command::GET_KERNEL_SUB_GROUP_INFO);

    call.record_object_use(kernel);
    call.record_object_use(device);
    call.record_value(param_name);
    call.record_value(input_value_size);
    call.record_array(input_value_size, static_cast<const char*>(input_value));
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_mem clCreateBuffer(cl_context context, cl_mem_flags flags, size_t size,
                      void* host_ptr, cl_int* errcode_ret) {
    auto ret = PFN_clCreateBuffer(context, flags, size, host_ptr, errcode_ret);

    Call call(oclapi::command::CREATE_BUFFER);

    call.record_object_use(context);
    call.record_value(flags);
    call.record_value(size);
    call.record_array(
        size, static_cast<char*>(host_ptr)); // TODO dedicated param class?
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_mem clCreateSubBuffer(cl_mem buffer, cl_mem_flags flags,
                         cl_buffer_create_type buffer_create_type,
                         const void* buffer_create_info, cl_int* errcode_ret) {
    auto ret = PFN_clCreateSubBuffer(buffer, flags, buffer_create_type,
                                     buffer_create_info, errcode_ret);

    Call call(oclapi::command::CREATE_SUB_BUFFER);
    call.record_object_use(buffer);
    call.record_value(flags);
    call.record_value(buffer_create_type);
    call.record_value_out_by_reference(const_cast<void*>(buffer_create_info),
                                       sizeof(_cl_buffer_region));
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

namespace {

size_t num_channels(cl_channel_order order) {
    switch (order) {
    case CL_R:
    case CL_Rx:
    case CL_A:
    case CL_INTENSITY:
    case CL_LUMINANCE:
        return 1;
    case CL_RG:
    case CL_RGx:
    case CL_RA:
        return 2;
    case CL_RGB:
    case CL_RGBx:
        return 3;
    case CL_RGBA:
    case CL_ARGB:
    case CL_BGRA:
        return 4;
    default:
        return 0;
    }
}

size_t element_size_per_channel(cl_channel_type channel_type) {
    switch (channel_type) {
    case CL_SNORM_INT8:
    case CL_UNORM_INT8:
    case CL_SIGNED_INT8:
    case CL_UNSIGNED_INT8:
        return 1;
    case CL_SNORM_INT16:
    case CL_UNORM_INT16:
    case CL_SIGNED_INT16:
    case CL_UNSIGNED_INT16:
    case CL_UNORM_SHORT_565:
    case CL_UNORM_SHORT_555:
    case CL_HALF_FLOAT:
        return 2;
    case CL_UNORM_INT_101010:
    case CL_SIGNED_INT32:
    case CL_UNSIGNED_INT32:
    case CL_FLOAT:
        return 4;
    default:
        return 0;
    }
}

size_t element_size(const cl_image_format& format) {
    return element_size_per_channel(format.image_channel_data_type) *
           num_channels(format.image_channel_order);
}

size_t calculate_image_region_size(const cl_image_format& format,
                                   size_t row_pitch, size_t slice_pitch,
                                   const size_t* region) {
    if (row_pitch == 0) {
        row_pitch = region[0] * element_size(format);
    }

    if (slice_pitch == 0) {
        slice_pitch = region[1] * row_pitch;
    }

    return region[2] * slice_pitch;
}

} // namespace

cl_mem clCreateImage(cl_context context, cl_mem_flags flags,
                     const cl_image_format* image_format,
                     const cl_image_desc* image_desc, void* host_ptr,
                     cl_int* errcode_ret) {
    auto ret = PFN_clCreateImage(context, flags, image_format, image_desc,
                                 host_ptr, errcode_ret);

    if (image_desc->mem_object != nullptr) {
        fatal("Image created from a buffer unsupported\n");
    }

    if (host_ptr != nullptr) {
        fatal("Image created with host ptr unsupported\n");
    }

    size_t region[3] = {image_desc->image_width, image_desc->image_height,
                        image_desc->image_depth};
    switch (image_desc->image_type) {
    case CL_MEM_OBJECT_IMAGE1D:
        region[1] = 1;
        region[2] = 1;
        break;
    case CL_MEM_OBJECT_IMAGE2D:
        region[2] = 1;
        break;
    case CL_MEM_OBJECT_IMAGE3D:
        break;
    case CL_MEM_OBJECT_IMAGE1D_ARRAY:
        region[1] = image_desc->image_array_size;
        region[2] = 1;
        break;
    case CL_MEM_OBJECT_IMAGE2D_ARRAY:
        region[2] = image_desc->image_array_size;
        break;
    }
    size_t image_data_size =
        calculate_image_region_size(*image_format, image_desc->image_row_pitch,
                                    image_desc->image_slice_pitch, region);

    Call call(oclapi::command::CREATE_IMAGE);

    call.record_object_use(context);
    call.record_value(flags);
    call.record_array(1, image_format); // TODO dedicated param type?
    call.record_array(1, image_desc);   // FIXME capture memobject use
    call.record_array(image_data_size, static_cast<char*>(host_ptr));
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_mem clCreateImage2D(cl_context context, cl_mem_flags flags,
                       const cl_image_format* image_format, size_t image_width,
                       size_t image_height, size_t image_row_pitch,
                       void* host_ptr, cl_int* errcode_ret) {
    PFN_clCreateImage2D(context, flags, image_format, image_width, image_height,
                        image_row_pitch, host_ptr, errcode_ret);

    cl_image_desc image_desc = {CL_MEM_OBJECT_IMAGE2D,
                                image_width,
                                image_height,
                                1, // image_depth
                                1, // image_array_size
                                image_row_pitch};

    return clCreateImage(context, flags, image_format, &image_desc, host_ptr,
                         errcode_ret);
}

cl_mem clCreateImage3D(cl_context context, cl_mem_flags flags,
                       const cl_image_format* image_format, size_t image_width,
                       size_t image_height, size_t image_depth,
                       size_t image_row_pitch, size_t image_slice_pitch,
                       void* host_ptr, cl_int* errcode_ret) {
    PFN_clCreateImage3D(context, flags, image_format, image_width, image_height,
                        image_depth, image_row_pitch, image_slice_pitch,
                        host_ptr, errcode_ret);

    cl_image_desc image_desc = {CL_MEM_OBJECT_IMAGE3D,
                                image_width,
                                image_height,
                                image_depth,
                                1, // image_array_size
                                image_row_pitch,
                                image_slice_pitch};

    return clCreateImage(context, flags, image_format, &image_desc, host_ptr,
                         errcode_ret);
}

cl_int clGetSupportedImageFormats(cl_context context, cl_mem_flags flags,
                                  cl_mem_object_type image_type,
                                  cl_uint num_entries,
                                  cl_image_format* image_formats,
                                  cl_uint* num_image_formats) {
    auto ret =
        PFN_clGetSupportedImageFormats(context, flags, image_type, num_entries,
                                       image_formats, num_image_formats);

    Call call(oclapi::command::GET_SUPPORTED_IMAGE_FORMATS);

    call.record_object_use(context);
    call.record_value(flags);
    call.record_value(image_type);
    call.record_value(num_entries);
    call.record_array(num_entries, image_formats);
    call.record_value_out_by_reference(num_image_formats);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetImageInfo(cl_mem image, cl_image_info param_name,
                      size_t param_value_size, void* param_value,
                      size_t* param_value_size_ret) {
    auto ret = PFN_clGetImageInfo(image, param_name, param_value_size,
                                  param_value, param_value_size_ret);

    Call call(oclapi::command::GET_IMAGE_INFO);

    call.record_object_use(image);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetMemObjectInfo(cl_mem memobj, cl_mem_info param_name,
                          size_t param_value_size, void* param_value,
                          size_t* param_value_size_ret) {
    auto ret = PFN_clGetMemObjectInfo(memobj, param_name, param_value_size,
                                      param_value, param_value_size_ret);

    Call call(oclapi::command::GET_MEM_OBJECT_INFO);
    call.record_object_use(memobj);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clSetMemObjectDestructorCallback(cl_mem memobj,
                                        void(CL_CALLBACK* pfn_notify)(cl_mem,
                                                                      void*),
                                        void* user_data) {
    auto ret =
        PFN_clSetMemObjectDestructorCallback(memobj, pfn_notify, user_data);

    Call call(oclapi::command::SET_MEM_OBJECT_DESTRUCTOR_CALLBACK);

    call.record_object_use(memobj);
    call.record_callback(OCL_CALLBACK_CONTEXT_NOTIFICATION, pfn_notify);
    call.record_callback_user_data(user_data);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clRetainMemObject(cl_mem memobj) {
    auto ret = PFN_clRetainMemObject(memobj);

    Call call(oclapi::command::RETAIN_MEM_OBJECT);

    call.record_object_use(memobj);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clReleaseMemObject(cl_mem mem) {
    auto ret = PFN_clReleaseMemObject(mem);

    Call call(oclapi::command::RELEASE_MEM_OBJECT);
    call.record_object_use(mem);
    call.record_return_value(ret);
    trace.record(call);

    return ret;
}

cl_command_queue clCreateCommandQueue(cl_context context, cl_device_id device,
                                      cl_command_queue_properties properties,
                                      cl_int* errcode_ret) {
    auto ret =
        PFN_clCreateCommandQueue(context, device, properties, errcode_ret);

    Call call(oclapi::command::CREATE_COMMAND_QUEUE);
    call.record_object_use(context);
    call.record_object_use(device);
    call.record_value(properties);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_command_queue
clCreateCommandQueueWithProperties(cl_context context, cl_device_id device,
                                   const cl_queue_properties* properties,
                                   cl_int* errcode_ret) {
    auto ret = PFN_clCreateCommandQueueWithProperties(context, device,
                                                      properties, errcode_ret);

    Call call(oclapi::command::CREATE_COMMAND_QUEUE_WITH_PROPERTIES);

    call.record_object_use(context);
    call.record_object_use(device);
    call.record_null_terminated_property_list(properties);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_int clGetCommandQueueInfo(cl_command_queue command_queue,
                             cl_command_queue_info param_name,
                             size_t param_value_size, void* param_value,
                             size_t* param_value_size_ret) {
    auto ret =
        PFN_clGetCommandQueueInfo(command_queue, param_name, param_value_size,
                                  param_value, param_value_size_ret);

    Call call(oclapi::command::GET_COMMAND_QUEUE_INFO);

    call.record_object_use(command_queue);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clRetainCommandQueue(cl_command_queue command_queue)
{
    auto ret = PFN_clRetainCommandQueue(command_queue);

    Call call(oclapi::command::RETAIN_COMMAND_QUEUE);

    call.record_object_use(command_queue);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clReleaseCommandQueue(cl_command_queue queue) {
    auto ret = PFN_clReleaseCommandQueue(queue);

    Call call(oclapi::command::RELEASE_COMMAND_QUEUE);
    call.record_object_use(queue);
    call.record_return_value(ret);
    trace.record(call);

    return ret;
}

cl_int clEnqueueNDRangeKernel(
    cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim,
    const size_t* global_work_offset, const size_t* global_work_size,
    const size_t* local_work_size, cl_uint num_events_in_wait_list,
    const cl_event* event_wait_list, cl_event* event) {
    auto ret = PFN_clEnqueueNDRangeKernel(
        command_queue, kernel, work_dim, global_work_offset, global_work_size,
        local_work_size, num_events_in_wait_list, event_wait_list, event);

    Call call(oclapi::command::ENQUEUE_NDRANGE_KERNEL);

    call.record_object_use(command_queue);
    call.record_object_use(kernel);
    call.record_value(work_dim);
    call.record_array(work_dim, global_work_offset);
    call.record_array(work_dim, global_work_size);
    call.record_array(work_dim, local_work_size);
    call.record_value(num_events_in_wait_list);
    call.record_object_use(num_events_in_wait_list,
                           const_cast<cl_event*>(event_wait_list));
    call.record_optional_object_creation(event != nullptr ? 1 : 0, event);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clEnqueueWriteImage(cl_command_queue command_queue, cl_mem image,
                           cl_bool blocking_write, const size_t* origin,
                           const size_t* region, size_t input_row_pitch,
                           size_t input_slice_pitch, const void* ptr,
                           cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list, cl_event* event) {

    if (!blocking_write) {
        warn("Non-blocking image write will be made blocking\n");
        trace.set_flag(Trace::flags::kImperfect);
    }
    // FIXME don't force command to be blocking
    auto ret = PFN_clEnqueueWriteImage(
        command_queue, image, CL_BLOCKING, origin, region, input_row_pitch,
        input_slice_pitch, ptr, num_events_in_wait_list, event_wait_list,
        event);
    cl_image_format format;

    // TODO record format at image creation time instead
    auto err = PFN_clGetImageInfo(image, CL_IMAGE_FORMAT, sizeof(format),
                                  &format, nullptr);
    if (err != CL_SUCCESS) {
        fatal("Error while getting image format\n");
    }

    auto data_size = calculate_image_region_size(format, input_row_pitch,
                                                 input_slice_pitch, region);

    Call call(oclapi::command::ENQUEUE_WRITE_IMAGE);

    call.record_object_use(command_queue);
    call.record_object_use(image);
    call.record_value(blocking_write);
    call.record_array(3, origin);
    call.record_array(3, region);
    call.record_value(input_row_pitch);
    call.record_value(input_slice_pitch);
    call.record_array(
        data_size, static_cast<const char*>(ptr)); // TODO introduce blob type?
    call.record_value(num_events_in_wait_list);
    call.record_object_use(num_events_in_wait_list,
                           const_cast<cl_event*>(event_wait_list));
    call.record_optional_object_creation(event != nullptr ? 1 : 0, event);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clEnqueueReadImage(cl_command_queue command_queue, cl_mem image,
                          cl_bool blocking_read, const size_t* origin,
                          const size_t* region, size_t row_pitch,
                          size_t slice_pitch, void* ptr,
                          cl_uint num_events_in_wait_list,
                          const cl_event* event_wait_list, cl_event* event) {
    if (!blocking_read) {
        warn("Non-blocking image read will be made blocking\n");
        trace.set_flag(Trace::flags::kImperfect);
    }
    // FIXME don't force command to be blocking
    auto ret = PFN_clEnqueueReadImage(
        command_queue, image, CL_BLOCKING, origin, region, row_pitch,
        slice_pitch, ptr, num_events_in_wait_list, event_wait_list, event);
    cl_image_format format;

    // TODO record format at image creation time instead
    auto err = PFN_clGetImageInfo(image, CL_IMAGE_FORMAT, sizeof(format),
                                  &format, nullptr);
    if (err != CL_SUCCESS) {
        fatal("Error while getting image format\n");
    }

    auto data_size =
        calculate_image_region_size(format, row_pitch, slice_pitch, region);

    Call call(oclapi::command::ENQUEUE_READ_IMAGE);

    call.record_object_use(command_queue);
    call.record_object_use(image);
    call.record_value(blocking_read);
    call.record_array(3, origin);
    call.record_array(3, region);
    call.record_value(row_pitch);
    call.record_value(slice_pitch);
    call.record_array(data_size, static_cast<char*>(ptr));
    call.record_value(num_events_in_wait_list);
    call.record_object_use(num_events_in_wait_list,
                           const_cast<cl_event*>(event_wait_list));
    call.record_optional_object_creation(event != nullptr ? 1 : 0, event);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

void* clEnqueueMapBuffer(cl_command_queue command_queue, cl_mem buffer,
                         cl_bool blocking_map, cl_map_flags map_flags,
                         size_t offset, size_t size,
                         cl_uint num_events_in_wait_list,
                         const cl_event* event_wait_list, cl_event* event,
                         cl_int* errcode_ret) {
    if (!blocking_map) {
        warn("Non-blocking buffer map will be made blocking\n");
        trace.set_flag(Trace::flags::kImperfect);
    }
    // FIXME don't force command to be blocking
    auto ret = PFN_clEnqueueMapBuffer(
        command_queue, buffer, CL_BLOCKING, map_flags, offset, size,
        num_events_in_wait_list, event_wait_list, event, errcode_ret);

    Call call(oclapi::command::ENQUEUE_MAP_BUFFER);

    call.record_object_use(command_queue);
    call.record_object_use(buffer);
    call.record_value(blocking_map);
    call.record_value(map_flags);
    call.record_value(offset);
    call.record_value(size);
    call.record_value(num_events_in_wait_list);
    call.record_object_use(num_events_in_wait_list,
                           const_cast<cl_event*>(event_wait_list));
    call.record_optional_object_creation(event != nullptr ? 1 : 0, event);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_map_pointer_creation(ret);

    trace.record(call);

    return ret;
}

cl_int clEnqueueUnmapMemObject(cl_command_queue command_queue, cl_mem memobj,
                               void* mapped_ptr,
                               cl_uint num_events_in_wait_list,
                               const cl_event* event_wait_list,
                               cl_event* event) {
    trace.set_flag(Trace::flags::kImperfect);
    // TODO insert a barrier
    // TODO capture memory region
    auto ret = PFN_clEnqueueUnmapMemObject(command_queue, memobj, mapped_ptr,
                                           num_events_in_wait_list,
                                           event_wait_list, event);
    // TODO wait
    // TODO remove mapped pointer

    Call call(oclapi::command::ENQUEUE_UNMAP_MEM_OBJECT);

    call.record_object_use(command_queue);
    call.record_object_use(memobj);
    call.record_map_pointer_use(mapped_ptr);
    call.record_value(num_events_in_wait_list);
    call.record_object_use(num_events_in_wait_list,
                           const_cast<cl_event*>(event_wait_list));
    call.record_optional_object_creation(event != nullptr ? 1 : 0, event);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clEnqueueWriteBuffer(cl_command_queue command_queue, cl_mem buffer,
                            cl_bool blocking_write, size_t offset, size_t size,
                            const void* ptr, cl_uint num_events_in_wait_list,
                            const cl_event* event_wait_list, cl_event* event) {
    if (!blocking_write) {
        warn("Non-blocking image write will be made blocking\n");
        trace.set_flag(Trace::flags::kImperfect);
    }
    // FIXME don't force command to be blocking

    auto ret = PFN_clEnqueueWriteBuffer(
        command_queue, buffer, CL_BLOCKING, offset, size, ptr,
        num_events_in_wait_list, event_wait_list, event);

    Call call(oclapi::command::ENQUEUE_WRITE_BUFFER);

    call.record_object_use(command_queue);
    call.record_object_use(buffer);
    call.record_value(blocking_write);
    call.record_value(offset);
    call.record_value(size);
    call.record_array(size, static_cast<const char*>(ptr));
    call.record_value(num_events_in_wait_list);
    call.record_object_use(num_events_in_wait_list,
                           const_cast<cl_event*>(event_wait_list));
    call.record_optional_object_creation(event != nullptr ? 1 : 0, event);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clEnqueueReadBuffer(cl_command_queue command_queue, cl_mem buffer,
                           cl_bool blocking_read, size_t offset, size_t size,
                           void* ptr, cl_uint num_events_in_wait_list,
                           const cl_event* event_wait_list, cl_event* event) {
    if (!blocking_read) {
        warn("Non-blocking read buffer will be made blocking\n");
        trace.set_flag(Trace::flags::kImperfect);
    }
    // FIXME don't force command to be blocking
    auto ret = PFN_clEnqueueReadBuffer(
        command_queue, buffer, CL_BLOCKING, offset, size, ptr,
        num_events_in_wait_list, event_wait_list, event);

    Call call(oclapi::command::ENQUEUE_READ_BUFFER);

    call.record_object_use(command_queue);
    call.record_object_use(buffer);
    call.record_value(blocking_read);
    call.record_value(offset);
    call.record_value(size);
    call.record_array(size, static_cast<char*>(ptr));
    call.record_value(num_events_in_wait_list);
    call.record_object_use(num_events_in_wait_list,
                           const_cast<cl_event*>(event_wait_list));
    call.record_optional_object_creation(event != nullptr ? 1 : 0, event);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clEnqueueTask(cl_command_queue command_queue, cl_kernel kernel,
                     cl_uint num_events_in_wait_list,
                     const cl_event* event_wait_list, cl_event* event) {
    auto ret = PFN_clEnqueueTask(command_queue, kernel, num_events_in_wait_list,
                                 event_wait_list, event);

    Call call(oclapi::command::ENQUEUE_TASK);

    call.record_object_use(command_queue);
    call.record_object_use(kernel);
    call.record_value(num_events_in_wait_list);
    call.record_object_use(num_events_in_wait_list,
                           const_cast<cl_event*>(event_wait_list));
    call.record_optional_object_creation(event != nullptr ? 1 : 0, event);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetEventInfo(cl_event event, cl_event_info param_name,
                      size_t param_value_size, void* param_value,
                      size_t* param_value_size_ret) {
    auto ret = PFN_clGetEventInfo(event, param_name, param_value_size,
                                  param_value, param_value_size_ret);

    Call call(oclapi::command::GET_EVENT_INFO);

    call.record_object_use(event);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clGetEventProfilingInfo(cl_event event, cl_profiling_info param_name,
                               size_t param_value_size, void* param_value,
                               size_t* param_value_size_ret) {
    auto ret = PFN_clGetEventProfilingInfo(event, param_name, param_value_size,
                                           param_value, param_value_size_ret);

    Call call(oclapi::command::GET_EVENT_PROFILING_INFO);

    call.record_object_use(event);
    call.record_value(param_name);
    call.record_value(param_value_size);
    call.record_value_out_by_reference(param_value, param_value_size);
    call.record_value_out_by_reference(param_value_size_ret);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_event clCreateUserEvent(cl_context context, cl_int* errcode_ret) {
    auto ret = PFN_clCreateUserEvent(context, errcode_ret);

    Call call(oclapi::command::CREATE_USER_EVENT);

    call.record_object_use(context);
    call.record_value_out_by_reference(errcode_ret);
    call.record_return_object_creation(ret);

    trace.record(call);

    return ret;
}

cl_int clSetUserEventStatus(cl_event event, cl_int execution_status) {
    auto ret = PFN_clSetUserEventStatus(event, execution_status);

    Call call(oclapi::command::SET_USER_EVENT_STATUS);

    call.record_object_use(event);
    call.record_value(execution_status);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clWaitForEvents(cl_uint num_events, const cl_event* event_list) {
    auto ret = PFN_clWaitForEvents(num_events, event_list);

    Call call(oclapi::command::WAIT_FOR_EVENTS);

    call.record_value(num_events);
    call.record_object_use(num_events, const_cast<cl_event*>(event_list));

    return ret;
}

cl_int clRetainEvent(cl_event event) {
    auto ret = PFN_clRetainEvent(event);

    Call call(oclapi::command::RETAIN_EVENT);

    call.record_object_use(event);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clReleaseEvent(cl_event event) {
    auto ret = PFN_clReleaseEvent(event);

    Call call(oclapi::command::RELEASE_EVENT);

    call.record_object_use(event);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clFlush(cl_command_queue command_queue) {
    auto ret = PFN_clFlush(command_queue);

    Call call(oclapi::command::FLUSH);

    call.record_object_use(command_queue);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clFinish(cl_command_queue queue) {
    auto ret = PFN_clFinish(queue);

    Call call(oclapi::command::FINISH);

    call.record_object_use(queue);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

void* clSVMAlloc(cl_context context, cl_svm_mem_flags flags, size_t size,
                 unsigned int alignment) {
    auto ret = PFN_clSVMAlloc(context, flags, size, alignment);

    Call call(oclapi::command::SVMALLOC);
    call.record_object_use(context);
    call.record_value(flags);
    call.record_value(size);
    call.record_value(alignment);
    call.record_return_map_pointer_creation(ret);

    trace.record(call);

    return ret;
}

void clSVMFree(cl_context context, void* svm_pointer) {
    PFN_clSVMFree(context, svm_pointer);

    Call call(oclapi::command::SVMFREE);
    call.record_object_use(context);
    call.record_map_pointer_use(svm_pointer);

    trace.record(call);
}

cl_int clUnloadCompiler(void)
{
    auto ret = PFN_clUnloadCompiler();

    Call call(oclapi::command::UNLOAD_COMPILER);

    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

cl_int clUnloadPlatformCompiler(cl_platform_id platform) {
    auto ret = PFN_clUnloadPlatformCompiler(platform);

    Call call(oclapi::command::UNLOAD_PLATFORM_COMPILER);

    call.record_object_use(platform);
    call.record_return_value(ret);

    trace.record(call);

    return ret;
}

#include <sys/types.h>
#include <unistd.h>

struct initialiser {
    initialiser() {
        ocltools_log_init();
        info("[%d] starting init\n", getpid());
        init_api(RTLD_NEXT);
        info("[%d] init done\n", getpid());
    }

    ~initialiser() {
        int pid = getpid();
        info("[%d] ending\n", pid);
        // trace.print(std::cout);

        const char* fname = getenv("OCLTRACE_TRACEFILE");
        if (fname == nullptr) {
            fname = "test.ocltrace";
        }
        std::string tracefile{fname};
        tracefile += "." + std::to_string(pid);
        if (trace.has_calls()) {
            // TODO do not overwrite by default, use PID and increment
            trace.save(tracefile);
            info("[%d] saving trace to %s\n", pid, tracefile.c_str());
        } else {
            info("[%d] no calls captured, not saving a trace\n", pid);
        }
    }
} gInitialiser;
