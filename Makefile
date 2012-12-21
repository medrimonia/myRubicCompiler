CFLAGS= -std=c99 -g   # -g, -O3 , ... par exemple
LDFLAGS=-ll -ly # les bibliotheques necessaires
CC=gcc

all: rubic

lex.yy.c: scanner.l
	lex $<
y.tab.c: parse.y
	yacc -d $<
y.tab.o: y.tab.c y.tab.h
	$(CC) $(CFLAGS) -c $<
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $<
rubic: y.tab.o lex.yy.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.s: %.ll
	llc $<

hello_world: hello_world.s
	$(CC) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

hashmap.o: hashmap.h

test_hashmap.o: hashmap.h

test_hashmap: hashmap.o test_hashmap.o
	$(CC) -o $@ $^

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