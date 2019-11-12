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


macro(ocltools_add_preload_library libname libsources)
    add_library(${libname} SHARED ${libsources} ${ARGN})
    add_dependencies(${libname} generate-ocltools-loader)
    target_include_directories(${libname} PRIVATE
        ${PROJECT_SOURCE_DIR}/external/OpenCL-Headers
        ${PROJECT_BINARY_DIR}/src/common
        ${PROJECT_SOURCE_DIR}/src/common)
    target_link_libraries(${libname} dl ocltools-common)
    set_target_properties(${libname} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endmacro()

macro(ocltools_add_tool toolname toolsources)
    add_executable(${toolname} ${toolsources} ${ARGN})
    target_include_directories(${toolname} PRIVATE
        ${PROJECT_SOURCE_DIR}/external/CLI11/include
        ${PROJECT_SOURCE_DIR}/src/common
        ${PROJECT_BINARY_DIR}/src/common
        ${PROJECT_SOURCE_DIR}/external/OpenCL-Headers)
    target_link_libraries(${toolname} ocltools-common)
    set_target_properties(${toolname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endmacro()

add_subdirectory(common)
add_subdirectory(trace)
