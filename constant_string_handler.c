#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constant_string_handler.h"

#define MAX_CONSTANTS 100


const char ** constants = NULL;
int nb_constants = 0;


int add_constant(const char * s){
	if (constants == NULL) //lazy mechanism
		constants = malloc(MAX_CONSTANTS * sizeof(char *));
	//TODO maybe optimized to avoid constant duplication
	constants[nb_constants++] = s;
	return nb_constants - 1;
}

void print_constants(){
	int i;
	for (i = 0; i < nb_constants; i++){
		printf("@%d = internal constant [20 x i8] c\"%s\\00\"\n",
					 i,
					 constants[i]);
	}
}
