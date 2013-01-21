#include <stdlib.h>

#include "function_set.h"

function_set_p global_fs = NULL;

void initialize_global_function_set(){
	global_fs = new_function_set();
}

function_set_p new_function_set(){
	return new_hashmap(&prototype_hash_function, &prototype_equal);
}

function_p function_set_get(function_set_p fs, prototype_p p){
	return (function_p)hashmap_get(fs, p);
}

bool function_set_exists(function_set_p fs, prototype_p p){
	return hashmap_exists(fs,p);
}

function_p function_set_add(function_set_p fs, prototype_p p, function_p f){
	return (function_p)hashmap_add(fs, p, f);
}

void function_set_remove(function_set_p fs,
												 prototype_p key,
												 bool free_prototype,
												 bool free_function){
	//TODO handle free
	hashmap_remove(fs, key, free_prototype, free_function);
}

int function_set_size(function_set_p fs){
	return hashmap_size(fs);
}

void function_set_destroy(function_set_p fs,
													bool free_keys,
													bool free_data){
	hashmap_destroy(fs, free_keys, free_data);
}


void function_set_start_iteration(function_set_p fs){
	hashmap_start_iteration(fs);
}

prototype_p function_set_get_current_key(function_set_p fs){
	return (prototype_p)hashmap_get_current_key(fs);
}

function_p function_set_get_current_value(function_set_p fs){
	return (function_p)hashmap_get_current_value(fs);
}

void function_set_next_element(function_set_p fs){
	hashmap_next_element(fs);
}

bool function_set_is_ended_iteration(function_set_p fs){
	return hashmap_is_ended_iteration(fs);
}
