#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"


#define NB_PRIMES 4

int primes[] = {3,5,7,11};

struct test{
	char * name;
	int n;
};

int string_hash_function(void * key){
	char * key_str = (char *) key;
	int sum = 0;
	int i = 0;
	while (key_str[i] != '\0'){
		sum += key_str[i] * primes[i % NB_PRIMES];
		i++;
	}
	return sum;	
}

int str_equal_function(void * key_1, void * key_2){
	return strcmp((char *) key_1, (char *) key_2) == 0;
}

int main(int argc, char ** argv){
	struct test * myStruct = malloc(sizeof(struct test));
	myStruct->name = "abracadabra";
	myStruct->n = 1;
	struct test * myStruct2 = malloc(sizeof(struct test));
	myStruct2->name = "plouf";
	myStruct2->n = 2;
	struct test * myStruct3 = malloc(sizeof(struct test));
	myStruct3->name = "alakazam";
	myStruct3->n = 3;
	hashmap_pointer myMap = new_hash_map(string_hash_function,
																			 str_equal_function);
	hash_map_add(myMap, myStruct->name, myStruct);
	hash_map_add(myMap, myStruct2->name, myStruct2);
	hash_map_add(myMap, myStruct3->name, myStruct3);
	struct test * getted_struct = hash_map_get(myMap, myStruct->name);
	struct test * getted_struct2 = hash_map_get(myMap, myStruct2->name);
	struct test * getted_struct3 = hash_map_get(myMap, myStruct3->name);
	struct test * invalid = hash_map_get(myMap,"inexistant");
	printf("Received struct : name = '%s', n = '%d'\n",
				 getted_struct->name,
				 getted_struct->n);
	printf("Received struct : name = '%s', n = '%d'\n",
				 getted_struct2->name,
				 getted_struct2->n);
	printf("Received struct : name = '%s', n = '%d'\n",
				 getted_struct3->name,
				 getted_struct3->n);
	printf("Is invalid working ? %d\n", invalid == NULL &&
				 !hash_map_exists(myMap,"inexistant"));

	hashmap_destroy(myMap,false,true);

}
