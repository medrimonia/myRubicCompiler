#!/bin/bash

FAILED="[\e[1;31mFAILED\e[0m]"
PASSED="[\e[1;32mPASSED\e[0m]"

make tests -i >/dev/null 2>/dev/null

NB_TESTS=$(ls tests/*.rubic | wc -w)
NB_TESTS_COMPILED=$(ls tests/*.test | wc -w)

RUBICS_FILES=(tests/*.rubic)

printf "\033[25C\033[1m COMPILING \033[0m\n"

for ((i = 0; i < ${#RUBICS_FILES[@]} ; i++))
do
		TARGET=${RUBICS_FILES[i]%.rubic}.test
		COMPILE_OUTPUT=${RUBICS_FILES[i]%.rubic}.compile.output
		COMPILE_ERROR=${RUBICS_FILES[i]%.rubic}.compile.error
		make ${TARGET} >${COMPILE_OUTPUT} 2>${COMPILE_ERROR}
		if [[ $? -ne 0 ]]
		then
				printf "${TARGET}\033[80D\033[60C"
				printf $FAILED
				printf "\n"
		fi
done

TESTS=(tests/*.test)


printf "\033[1m${NB_TESTS_COMPILED} / ${NB_TESTS} tests compiling\033[0m\n"

printf "\033[25C\033[1m RUNNING TESTS \033[0m\n"

for ((i = 0; i < ${#TESTS[@]} ; i++))
do
		printf "${TESTS[$i]}\033[80D\033[60C" 
		./${TESTS[$i]} >${TESTS[$i]}.output 2>${TESTS[$i]}.error \
				&& printf $PASSED || printf $FAILED
		printf "\n"
done