#!/bin/bash

set -e

lcov --directory . --zerocounters
make clean
make
./build/bin/test-message
./build/bin/test-mprintf
./build/bin/test-allocator
lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.final
mv coverage.final coverage.info
genhtml -o coveragereport coverage.info
