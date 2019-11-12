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

    code = ''
    for cmd in spec.commands():
        name = cmd.name
        rettype = cmd.rettype
        params = cmd.params
        param_names = cmd.param_names
        pdef = '\n{} {}({}){{\n'.format(rettype, name, ', '.join(params))
        pdef += '\tunimplemented("{}");\n'.format(name)
        pdef += '}\n'
        code += pdef

    with open(args.o, 'w') as f:
        f.write(code)
