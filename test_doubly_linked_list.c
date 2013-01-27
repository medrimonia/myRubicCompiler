#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doubly_linked_list.h"

#define ELEMENT1 "ELEMENT 1"
#define ELEMENT2 "ELEMENT 2"
#define ELEMENT3 "ELEMENT 3"


struct test{
  char * name;
  int n;
};

void test_new_doubly_linked_list(){
  doubly_linked_list_pointer myList = new_doubly_linked_list();
  assert(doubly_linked_list_size(myList) == 0);
  doubly_linked_list_destroy(myList);
}

void test_doubly_linked_list_insert(){
  char * got_element;
  doubly_linked_list_pointer myList = new_doubly_linked_list();
  char * element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
  // Inserting in an empty array
  doubly_linked_list_insert(myList, strcpy(element1,ELEMENT1));
  got_element = doubly_linked_list_get(myList);
  assert(got_element == element1);
  // Inserting before the first element
  char * element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
  doubly_linked_list_insert(myList, strcpy(element2,ELEMENT2));
  doubly_linked_list_previous(myList);
  got_element = doubly_linked_list_get(myList);
  // Here, doubly_linked_list_insert has a pointer issue
  // (still pointing on the same element)
  assert(got_element == element2);
  doubly_linked_list_next(myList);
  got_element = doubly_linked_list_get(myList);
  assert(got_element == element1);
  doubly_linked_list_destroy(myList);
}
void test_doubly_linked_list_insert_in_head(){
  char * got_element;
  doubly_linked_list_pointer myList = new_doubly_linked_list();
  char * element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
  // Inserting in an empty array
  doubly_linked_list_insert_in_head(myList, strcpy(element1,ELEMENT1));
  got_element = doubly_linked_list_get(myList);
  assert(got_element == element1);
  // Inserting in head, while on head
  char * element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
  doubly_linked_list_insert_in_head(myList, strcpy(element2,ELEMENT2));
  got_element = doubly_linked_list_get(myList);
  assert(got_element == element1);
  doubly_linked_list_first(myList);
  got_element = doubly_linked_list_get(myList);
  assert(got_element == element2);
  // Inserting while on the second element
  doubly_linked_list_next(myList);
  got_element = doubly_linked_list_get(myList);
  assert(got_element == element1);
  char * element3 = malloc((1 + strlen(ELEMENT3)) * sizeof(char));
  doubly_linked_list_insert_in_head(myList, strcpy(element3,ELEMENT3));
  // element should be the same as before
  got_element = doubly_linked_list_get(myList);
  assert(got_element == element1);
  // checking order
  doubly_linked_list_first(myList);
  got_element = doubly_linked_list_get(myList);
  assert(got_element == element3);
  doubly_linked_list_next(myList);
  got_element = doubly_linked_list_get(myList);
  assert(got_element == element2);
  doubly_linked_list_next(myList);
  got_element = doubly_linked_list_get(myList);
  assert(got_element == element1);
  doubly_linked_list_destroy(myList);
}

void test_doubly_linked_list_end(){
  doubly_linked_list_pointer myList = new_doubly_linked_list();
  assert(doubly_linked_list_is_last(myList));
  char * element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
  // Inserting in an empty array
  doubly_linked_list_insert(myList, strcpy(element1,ELEMENT1));
  assert(doubly_linked_list_is_last(myList));
  char * element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
  // Inserting in an empty array
  doubly_linked_list_insert(myList, strcpy(element2,ELEMENT2));
  doubly_linked_list_previous(myList);
  assert(!doubly_linked_list_is_last(myList));
  doubly_linked_list_next(myList);
  assert(doubly_linked_list_is_last(myList));
  doubly_linked_list_destroy(myList);
}

void test_doubly_linked_list_remove(){
  doubly_linked_list_pointer myList = new_doubly_linked_list();
  char * element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
  doubly_linked_list_insert(myList, strcpy(element1,ELEMENT1));
  char * element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
  doubly_linked_list_insert(myList, strcpy(element2,ELEMENT2));
  doubly_linked_list_previous(myList);
  char * element3 = malloc((1 + strlen(ELEMENT3)) * sizeof(char));
  doubly_linked_list_insert(myList, strcpy(element3,ELEMENT3));
  doubly_linked_list_previous(myList);
  
  assert(doubly_linked_list_get(myList) == element3);
  doubly_linked_list_remove(myList);
  assert(doubly_linked_list_get(myList) == element2);
  doubly_linked_list_remove(myList);
  assert(doubly_linked_list_get(myList) == element1);
  doubly_linked_list_remove(myList);
  assert(doubly_linked_list_is_empty(myList));
  doubly_linked_list_destroy(myList);
  // Second try
  myList = new_doubly_linked_list();
  element1 = malloc((1 + strlen(ELEMENT1)) * sizeof(char));
  doubly_linked_list_insert(myList, strcpy(element1,ELEMENT1));
  doubly_linked_list_previous(myList);
  element2 = malloc((1 + strlen(ELEMENT2)) * sizeof(char));
  doubly_linked_list_insert(myList, strcpy(element2,ELEMENT2));
  doubly_linked_list_previous(myList);
  element3 = malloc((1 + strlen(ELEMENT3)) * sizeof(char));
  doubly_linked_list_insert(myList, strcpy(element3,ELEMENT3));
  doubly_linked_list_previous(myList);
  doubly_linked_list_next(myList);
  assert(doubly_linked_list_get(myList) == element2);
  doubly_linked_list_remove(myList);
  assert(doubly_linked_list_get(myList) == element1);
  doubly_linked_list_remove(myList);
  assert(doubly_linked_list_get(myList) == element3);
  doubly_linked_list_remove(myList);
  assert(doubly_linked_list_is_empty(myList));
  doubly_linked_list_destroy(myList); 
}

int main(int argc, char ** argv){
  test_new_doubly_linked_list();
  test_doubly_linked_list_insert();
  test_doubly_linked_list_insert_in_head();
  test_doubly_linked_list_end();
  test_doubly_linked_list_remove();
}
