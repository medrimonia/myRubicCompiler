#!/bin/bash

FAILED="[\e[1;31mFAILED\e[0m]\n"
PASSED="[\e[1;32mPASSED\e[0m]\n"

make test >ci_test
printf "test       :\t"
./test && printf $PASSED || printf $FAILED

make test_appel >ci_test_appel
printf "test_appel :\t"
./test_appel && printf $PASSED || printf $FAILED