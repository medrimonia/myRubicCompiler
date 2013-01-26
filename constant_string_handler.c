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

const char * get_constant(int id){
	if (id >= nb_constants)
		return NULL;
	return constants[id];
}

int get_constant_size(int id){
	if (id >= nb_constants)
		return -1;
	return strlen(constants[id]) + 1;
}

void print_constants(){
	int i;
	for (i = 0; i < nb_constants; i++){
		printf("@%d = internal constant [%d x i8] c\"%s\\00\"\n",
					 i,
					 get_constant_size(i),
					 constants[i]);
	}
}

void destroy_constants(){
	free(constants);
}
