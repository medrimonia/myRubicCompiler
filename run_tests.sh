#!/bin/bash

FAILED="[\e[1;31mFAILED\e[0m]"
PASSED="[\e[1;32mPASSED\e[0m]"

make tests -i >/dev/null 2>/dev/null

NB_TESTS=$(ls *.rubic | wc -w)
NB_TESTS_COMPILED=$(ls *.test | wc -w)

TESTS=(*.test)


printf "\033[25C\033[1m ${NB_TESTS_COMPILED} / ${NB_TESTS} tests compiling\033[0m\n"

for ((i = 0; i < ${#TESTS[@]} ; i++))
do
		printf "${TESTS[$i]}\033[80D\033[60C" 
		./${TESTS[$i]} >${TESTS[$i]}.output 2>${TESTS[$i]}.error \
				&& printf $PASSED || printf $FAILED
		printf "\n"
done