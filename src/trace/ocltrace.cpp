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

#include "ocltools.hpp"

#include "CLI/CLI.hpp"
#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "trace.hpp"

#include "visitor-replay.hpp"
#include "visitor-srcgen.hpp"

bool handle_capture(const std::string& tracefile, const std::string& app,
                    const std::vector<std::string>& args) {

    std::vector<std::string> env;
    std::string env_tracefile{"OCLTRACE_TRACEFILE="};
    env_tracefile += tracefile;
    env.push_back(env_tracefile);

    return run_binary_with_preload(app, args, "libocltools-trace.so", env);
}

bool handle_replay(const std::string& tracefile) {
    void* handle = dlopen("libOpenCL.so", RTLD_LAZY);
    init_api(handle);
    Trace trace;
    trace.load(tracefile);
    TraceReplayVisitor replay;
    replay.visit(trace);
    return true;
}

bool handle_srcgen(const std::string& tracefile) {
    TraceSourceGenerationVisitor srcgen;
    Trace trace;
    trace.load(tracefile);
    srcgen.visit(trace);
    info("%s", srcgen.source().c_str());
    return true;
}

bool handle_info(const std::string& tracefile) {
    Trace trace;
    trace.load(tracefile);
    trace.print_info(std::cout);
    return true;
}

bool handle_print(const std::string& tracefile) {
    Trace trace;
    trace.load(tracefile);
    trace.print(std::cout);
    return true;
}

bool handle_stats(const std::string& tracefile) {
    Trace trace;
    trace.load(tracefile);
    trace.print_stats(std::cout);
    return true;
}

int main(int argc, char* argv[]) {
    CLI::App app{"OpenCL trace tool"};
    app.set_help_all_flag("--help-all", "Print full help for all subcommands");
    app.require_subcommand();

    std::string tracefile;
    app.add_option("tracefile", tracefile, "The trace file to operate on")
        ->required();

    std::string application;
    CLI::App* cmd_capture = app.add_subcommand("capture", "Capture a trace");
    cmd_capture->allow_extras();
    cmd_capture->add_option("application", application)->required();

    CLI::App* cmd_replay = app.add_subcommand("replay", "Replay a trace");

    CLI::App* cmd_srcgen =
        app.add_subcommand("generate-source", "Generate a C++ source file");

    CLI::App* cmd_info = app.add_subcommand("info", "Infos on a trace");

    CLI::App* cmd_print = app.add_subcommand("print", "Print a trace");

    CLI::App* cmd_stats = app.add_subcommand("stats", "Stats on a trace");

    CLI11_PARSE(app, argc, argv);

    ocltools_log_init();

    bool success;
    if (app.got_subcommand(cmd_capture)) {
        std::vector<std::string> application_args = cmd_capture->remaining();
        success = handle_capture(tracefile, application, application_args);
    } else if (app.got_subcommand(cmd_replay)) {
        success = handle_replay(tracefile);
    } else if (app.got_subcommand(cmd_srcgen)) {
        success = handle_srcgen(tracefile);
    } else if (app.got_subcommand(cmd_info)) {
        success = handle_info(tracefile);
    } else if (app.got_subcommand(cmd_print)) {
        success = handle_print(tracefile);
    } else if (app.got_subcommand(cmd_stats)) {
        success = handle_stats(tracefile);
    }

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
