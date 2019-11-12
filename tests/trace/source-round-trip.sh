#!/usr/bin/env bash

set -ex

TESTBIN=/mnt/work/repos/clvk/build/simple_test
OCLTRACE=$(pwd)/build/cltrace
export LD_LIBRARY_PATH=$(pwd)/build
export OCL_ICD_PLATFORM_SORT=none # See https://github.com/OCL-dev/ocl-icd/issues/14

TESTDIR=$(mktemp -d ocltrace.XXXXXX)
echo "Test directory: ${TESTDIR}"

cd ${TESTDIR}

TRACE="test.trace"
TRACE_GEN="test-gen.trace"

# Capture trace
${OCLTRACE} ${TRACE} capture ${TESTBIN}

# Show info
${OCLTRACE} ${TRACE}* info
${OCLTRACE} ${TRACE}* info

# Replay trace
#${OCLTRACE} ${TRACE}* replay

# Generate source FIXME should see PID when only one process saved a trace
${OCLTRACE} ${TRACE}* generate-source > gen.cpp

# Compile source
g++ -o gen gen.cpp -lOpenCL

# Capture trace from compiled source
${OCLTRACE} ${TRACE_GEN} capture ./gen

# Generate source for it
${OCLTRACE} ${TRACE_GEN}* generate-source > regen.cpp

# Sources have to match
diff gen.cpp regen.cpp
