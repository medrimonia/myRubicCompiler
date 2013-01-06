#!/bin/bash

FAILED="[\e[1;31mFAILED\e[0m]"
PASSED="[\e[1;32mPASSED\e[0m]"

make tests >/dev/null

TESTS=(*.test)

for ((i = 0; i < ${#TESTS[@]} ; i++))
do
		printf "${TESTS[$i]}\033[80D\033[60C" 
		./${TESTS[$i]} >${TESTS[$i]}.output 2>${TESTS[$i]}.error \
				&& printf $PASSED || printf $FAILED
		printf "\n"
done