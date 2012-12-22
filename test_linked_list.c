#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

#define ELEMENT1 "ELEMENT 1"
#define ELEMENT2 "ELEMENT 2"
#define ELEMENT3 "ELEMENT 3"


struct test{
	char * name;
	int n;
};

int test_new_linked_list(){
	linked_list_pointer myList = new_linked_list();
	assert(linked_list_size(myList) == 0);
	linked_list_destroy(myList);
}

int test_linked_list_insert(){
	char * got_element;
	linked_list_pointer myList = new_linked_list();
	char * element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
	// Inserting in an empty array
	linked_list_insert(myList, strcpy(element1,ELEMENT1));
	got_element = linked_list_get(myList);
	assert(got_element == element1);
	// Inserting before the first element
	char * element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
	linked_list_insert(myList, strcpy(element2,ELEMENT2));
	got_element = linked_list_get(myList);
	assert(got_element == element2);
	linked_list_next(myList);
	got_element = linked_list_get(myList);
	assert(got_element == element1);
	linked_list_destroy(myList);
}
int test_linked_list_insert_in_head(){
	char * got_element;
	linked_list_pointer myList = new_linked_list();
	char * element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
	// Inserting in an empty array
	linked_list_insert_in_head(myList, strcpy(element1,ELEMENT1));
	got_element = linked_list_get(myList);
	assert(got_element == element1);
	// Inserting in head, while on head
	char * element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
	linked_list_insert_in_head(myList, strcpy(element2,ELEMENT2));
	got_element = linked_list_get(myList);
	assert(got_element == element1);
	linked_list_restart(myList);
	got_element = linked_list_get(myList);
	assert(got_element == element2);
	// Inserting while on the second element
	linked_list_next(myList);
	got_element = linked_list_get(myList);
	assert(got_element == element1);
	char * element3 = malloc((1 + strlen(ELEMENT3)) * sizeof(char));
	linked_list_insert_in_head(myList, strcpy(element3,ELEMENT3));
	// element should be the same as before
	got_element = linked_list_get(myList);
	assert(got_element == element1);
	// checking order
	linked_list_restart(myList);
	got_element = linked_list_get(myList);
	assert(got_element == element3);
	linked_list_next(myList);
	got_element = linked_list_get(myList);
	assert(got_element == element2);
	linked_list_next(myList);
	got_element = linked_list_get(myList);
	assert(got_element == element1);
	linked_list_destroy(myList);
}

int test_linked_list_end(){
	char * got_element;
	linked_list_pointer myList = new_linked_list();
	assert(linked_list_end(myList));
	char * element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
	// Inserting in an empty array
	linked_list_insert(myList, strcpy(element1,ELEMENT1));
	assert(linked_list_end(myList));
	char * element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
	// Inserting in an empty array
	linked_list_insert(myList, strcpy(element2,ELEMENT2));
	assert(!linked_list_end(myList));
	linked_list_next(myList);
	assert(linked_list_end(myList));
	linked_list_destroy(myList);
}

int test_linked_list_remove_next(){
	char * got_element;
	linked_list_pointer myList = new_linked_list();
	char * element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
	linked_list_insert(myList, strcpy(element1,ELEMENT1));
	char * element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
	linked_list_insert(myList, strcpy(element2,ELEMENT2));
	char * element3 = malloc((1 + strlen(ELEMENT3)) * sizeof(char));
	linked_list_insert(myList, strcpy(element3,ELEMENT3));
	assert(linked_list_get(myList) == element3);
	linked_list_remove_next(myList);
	linked_list_next(myList);
	assert(linked_list_get(myList) == element1);
	linked_list_destroy(myList);	
}

int test_linked_list_remove(){
	char * got_element;
	linked_list_pointer myList = new_linked_list();
	char * element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
	linked_list_insert(myList, strcpy(element1,ELEMENT1));
	char * element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
	linked_list_insert(myList, strcpy(element2,ELEMENT2));
	char * element3 = malloc((1 + strlen(ELEMENT3)) * sizeof(char));
	linked_list_insert(myList, strcpy(element3,ELEMENT3));
	assert(linked_list_get(myList) == element3);
	linked_list_remove(myList);
	assert(linked_list_get(myList) == element2);
	linked_list_remove(myList);
	assert(linked_list_get(myList) == element1);
	linked_list_remove(myList);
	assert(linked_list_is_empty(myList));
	linked_list_destroy(myList);
	// Second try
	myList = new_linked_list();
	element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
	linked_list_insert(myList, strcpy(element1,ELEMENT1));
	element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
	linked_list_insert(myList, strcpy(element2,ELEMENT2));
	element3 = malloc((1 + strlen(ELEMENT3)) * sizeof(char));
	linked_list_insert(myList, strcpy(element3,ELEMENT3));
	linked_list_next(myList);
	assert(linked_list_get(myList) == element2);
	linked_list_remove(myList);
	assert(linked_list_get(myList) == element1);
	linked_list_remove(myList);
	assert(linked_list_get(myList) == element3);
	linked_list_remove(myList);
	assert(linked_list_is_empty(myList));
	linked_list_destroy(myList);	
}

int main(int argc, char ** argv){
	test_new_linked_list();
	test_linked_list_insert();
	test_linked_list_insert_in_head();
	test_linked_list_end();
	test_linked_list_remove_next();
	test_linked_list_remove();
}
