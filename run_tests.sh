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

# TESTING RETURN STATUS

printf "\033[1m${NB_TESTS_COMPILED} / ${NB_TESTS} tests compiling\033[0m\n"

printf "\033[25C\033[1m RUNNING TESTS \033[0m\n"

NB_TESTS_OK=0

for ((i = 0; i < ${#TESTS[@]} ; i++))
do
		printf "${TESTS[$i]}\033[80D\033[60C" 
		./${TESTS[$i]} >${TESTS[$i]}.output 2>${TESTS[$i]}.error
		if [[ $? -ne 0 ]]
		then
				printf $FAILED
		else
				NB_TESTS_OK=$((NB_TESTS_OK+1))
				printf $PASSED
		fi
		printf "\n"
done

printf "\033[1m${NB_TESTS_OK} / ${NB_TESTS} tests ok\033[0m\n"

#CHECKING MEMORY_LEAKS
printf "\033[25C\033[1m CHECKING MEMORY LEAKS \033[0m\n"

NB_MEMORY_OK=0

for ((i = 0; i < ${#RUBICS_FILES[@]} ; i++))
do
		VALGRIND_OUT=${RUBICS_FILES[i]%.rubic}.valgrind.output
		VALGRIND_ERR=${RUBICS_FILES[i]%.rubic}.valgrind.error
		valgrind ./rubic <${RUBICS_FILES[i]} >${VALGRIND_OUT} 2>${VALGRIND_ERR}
		if grep -Fq "All heap blocks were freed" ${VALGRIND_ERR}
		then
				NB_MEMORY_OK=$((NB_MEMORY_OK+1))
				printf "\e[1;32m"
		else
				printf "\e[1;31m"
		fi
		printf "\033[1m${RUBICS_FILES[i]}:\033[0m\n"
		grep "total heap usage" $VALGRIND_ERR
		printf "\e[0m\n"
done

printf "\033[1m${NB_MEMORY_OK} / ${NB_TESTS} tests without memory leaks\033[0m\n"