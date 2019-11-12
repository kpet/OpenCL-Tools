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
set(OCL_API_GENERATOR ${PROJECT_SOURCE_DIR}/src/common/generate-loader.py)
set(OCL_API_GEN ${CMAKE_CURRENT_BINARY_DIR}/ocltools-loader-gen.hpp)

add_custom_command(
    OUTPUT ${OCL_API_GEN}
    COMMAND ${OCL_API_GENERATOR} -i ${OCL_API_XML} -o ${OCL_API_GEN}
    DEPENDS ${OCL_API_GENERATOR} ${OCL_API_XML})

add_custom_target(generate-ocltools-loader DEPENDS ${OCL_API_GEN})

set(OCL_API_GENERATOR
    ${PROJECT_SOURCE_DIR}/src/common/generate-api-definitions.py)
set(OCL_API_GEN ${CMAKE_CURRENT_BINARY_DIR}/ocl-api-gen.hpp)

add_custom_command(
    OUTPUT ${OCL_API_GEN}
    COMMAND ${OCL_API_GENERATOR} -i ${OCL_API_XML} -o ${OCL_API_GEN}
    DEPENDS ${OCL_API_GENERATOR} ${OCL_API_XML}
    ${PROJECT_SOURCE_DIR}/src/common/oclspec.py)

add_custom_target(generate-ocl-api DEPENDS ${OCL_API_GEN})

add_library(ocltools-common ocltools.cpp log.cpp)
set_property(TARGET ocltools-common PROPERTY POSITION_INDEPENDENT_CODE ON)
