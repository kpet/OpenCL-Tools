# Copyright 2019-2023 The OpenCL-Tools authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


set(OCL_API_XML ${PROJECT_SOURCE_DIR}/external/OpenCL-Docs/xml/cl.xml)
set(OCL_API_GENERATOR ${CMAKE_CURRENT_SOURCE_DIR}/generate-stubs.py)
set(OCL_API_GEN ${CMAKE_CURRENT_BINARY_DIR}/ocltools-trace-stubs-gen.hpp)

add_custom_command(
    OUTPUT ${OCL_API_GEN}
    COMMAND env PYTHONPATH=${PROJECT_SOURCE_DIR}/src/common python ${OCL_API_GENERATOR} -i ${OCL_API_XML} -o ${OCL_API_GEN}
    DEPENDS ${OCL_API_GENERATOR} ${OCL_API_XML}
    ${PROJECT_SOURCE_DIR}/src/common/oclspec.py)

add_custom_target(generate-trace-stubs DEPENDS ${OCL_API_GEN})

ocltools_add_preload_library(ocltools-trace cltrace.cpp trace-stubs.cpp
    ${OCL_API_GEN})
target_compile_options(ocltools-trace PRIVATE "-Wno-attributes")
target_include_directories(ocltools-trace PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
add_dependencies(ocltools-trace generate-trace-stubs)

ocltools_add_tool(cltrace ocltrace.cpp trace.cpp)
add_dependencies(cltrace generate-ocltools-loader)
add_dependencies(cltrace generate-ocl-api)
target_link_libraries(cltrace dl)
