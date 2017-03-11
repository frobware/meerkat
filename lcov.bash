#!/bin/bash

lcov --directory . --zerocounters
make clean
make
./bin/test-message
lcov --directory . --capture --output-file coverage.info
lcov --remove coverage.info 'lib/test/*' '/usr/*' --output-file coverage.final
mv coverage.final coverage.info
genhtml -o coveragereport coverage.info
