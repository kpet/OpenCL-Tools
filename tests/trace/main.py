#!/usr/bin/env python
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


import collections
import filecmp
import glob
import os
import shutil
import subprocess
import sys
import tempfile
import unittest

BUILD_DIR_DEFAULT = os.path.abspath('./build')
SOURCE_DIR_DEFAULT = os.path.abspath('.')
BUILD_DIR = os.environ.get('CLTRACE_BUILD_DIR', BUILD_DIR_DEFAULT)
SOURCE_DIR = os.environ.get('CLTRACE_SOURCE_DIR', SOURCE_DIR_DEFAULT)
OPENCL_LIB_DIR = os.environ.get('CLTRACE_OPENCL_LIB_DIR')
CLTRACE = os.path.join(BUILD_DIR, 'cltrace')
CLTESTS = os.path.join(BUILD_DIR, 'tests', 'trace', 'cltests')
CXX_COMPILER = 'g++'

def run_cltrace(args, cwd=None):
    env = dict(os.environ)
    lib_paths = [BUILD_DIR]
    if OPENCL_LIB_DIR:
        lib_paths.append(OPENCL_LIB_DIR)
    env['LD_LIBRARY_PATH'] = ':'.join(lib_paths)
    #debug = ['gdb','-ex','set follow-fork-mode child','-ex','run','-ex','bt','-ex','quit','--args']
    debug = []
    cmd = debug + [CLTRACE] + args
    res = subprocess.run(cmd, capture_output=True, env=env, cwd=cwd)
    return res

def compile_source(source, binary, cwd):
    cmd = [
        CXX_COMPILER,
        '-o', binary,
        '-DCL_TARGET_OPENCL_VERSION=300',
        '-Wno-deprecated-declarations',
        '-I{}'.format(os.path.join(SOURCE_DIR, 'external', 'OpenCL-Headers')),
    ]
    if OPENCL_LIB_DIR:
        cmd.append('-L{}'.format(OPENCL_LIB_DIR))
    cmd += [
        source,
        '-lOpenCL'
    ]
    res = subprocess.run(cmd, capture_output=True, cwd=cwd)
    return res

class TestCommandLineInterface(unittest.TestCase):

    def test_help(self):
        res = run_cltrace(['--help'])
        print(res.stderr)
        self.assertEqual(res.returncode, 0)
        self.assertGreater(len(res.stdout), 0)
        self.assertEqual(len(res.stderr), 0)

    def test_noparameters(self):
        res = run_cltrace([])
        self.assertNotEqual(res.returncode, 0)
        self.assertEqual(len(res.stdout), 0)
        self.assertGreater(len(res.stderr), 0)

    def test_capture_no_app(self):
        res = run_cltrace(['dummy.trace', 'capture'])
        self.assertNotEqual(res.returncode, 0)
        self.assertEqual(len(res.stdout), 0)
        self.assertGreater(len(res.stderr), 0)

    def test_capture_app_args(self):
        res = run_cltrace(['dummy.trace', 'capture', 'echo', '-d'])
        self.assertEqual(res.returncode, 0)
        self.assertGreater(len(res.stdout), 0)
        self.assertEqual(len(res.stderr), 0)

class TestRoundTrip(unittest.TestCase):

    def test_round_trip(self):

        # Create test directory
        tmpdir = tempfile.mkdtemp(prefix='OpenCL-Tools-trace-')
        print("Test dir: ", tmpdir)

        # Capture trace
        res = run_cltrace(['capture.trace', 'capture', CLTESTS], cwd=tmpdir)
        print(res.stdout.decode('utf-8'))
        print(res.stderr.decode('utf-8'))
        self.assertEqual(res.returncode, 0)
        self.assertEqual(len(res.stderr), 0)

        # Check a single trace file was created and get its name
        files = os.listdir(tmpdir)
        self.assertEqual(len(files), 1)
        tracefile = files[0]

        # TODO check that the trace is not empty

        # Generate source
        res = run_cltrace([tracefile, 'generate-source'], cwd=tmpdir)
        self.assertEqual(res.returncode, 0)
        self.assertEqual(len(res.stderr), 0)
        self.assertGreater(len(res.stdout), 0)
        srcfile = os.path.join(tmpdir, 'gen.cpp')
        with open(srcfile, 'w') as f:
            f.write(res.stdout.decode('utf-8'))

        # Compile source
        binary = os.path.join(tmpdir, 'gen')
        res = compile_source(srcfile, binary, tmpdir)
        print(res.stdout.decode('utf-8'))
        print(res.stderr.decode('utf-8'))
        self.assertEqual(res.returncode, 0)
        self.assertEqual(len(res.stderr), 0)
        self.assertEqual(len(res.stdout), 0)

        # Capture from generated program
        res = run_cltrace(['gen.trace', 'capture', binary], cwd=tmpdir)
        print(res.stdout.decode('utf-8'))
        print(res.stderr.decode('utf-8'))
        self.assertEqual(res.returncode, 0)
        self.assertEqual(len(res.stderr), 0)
        files = glob.glob(os.path.join(tmpdir,'gen.trace.*'))
        self.assertEqual(len(files), 1)
        gentrace = files[0]

        # Generate source
        res = run_cltrace([gentrace, 'generate-source'], cwd=tmpdir)
        self.assertEqual(res.returncode, 0)
        self.assertEqual(len(res.stderr), 0)
        self.assertGreater(len(res.stdout), 0)
        gensrcfile = os.path.join(tmpdir, 'regen.cpp')
        with open(gensrcfile, 'w') as f:
            f.write(res.stdout.decode('utf-8'))

        # Compare sources
        self.assertTrue(filecmp.cmp(srcfile, gensrcfile))

        # Cleanup
        shutil.rmtree(tmpdir)

# TODO test replay
# TODO test trace contents
# TODO test info, stats

if __name__ == '__main__':
    unittest.main(verbosity=2)
