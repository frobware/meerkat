#!/bin/bash

lcov --directory . --zerocounters
make clean
make
./bin/test-message
./bin/test-mprintf
./bin/test-allocator
lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info '/usr/*' --output-file coverage.final
mv coverage.final coverage.info
genhtml -o coveragereport coverage.info
