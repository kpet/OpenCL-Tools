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

set(SOURCES main.cpp simple.cpp info-tests.cpp api-object-tests.cpp memobjects-tests.cpp image-tests.cpp kernel-tests.cpp event-tests.cpp svm-tests.cpp command-tests.cpp)

add_executable(cltests ${SOURCES})
target_include_directories(cltests PRIVATE
    ${PROJECT_SOURCE_DIR}/external/OpenCL-Headers)
if (OPENCL_SDK_BUILD_DIR)
    target_link_directories(cltests PRIVATE ${OPENCL_SDK_BUILD_DIR})
endif()
target_link_libraries(cltests gtest gtest_main OpenCL)
