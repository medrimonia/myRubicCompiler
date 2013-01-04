%code top{
	#include <stdlib.h>
  #include <stdio.h>
	#include <string.h>
  #include "y.tab.h"
	#include "context.h"
	#include "tree.h"
	#include "code_generator.h"
	#include "function.h"
	#include "type.h"
	#include "type_handler.h"
	#include "constant_string_handler.h"

	// Declaring functions in order to avoid warnings
	int yylex(void);
	int yyerror(char *);
	char *strndup(const char *s, size_t n);
	
	context_pointer global_context;
	context_pointer actual_context;

	tn_pointer global_root;

	int yydebug = 1;

}
%code requires{
	#include <assert.h>
  #include "tree.h"
  #include "linked_list.h"
}
%union{
	int i;
	float f;
	char * s;
	tn_pointer node;
	linked_list_pointer l;
 }

%token <s> STRING


%token <s> ID
%token <f> FLOAT
%token <i> INT

%token AND OR CLASS IF THEN ELSE END WHILE DO DEF LEQ GEQ 
%token FOR TO RETURN IN NEQ
%left '*' 
%left '/'
%left '+' '-'
%left '<' '>' LEQ GEQ EQ
%left AND OR

%type <node> lhs
%type <node> expr
%type <node> primary
%type <node> stmt
%type <node> comp_expr
%type <node> additive_expr
%type <node> multiplicative_expr
%type <node> topstmts
%type <node> topstmt
%type <node> stmts
%type <l> opt_params
%type <l> params
%type <l> exprs

%%

program		:  topstmts opt_terms
{
	global_root = $1;
}
;
topstmts        :      {$$ = NULL;}
| topstmt { $$ = $1;}
| topstmts terms topstmt
{
	$$ = (tn_pointer)malloc(sizeof(struct tree_node));
	$$->left_child = $1;
	$$->right_child = $3;
	$$->type = LIST;
}
;
topstmt	        : CLASS ID term stmts END
{
	printf("Not implemented part\n");
	exit(EXIT_FAILURE);
}
                | CLASS ID '<' ID term stmts END
{
	printf("Not implemented part\n");
	exit(EXIT_FAILURE);
}
| stmt { $$ = $1;}
;

stmts	        : /* none */ {$$ = NULL;}
                | stmt
{
	$$ = $1;
}
                | stmts terms stmt
{
	$$ = (tn_pointer)malloc(sizeof(struct tree_node));
	$$->left_child = $1;
	$$->right_child = $3;
	$$->type = LIST;
}
                ;

stmt		: IF expr THEN stmts terms END
{
	printf("Not implemented part\n");
	exit(EXIT_FAILURE);
}
                | IF expr THEN stmts terms ELSE stmts terms END
								{
	printf("Not implemented part\n");
	exit(EXIT_FAILURE);
								}

                | FOR ID IN expr TO expr term stmts terms END
								{
	printf("Not implemented part\n");
	exit(EXIT_FAILURE);
								}
                | WHILE expr DO term stmts terms END 
								{
	printf("Not implemented part\n");
	exit(EXIT_FAILURE);
								}
                | lhs '=' expr
{
	if ($1->type == CALL){
		fprintf(stderr, "Can't affect something to a function\n");
		exit(EXIT_FAILURE);
	}
	if (!is_declared_variable(actual_context,$1->content)){
		//printf("Declaring a variable named '%s'.\n", (char *) $1->content);
		declare_variable(actual_context,$1->content);
	}
	else{
		// variable won't be added to the dictionnary, access to it will be lost
		// free might be needed
	}
	$$ = (tn_pointer)malloc(sizeof(struct tree_node));
	$$->left_child = $1;
	$$->right_child = $3;
	$$->type = AFFECT;
	
}
                | RETURN expr
{
	$$ = new_tree_node(RETURN_NODE);
	$$->left_child = $2;
}
                | DEF ID opt_params
								{ // Context switch is needed before term parsing
									actual_context = create_context_child(actual_context);
								}
								term stmts[code] terms END
								{
									function_p f = new_function(actual_context);
									f->name = $2;
									f->parameters = $opt_params;
									add_function_to_context(f, actual_context->parent_context);
									f->root = $code;
									$$ = new_tree_node(FUNCTION);
									$$->content = f;
								}
