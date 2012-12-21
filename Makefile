CFLAGS=   # -g, -O3 , ... par exemple
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
	gcc -o $@ $<

clean:
	rm -rf *.o *.s hello_world rubic lex.yy.c y.tab.c y.tab.h
