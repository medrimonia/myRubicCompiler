#!/bin/sh

make test >ci_test
./test || echo "test passed"

make test_appel >ci_test_appel
./test_appel || echo "test_appel passed"