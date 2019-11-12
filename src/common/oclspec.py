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


import xml.etree.ElementTree as ET
import collections

Command = collections.namedtuple('Command', 'rettype name params param_names')

class OpenCLSpec:

    def __init__(self, xmlfile):
        self.xmlfile = xmlfile
        self._commands = []

    def load(self):
        self.tree = ET.parse(self.xmlfile)
        tree = self.tree
        root = tree.getroot()
        commands = root.find('commands')
        for cmd in commands.iter('command'):
            proto = cmd.find('proto')
            name = proto.find('name').text
            excludes = ('D3D', 'DX9', 'EGL', 'FromGLsync', 'GetGL', 'VA','clGetLayerInfo','clInitLayer') # FIXME should start from list of required APIs instead
            exclude = False
            for ex in excludes:
                if ex in name:
                    exclude = True
                    break
            if exclude: continue
            proto_type = proto.find('type')
            #print(proto_type.text)
            rettype = ''
            if hasattr(proto_type, 'text'):
                rettype += proto_type.text
            if hasattr(proto_type, 'tail') and proto_type.tail:
                rettype += proto_type.tail
            #print(rettype)
            if proto.text:
                rettype += proto.text
            params = []
            param_names = []
            for param in cmd.iter('param'):
                ptxt = ''
                ###for child in param:
                ###    ptxt += child.text if child.text else ''
                ###    ptxt += ' '
                if param.text:
                    ptxt += param.text.strip()
                    ptxt += ' '
                ptype = param.find('type')
                if hasattr(ptype, 'text'):
                    ptxt += ptype.text.strip()
                    ptxt += ' '
                if hasattr(ptype, 'tail'):
                    ptxt += ptype.tail.strip()
                    ptxt += ' '
                pname = param.find('name')
                if hasattr(pname, 'text'):
                    ptxt += pname.text.strip()
                    param_names.append(pname.text.strip())
                if hasattr(pname, 'tail') and pname.tail:
                    ptxt += ' '
                    ptxt += pname.tail.strip()
                params.append(ptxt.strip())
            self._commands.append(Command(rettype.strip(), name.strip(), params, param_names))

    def commands(self):
        for cmd in self._commands:
            yield cmd


def generate_loader(spec):
    code = ''
    code_init = ''
    for cmd in spec.commands():
        if cmd.name in ('clGetLayerInfo','clInitLayer'): continue
        code += '\ntypedef {} (*TPFN_{})({});\n'.format(cmd.rettype, cmd.name, ', '.join(cmd.params))
        code += 'TPFN_{} PFN_{};\n'.format(cmd.name, cmd.name)
        code_init += 'PFN_{} = reinterpret_cast<TPFN_{}>(dlsym(handle, "{}"));\n'.format(cmd.name, cmd.name, cmd.name)
    code_init_fn = 'void init_api(void *handle){\n' + code_init + '}\n'

    return code + code_init_fn

def camel_to_all_caps(name):
    dname = ''
    p = ''
    for c in name:
        if c.isupper():
            if not p.isupper() and not p.isdigit():
                dname += '_'
            dname += c
        else:
            dname += c.upper()
        p = c
    return dname[3:]
