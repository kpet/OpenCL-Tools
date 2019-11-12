#!/usr/bin/env python3
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


import oclspec

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser()

    parser.add_argument('-i', required=True, help='API XML')
    parser.add_argument('-o', required=True, help='Output C++')

    args = parser.parse_args()

    spec = oclspec.OpenCLSpec(args.i)
    spec.load()

    callnum = 0
    commands = []
    for cmd in spec.commands():
        name_caps = oclspec.camel_to_all_caps(cmd.name)
        commands.append((callnum, cmd.name, name_caps))
        callnum += 1

    # Generate call enum
    code = 'enum class command : uint32_t {\n'
    for cmd in commands:
        code += '\t{} = {},\n'.format(cmd[2], cmd[0])
    code += '\tMAX_COMMAND = {}\n'.format(len(commands))
    code += '};\n'

    # Generate call name map
    code += 'static const char* gCommandNames[] = {\n'
    for cmd in commands:
        code += '\t"{}",\n'.format(cmd[1])
        callnum += 1
    code += '};\n'
    with open(args.o, 'w') as f:
        f.write(code)

