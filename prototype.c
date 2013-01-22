#include <stdlib.h>
#include <string.h>

#include "prototype.h"
#include "dictionnary.h"//for string_hash_function
#include "type.h"
#include "type_handler.h"
#include "variable.h"

#define NB_PRIMES 4

int proto_primes[] = {3,5,7,11};

prototype_p new_prototype(const char * name,
													linked_list_pointer params){
	prototype_p new = (prototype_p)malloc(sizeof(struct prototype));
	new->name = name;
	new->params = params;
	return new;
}

int prototype_hash_function(const void * key){
	prototype_p p = (prototype_p) key;
	int hash = string_hash_function(p->name);
	hash += linked_list_size(p->params) * proto_primes[0];
	if (linked_list_is_empty(p->params))
		return hash;
	int i = 1;
	linked_list_restart(p->params);
	while(true){
		type_p t = linked_list_get(p->params);
		hash += proto_primes[i] * type_associated_int(t);
		i = (i + 1) % NB_PRIMES;
		if (linked_list_end(p->params))
			return hash;
		linked_list_next(p->params);
	}
}

bool prototype_equal(const void * key_1, const void * key_2){
	prototype_p p1 = (prototype_p)key_1;
	prototype_p p2 = (prototype_p)key_2;
	if (strcmp(p1->name, p2->name) != 0 ||
			linked_list_size(p1->params) != linked_list_size(p2->params))
		return false;
	// Here function have the same number of parameters
	if (linked_list_is_empty(p1->params))
		return true;
	// Compare parameters
	linked_list_restart(p1->params);
	linked_list_restart(p2->params);
	while(true){
		if (linked_list_get(p1->params) != linked_list_get(p2->params))
			return false;
		if (linked_list_end(p1->params))
			return linked_list_end(p2->params);//equivalent to return true
		linked_list_next(p1->params);
		linked_list_next(p2->params);
	}
}

bool prototype_matches(prototype_p target, linked_list_pointer l){
	if (linked_list_size(target->params) != linked_list_size(l))
		return false;
	linked_list_restart(target->params);
	linked_list_restart(l);
	while(true){
		variable_p v = linked_list_get(target->params);
		type_p required = th_true_type(v->allowed_types);
		linked_list_pointer allowed = linked_list_get(l);
		if (!type_list_contains(allowed, required))
			return false;
		if (linked_list_end(target->params))
			return true;
		linked_list_next(target->params);
		linked_list_next(l);
	}
}
