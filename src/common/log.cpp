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

#include "log.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <unistd.h>

static int gLoggingLevel;
static bool gLoggingColour;
static FILE* gLoggingFile;

bool ocltools_log_level_enabled(loglevel level) { return gLoggingLevel >= level; }

static const char colourRed[] = "\e[0;31m";
static const char colourYellow[] = "\e[0;33m";
static const char colourReset[] = "\e[0m";

void ocltools_log_init() {
    char* logging = getenv("OCLTOOLS_LOG");
    if (logging) {
        loglevel setting = static_cast<loglevel>(atoi(logging));
        if ((setting < loglevel::fatal) || (setting > loglevel::debug)) {
            // FIXME handle all errors
            fprintf(stderr, "FATAL: Unknown log level '%s'.\n", logging);
            exit(EXIT_FAILURE);
        }
        gLoggingLevel = setting;
    } else {
        gLoggingLevel = loglevel::info;
    }

    char* logging_dest = getenv("OCLTOOLS_LOG_DEST");
    if (logging_dest) {

        std::string val(logging_dest);

        if (val == "stdout") {
            gLoggingFile = stdout;
        } else if (val == "stderr") {
            gLoggingFile = stderr;
        } else if (val.rfind("file:", 0) == 0) {

            val.erase(0, strlen("file:"));

            gLoggingFile = fopen(val.c_str(), "w+");

            if (gLoggingFile == nullptr) {
                fprintf(stderr, "FATAL: Could not open log file '%s': %s.\n",
                        val.c_str(), strerror(errno));
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "FATAL: Unknown log destination '%s'.\n",
                    val.c_str());
            exit(EXIT_FAILURE);
        }
    } else {
        gLoggingFile = stdout;
    }

    bool isTTY = isatty(fileno(gLoggingFile));
    if (isTTY) {
        gLoggingColour = true;
    } else {
        gLoggingColour = false;
    }

    char* logging_colour = getenv("OCLTOOLS_LOG_COLOUR");
    if (logging_colour) {
        int val = atoi(logging_colour);
        // FIXME handle errors
        if (val == 0) {
            gLoggingColour = false;
        } else {
            gLoggingColour = true;
        }
    }
}

void ocltools_log_term() {
    if ((gLoggingFile != stdout) && (gLoggingFile != stderr)) {
        fclose(gLoggingFile);
    }
}

void ocltools_log(loglevel level, const char* fmt, ...) {

    if (!ocltools_log_level_enabled(level)) {
        return;
    }

    const char* colourCode = nullptr;

    if (gLoggingColour) {

        switch (level) {
        case loglevel::fatal:
        case loglevel::error:
            colourCode = colourRed;
            break;
        case loglevel::warn:
            colourCode = colourYellow;
            break;
        case loglevel::info:
        case loglevel::debug:
            break;
        }

        if (colourCode != nullptr) {
            fprintf(gLoggingFile, "%s", colourCode);
        }
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(gLoggingFile, fmt, args);
    va_end(args);

    if ((gLoggingColour) && (colourCode != nullptr)) {
        fprintf(gLoggingFile, "%s", colourReset);
    }

    if (level == loglevel::fatal) {
        abort();
    }
}

