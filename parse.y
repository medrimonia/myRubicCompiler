%{
  #include <stdio.h>
  #include "y.tab.h"
	#include "context.h"

	context_pointer global_context;
	context_pointer actual_context;

%}
%union{
	int i;
	char * s;
 }

%token AND OR CLASS IF THEN ELSE END WHILE DO DEF LEQ GEQ 
%token STRING FLOAT INT ID FOR TO RETURN IN NEQ
%left '*' 
%left '/'
%left '+' '-'
%left '<' '>' LEQ GEQ EQ
%left AND OR

%type <s> lhs

%%
program		:  topstmts opt_terms
;
topstmts        :      
| topstmt
| topstmts terms topstmt
;
topstmt	        : CLASS ID term stmts END 
                | CLASS ID '<' ID term stmts END
                | stmt
;

stmts	        : /* none */
                | stmt
                | stmts terms stmt
                ;

stmt		: IF expr THEN stmts terms END
                | IF expr THEN stmts terms ELSE stmts terms END 
                | FOR ID IN expr TO expr term stmts terms END
                | WHILE expr DO term stmts terms END 
                | lhs '=' expr
{
	if (!is_declared_global_variable(actual_context,$1)){
		printf("Declaring a variable named '%s'.\n", $1);
		declare_global_variable(actual_context,$1);
	}
	else{
		printf("Variable was already declared\n");
	}
}
                | RETURN expr
                | DEF ID opt_params term stmts terms END
; 

opt_params      : /* none */
                | '(' ')'
                | '(' params ')'
;
params          : ID ',' params
                | ID
; 
lhs             : ID
                | ID '.' primary
                | ID '(' exprs ')'
;
exprs           : exprs ',' expr
                | expr
;
primary         : lhs
                | STRING 
                | FLOAT
                | INT
                | '(' expr ')'
;
expr            : expr AND comp_expr
                | expr OR comp_expr
                | comp_expr
;
comp_expr       : additive_expr '<' additive_expr
                | additive_expr '>' additive_expr
                | additive_expr LEQ additive_expr
                | additive_expr GEQ additive_expr
                | additive_expr EQ additive_expr
                | additive_expr NEQ additive_expr
                | additive_expr
;
additive_expr   : multiplicative_expr
                | additive_expr '+' multiplicative_expr
                | additive_expr '-' multiplicative_expr
;
multiplicative_expr : multiplicative_expr '*' primary
                    | multiplicative_expr '/' primary
                    | primary
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
	global_context = new_context();
	actual_context = global_context;
  yyparse(); 
  return 0;
}
