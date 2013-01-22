#include <stdlib.h>

#include "function_set.h"

function_set_p global_fs = NULL;

void initialize_global_function_set(){
	global_fs = new_function_set();
}

function_set_p new_function_set(){
	return new_dictionnary();
}

function_p function_set_get(function_set_p fs, prototype_p p){
	hashmap_pointer hm = (hashmap_pointer)dictionnary_get(fs, p->name);
	if (hm == NULL)
		return NULL;
	return (function_p)hashmap_get(hm, p);
}

bool function_set_exists(function_set_p fs, prototype_p p){
	hashmap_pointer hm = (hashmap_pointer)dictionnary_get(fs, p->name);
	if (hm == NULL)
		return false;
	return hashmap_exists(hm,p);
}

function_p function_set_add(function_set_p fs, prototype_p p, function_p f){
	hashmap_pointer hm = (hashmap_pointer)dictionnary_get(fs, p->name);
	if (hm == NULL){
		hm = new_hashmap(&prototype_hash_function,&prototype_equal);
		dictionnary_add(fs, p->name, hm);
	}
	return (function_p)hashmap_add(hm, p, f);
}

void function_set_remove(function_set_p fs,
												 prototype_p key,
												 bool free_prototype,
												 bool free_function){
	//TODO handle free
	hashmap_pointer hm = (hashmap_pointer)dictionnary_get(fs, key->name);
	if (hm == NULL)
		return;
	hashmap_remove(hm, key, free_prototype, free_function);
	// empty hashmaps shouldn't be keeped
	if (hashmap_size(hm) == 0){
		hashmap_destroy(hm, false, false);
		dictionnary_remove(fs, key->name, false, false);
	}
}

int function_set_size(function_set_p fs){
	return dictionnary_size(fs);
}

void function_set_destroy(function_set_p fs,
													bool free_keys,
													bool free_data){
	if (dictionnary_size(fs) > 0){
		dictionnary_start_iteration(fs);
		while(true){
			hashmap_pointer hm = dictionnary_get_current_value(fs);
			hashmap_destroy(hm, free_keys, free_data);
			if (dictionnary_is_ended_iteration(fs))
				break;
			dictionnary_next_element(fs);
		}
	}
	
	dictionnary_destroy(fs, free_keys, free_data);
}


void function_set_start_iteration(function_set_p fs){
	dictionnary_start_iteration(fs);
	hashmap_pointer hm = dictionnary_get_current_value(fs);
	hashmap_start_iteration(hm);
}

prototype_p function_set_get_current_key(function_set_p fs){
	hashmap_pointer hm = dictionnary_get_current_value(fs);
	return (prototype_p)hashmap_get_current_key(hm);
}

function_p function_set_get_current_value(function_set_p fs){
	hashmap_pointer hm = dictionnary_get_current_value(fs);
	return (function_p)hashmap_get_current_value(hm);
}

void function_set_next_element(function_set_p fs){
	hashmap_pointer hm = dictionnary_get_current_value(fs);
	if (hashmap_is_ended_iteration(hm))
		dictionnary_next_element(fs);
	else
		hashmap_next_element(hm);
}

bool function_set_is_ended_iteration(function_set_p fs){
	hashmap_pointer hm = dictionnary_get_current_value(fs);
	return (dictionnary_is_ended_iteration(fs) &&
					hashmap_is_ended_iteration(hm));
}

// if key = false, then values are added to the list
linked_list_pointer function_set_matching(function_set_p fs,
																					const char * name,
																					linked_list_pointer l,
																					bool key){
	linked_list_pointer result = new_linked_list();
	hashmap_pointer hm = (hashmap_pointer)dictionnary_get(fs, name);
	if (hm == NULL)//no functions with the specified name
		return result;
	hashmap_start_iteration(hm);
	while(!hashmap_is_ended_iteration(hm)){
		prototype_p p = (prototype_p)hashmap_get_current_key(hm);
		if (prototype_matches(p, l)){
			if (key)
				linked_list_insert(result, p);
			else{
				function_p to_add = (function_p)hashmap_get_current_value(hm);
				linked_list_insert(result, to_add);
			}
		}
		if (hashmap_is_ended_iteration(hm))
			break;
		hashmap_next_element(hm);
	}
	return  result;
}

linked_list_pointer function_set_matching_functions(function_set_p fs,
																										const char * name,
																										linked_list_pointer l){
	return function_set_matching(fs, name, l, false);
}

linked_list_pointer function_set_matching_prototypes(function_set_p fs,
																										 const char * name,
																										 linked_list_pointer l){
	return function_set_matching(fs, name, l, true);
}
