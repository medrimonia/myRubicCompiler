#!/bin/bash

FAILED="[\e[1;31mFAILED\e[0m]\n"
PASSED="[\e[1;32mPASSED\e[0m]\n"

TESTS[0]="test      "
TESTS[1]="test_appel"

for ((i = 0; i < ${#TESTS[@]} ; i++))
do
		printf "${TESTS[$i]} :\t" 
		./${TESTS[$i]} && printf $PASSED || printf $FAILED
done