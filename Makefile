CFLAGS= -std=c99 -g -Wall -Wextra # -g, -O3 , ... par exemple
LDFLAGS=-ll -ly # les bibliotheques necessaires
CC=gcc
YACCFLAGS= #--debug --verbose #used to debug grammar issues

all: rubic

# LEX AND YACC PART

lex.yy.c: scanner.l
	lex $<
y.tab.c: parse.y tree.h linked_list.h
	yacc $(YACCFLAGS) -d $<
y.tab.o: y.tab.c y.tab.h context.h tree.h code_generator.h
	$(CC) $(CFLAGS) -c $<
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $<
rubic: y.tab.o lex.yy.o context.o dictionnary.o hashmap.o linked_list.o \
			 code_generator.o tree.o function.o type_handler.o type.o variable.o \
			 constant_string_handler.o
	$(CC) -o $@ $^ $(LDFLAGS)

# GENERIC RULES

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.s: %.ll
	llc $<

%.ll: %.rubic rubic
	./rubic <$< >$@

# SPECIFIC RULES

test_appel : test_appel.s
	$(CC) -o $@ $<

test: test.s
	$(CC) -o $@ $<

hello_world: hello_world.s
	$(CC) -o $@ $<

function.o : function.h

tree.o : tree.h

context.o : dictionnary.h

context.o : context.h

code_generator.o : code_generator.h

dictionnary.o : dictionnary.h

hashmap.o: hashmap.h linked_list.h

test_hashmap.o: hashmap.h

test_hashmap: hashmap.o test_hashmap.o linked_list.o
	$(CC) -o $@ $^

linked_list.o: linked_list.h

test_linked_list.o: linked_list.h

type.o : type.h

variable.o : variable.h

type_handler.o : type_handler.h

constant_string_handler.o : constant_string_handler.h

test_linked_list: linked_list.o test_linked_list.o
	$(CC) -o $@ $^

EXECUTABLES=       \
	hello_world      \
	rubic	           \
	test_hashmap     \
	test_linked_list \
	test             \
	test_appel


### .h DEPENDENCIES

type_handler.h : type.h linked_list.h

type.h : linked_list.h

tree.h : linked_list.h

### CLEANING PART ###


.PHONY: clean mrproper

clean:
	rm -rf *.o *.s lex.yy.c y.tab.c y.tab.h y.output test.ll compile_information

mrproper: clean
	rm -rf $(EXECUTABLES)