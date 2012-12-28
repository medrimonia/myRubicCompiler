#include <stdlib.h>
#include <stdio.h>

#include "code_generator.h"


int generate_code(tn_pointer node){
	if (node->left_child != NULL)
		generate_code(node->left_child);
	printf("On node of type : %d\n", node->type);
	if (node->right_child != NULL)
		generate_code(node->right_child);
	if (node->content != NULL)
		printf("node_content : %s\n", (char *) node->content);
	return 0;	
}
