CFLAGS= -std=c99 -g -Wall -Wextra # -g, -O3 , ... par exemple
LDFLAGS=-ll -ly # les bibliotheques necessaires
CC=gcc
YACCFLAGS= #--debug --verbose #used to debug grammar issues

all: rubic

lex.yy.c: scanner.l
	lex $<
y.tab.c: parse.y tree.h
	yacc $(YACCFLAGS) -d $<
y.tab.o: y.tab.c y.tab.h context.h tree.h code_generator.h
	$(CC) $(CFLAGS) -c $<
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $<
rubic: y.tab.o lex.yy.o context.o dictionnary.o hashmap.o linked_list.o code_generator.o tree.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.s: %.ll
	llc $<

test.ll: test.txt rubic
	./rubic <test.txt >test.ll

test: test.s
	$(CC) -o $@ $<

hello_world: hello_world.s
	$(CC) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

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

test_linked_list: linked_list.o test_linked_list.o
	$(CC) -o $@ $^

EXECUTABLES= \
	hello_world \
	rubic	\
	test_hashmap \
	test_linked_list

.PHONY: clean mrproper

clean:
	rm -rf *.o *.s lex.yy.c y.tab.c y.tab.h

mrproper: clean
	rm -rf $(EXECUTABLES)