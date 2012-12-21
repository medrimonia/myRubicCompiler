#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"


struct test{
	char * name;
	int n;
};

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
	linked_list_pointer myList = new_linked_list();
	linked_list_insert_in_head(myList, myStruct);
	linked_list_insert_in_head(myList, myStruct2);
	linked_list_insert_in_head(myList, myStruct3);
	linked_list_restart(myList);
	struct test * getted_struct = linked_list_get(myList);
	linked_list_next(myList);
	struct test * getted_struct2 = linked_list_get(myList);
	linked_list_next(myList);
	struct test * getted_struct3 = linked_list_get(myList);
	printf("Received struct : name = '%s', n = '%d'\n",
				 getted_struct->name,
				 getted_struct->n);
	printf("Received struct : name = '%s', n = '%d'\n",
				 getted_struct2->name,
				 getted_struct2->n);
	printf("Received struct : name = '%s', n = '%d'\n",
				 getted_struct3->name,
				 getted_struct3->n);
	printf("Is end working ? %d\n", linked_list_end(myList));

}
