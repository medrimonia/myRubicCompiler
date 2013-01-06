#!/bin/bash

FAILED="\033[40C[\e[1;31mFAILED\e[0m]\033[48D"
PASSED="\033[40C[\e[1;32mPASSED\e[0m]\033[48D"

make tests

TESTS=(*.test)

for ((i = 0; i < ${#TESTS[@]} ; i++))
do
		./${TESTS[$i]} && printf $PASSED || printf $FAILED
		printf "${TESTS[$i]}\n" 
done