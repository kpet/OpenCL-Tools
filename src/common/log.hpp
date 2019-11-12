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


#pragma once

enum loglevel
{
    fatal = 0,
    error = 1,
    warn = 2,
    info = 3,
    debug = 4
};

#ifndef _MSC_VER
#define CHECK_PRINTF(index, first) __attribute__((format(printf, index, first)))
#else
#define CHECK_PRINTF(index, first)
#endif

void ocltools_log_init();
void ocltools_log_term();

void ocltools_log(loglevel level, const char* fmt, ...) CHECK_PRINTF(2, 3);

#ifndef OCLTOOLS_LOG_PREFIX
#define OCLTOOLS_LOG_PREFIX
#endif

#define fatal(fmt, ...) ocltools_log(loglevel::fatal, OCLTOOLS_LOG_PREFIX fmt "\n", ##__VA_ARGS__)
#define error(fmt, ...) ocltools_log(loglevel::error, OCLTOOLS_LOG_PREFIX fmt "\n", ##__VA_ARGS__)
#define warn(fmt, ...) ocltools_log(loglevel::warn, OCLTOOLS_LOG_PREFIX fmt "\n", ##__VA_ARGS__)
#define info(fmt, ...) ocltools_log(loglevel::info, OCLTOOLS_LOG_PREFIX fmt "\n", ##__VA_ARGS__)
#define debug(fmt, ...) ocltools_log(loglevel::debug, OCLTOOLS_LOG_PREFIX fmt "\n", ##__VA_ARGS__)

