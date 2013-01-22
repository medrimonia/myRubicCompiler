/* File name : dictionnary.c
 * Author : Medrimonia
 * created the : 2012/12/23
 * last update the : 2012/12/23
 *
 * Description :
 * This file contains an implementation of the dictionnary.h interface
 * by using an hashmap structure
 */

#include <string.h>

#include "dictionnary.h"

#define NB_PRIMES 4

int primes[] = {3,5,7,11};


int string_hash_function(const void * key){
	char * key_str = (char *) key;
	int sum = 0;
	int i = 0;
	while (key_str[i] != '\0'){
		sum += key_str[i] * primes[i % NB_PRIMES];
		i++;
	}
	return sum;	
}

bool str_equal_function(const void * key_1, const void * key_2){
	return strcmp((char *) key_1, (char *) key_2) == 0;
}

dictionnary_pointer new_dictionnary(){
	return new_hashmap(string_hash_function,
										 str_equal_function);
}

void * dictionnary_get(dictionnary_pointer d, const char * key){
	return hashmap_get(d, key);
}

bool dictionnary_exists(dictionnary_pointer d, const char * key){
	return hashmap_exists(d, key);
}

void * dictionnary_add(dictionnary_pointer d,
											 const char * key,
											 void * value){
	return hashmap_add(d, key, value);
}
void dictionnary_remove(dictionnary_pointer d,
												const char * key,
												bool free_key,
												bool free_data){
	hashmap_remove(d, key, free_key, free_data);
}

int dictionnary_size(dictionnary_pointer d){
	return hashmap_size(d);
}
void dictionnary_destroy(hashmap_pointer d,
												 bool free_keys,
												 bool free_data){
	hashmap_destroy(d, free_keys, free_data);
}


void dictionnary_start_iteration(dictionnary_pointer d){
	hashmap_start_iteration(d);
}

void * dictionnary_get_current_key(dictionnary_pointer d){
	return hashmap_get_current_key(d);
}

void * dictionnary_get_current_value(dictionnary_pointer d){
	return hashmap_get_current_value(d);
}

void dictionnary_next_element(dictionnary_pointer d){
	hashmap_next_element(d);
}

bool dictionnary_is_ended_iteration(dictionnary_pointer d){
	return hashmap_is_ended_iteration(d);
}