; 

opt_params      : /* none */ { $$ = NULL;}
                | '(' ')' { $$ = NULL;}
                | '(' params ')' {$$ = $2;}
;
params          : ID ',' params
{
	linked_list_insert($3, $ID);
	$$ = $3;
}
                | ID
{
	$$ = new_linked_list();
	linked_list_insert($$, $ID);
}
; 
lhs             : ID
{
	$$ = new_tree_node(IDENTIFIER);
	$$->content = $1;
}
                | ID '.' primary
								{
	printf("Not implemented part\n");
	exit(EXIT_FAILURE);
								}
                | ID '(' exprs ')'
								{
									$$ = new_tree_node(CALL);
									$$->content = new_function_call($ID);
									((function_call_p)$$->content)->parameters = $3;
								}
;
exprs           : exprs ',' expr
{
	linked_list_insert($1, $3);
	$$ = $1;
}
                | expr
{
	$$ = new_linked_list();
	linked_list_insert($$, $1);
}
;
primary         : lhs
                | STRING
{
	int n = add_constant(strndup($1 + 1, strlen($1) - 2));
	$$ = new_tree_node(PRIMARY);
	primary_p value = malloc(sizeof(struct primary));
	value->t = PRIMARY_STRING;
	value->s_id = n;
	$$->content = value;
}
                | FLOAT
{
	$$ = new_tree_node(PRIMARY);
	primary_p value = malloc(sizeof(struct primary));
	value->t = PRIMARY_FLOAT;
	value->f = $1;
	$$->content = value;
}
                | INT
{
	$$ = new_tree_node(PRIMARY);
	primary_p value = malloc(sizeof(struct primary));
	value->t = PRIMARY_INT;
	value->i = $1;
	$$->content = value;
}
                | '(' expr ')'
								{
	printf("Not implemented part\n");
	exit(EXIT_FAILURE);
								}
;
expr            : expr AND comp_expr
                | expr OR comp_expr
                | comp_expr { $$ = $1;}
;
comp_expr       : additive_expr '<' additive_expr
                | additive_expr '>' additive_expr
                | additive_expr LEQ additive_expr
                | additive_expr GEQ additive_expr
                | additive_expr EQ additive_expr
                | additive_expr NEQ additive_expr
                | additive_expr { $$ = $1;}
;
additive_expr   : multiplicative_expr { $$ = $1;}
                | additive_expr '+' multiplicative_expr
{
	$$ = new_tree_node(ADDITION);
	$$->left_child = $1;
	$$->right_child = $3;
}
                | additive_expr '-' multiplicative_expr
;
multiplicative_expr : multiplicative_expr '*' primary
{
	$$ = new_tree_node(MULTIPLY);
	$$->left_child = $1;
	$$->right_child = $3;
}
                    | multiplicative_expr '/' primary
{
	$$ = new_tree_node(DIVIDE);
	$$->left_child = $1;
	$$->right_child = $3;
}
                    | primary
{
	$$ = $1;
}
;
opt_terms	: /* none */
            | terms
						;

terms		: terms ';'
          | terms '\n'
          | ';'
          | '\n'
          ;
term            : ';'
                | '\n'
                ;
%%

int main() {
	initialize_types();//add basic types to all_types
	global_context = new_context();
	actual_context = global_context;
  yyparse();
	//printf("declare i32 @puts(i32*)\n"); //TODO Hack
	print_constants();
	generate_code(global_root);
	destroy_context(global_context);
  return 0;
}
