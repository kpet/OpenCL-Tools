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
#include "log.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <functional>

static bool wait_for_child() {
    int child_status;
    if (wait(&child_status) == -1) {
        error("Can't wait for child to complete: %s\n", strerror(errno));
        return false;
    }
    if (WIFEXITED(child_status)) {
        int ret = WEXITSTATUS(child_status);
        info("Child terminated normally with %d\n", ret);
        return true;
    } else if (WIFSIGNALED(child_status)) {
        int signum = WTERMSIG(child_status);
        error("Child terminated by signal %d\n", signum);
        return false;
    } else {
        error("Child stopped execution\n");
        return false;
    }
    return true;
}

bool run_binary_with_preload(const std::string &app, const std::vector<std::string> &args,
                             const std::string& preload, const std::vector<std::string>& env,
                             std::function<bool()> parent_fn) {
    int child_pid = fork();
    if (child_pid != 0) {
        if (parent_fn != nullptr) {
            return parent_fn();
        } else {
            return wait_for_child();
        }
    } else {
        std::vector<const char*> args_c{app.c_str()};
        for (auto &arg : args) {
            args_c.push_back(arg.c_str());
        }
        args_c.push_back(nullptr);

        std::vector<const char*> env_c;
        int i = 0;
        while (environ[i] != nullptr) {
            // TODO check and filter LD_PRELOAD
            env_c.push_back(environ[i]);
            i++;
        }
        std::string var_ld_preload{"LD_PRELOAD="};
        var_ld_preload += preload;
        env_c.push_back(var_ld_preload.c_str());
        for (auto& e : env) {
            env_c.push_back(e.c_str());
        }
        env_c.push_back(nullptr);
        int ret = execvpe(app.c_str(),
                          const_cast<char * const*>(args_c.data()),
                          const_cast<char*const*>(env_c.data()));
        // TODO report whta's in errno
        return false;
    }
    return true;
}

