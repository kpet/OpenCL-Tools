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

cmake_minimum_required(VERSION 3.0)
project(OpenCL-Tools)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckCXXCompilerFlag)

macro(AddCXXFlagIfSupported flag test)
   CHECK_CXX_COMPILER_FLAG(${flag} ${test})
   if( ${${test}} )
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
   endif()
endmacro()

if("Ninja" STREQUAL ${CMAKE_GENERATOR})
    AddCXXFlagIfSupported(-fdiagnostics-color COMPILER_SUPPORTS_COLOR_DIAGNOSTICS)
endif()

add_subdirectory(src)
# TODO make conditional, detect GoogleTest, etc

add_subdirectory(${PROJECT_SOURCE_DIR}/external/googletest EXCLUDE_FROM_ALL)
add_subdirectory(tests)

