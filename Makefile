CFLAGS= -std=c99 -g -Wall -Wextra # -g, -O3 , ... par exemple
LDFLAGS=-ll -ly # les bibliotheques necessaires
CC=gcc
YACCFLAGS= #--debug --verbose #used to debug grammar issues

report.pdf: report.dvi
	dvipdf $< $@

report.dvi : report/report.tex
	latex $< -o $@

# CREATING ARCHIVE

AR_SOURCE = *.c               \
						*.h	              \
						scanner.l         \
		        parse.y           \
						report/report.tex \
            report.pdf        \
		        tests/*.rubic     \
						Makefile          \
	          README            \
					 	run_tests.sh

AR = tar
AR_FLAGS = --exclude-vcs -cf
archive: mrproper report.pdf $(AR_SOURCE)
	$(AR) $(AR_FLAGS) hofer.tar $(AR_SOURCE)

all: rubic

# LEX AND YACC PART

lex.yy.c: scanner.l
	lex $<
y.tab.c: parse.y tree.h linked_list.h lex.yy.c
	yacc $(YACCFLAGS) -d $<
y.tab.o: y.tab.c y.tab.h context.h tree.h code_generator.h validation.h
	$(CC) $(CFLAGS) -c $<
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $<
rubic: y.tab.o lex.yy.o context.o dictionnary.o hashmap.o linked_list.o \
			 code_generator.o tree.o function.o type_handler.o type.o variable.o \
			 constant_string_handler.o doubly_linked_list.o \
			 possible_types_solver.o validation.o prototype.o function_set.o \
			 type_updater.o
	$(CC) -o $@ $^ $(LDFLAGS)

# GENERIC RULES

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.s: %.ll
	-llc $<

%.ll: %.rubic rubic
	./rubic <$< >$@

# SPECIFIC RULES

%.test : %.s
	$(CC) -o $@ $<

function.o : code_generator.h dictionnary.h function.h function_set.h \
					   prototype.h type_handler.h

tree.o : tree.h type_handler.h

context.o : dictionnary.h

context.o : context.h

code_generator.o : code_generator.h constant_string_handler.h function.h \
									 prototype.h type.h type_handler.h type_updater.h \
									 validation.h variable.h

dictionnary.o : dictionnary.h

hashmap.o: hashmap.h linked_list.h

test_hashmap.o: hashmap.h

test_hashmap: hashmap.o test_hashmap.o linked_list.o
	$(CC) -o $@ $^

linked_list.o: linked_list.h

test_linked_list.o: linked_list.h

doubly_linked_list.o: doubly_linked_list.h

test_doubly_linked_list.o: doubly_linked_list.h

possible_types_solver.o: possible_types_solver.h type.h type_handler.h

type.o : type.h

variable.o : variable.h

type_handler.o : type_handler.h

constant_string_handler.o : constant_string_handler.h

validation.o : validation.h type_handler.h type.h type_updater.h \
							 prototype.h function_set.h

prototype.o : prototype.h dictionnary.h type.h type_handler.h variable.h

function_set.o : function_set.h

type_updater.o : type_updater.h function.h linked_list.h function_set.h

test_linked_list: linked_list.o test_linked_list.o
	$(CC) -o $@ $^

test_doubly_linked_list: doubly_linked_list.o test_doubly_linked_list.o
	$(CC) -o $@ $^

RUBICS_FILES=$(wildcard tests/*.rubic)
RUBICS_OBJS=$(RUBICS_FILES:.rubic=.ll)
RUBICS_TESTS=$(RUBICS_FILES:.rubic=.test)
RUBICS_TESTS_OUTPUT=$(RUBICS_FILES:.rubic=.test.output)
RUBICS_TESTS_ERROR=$(RUBICS_FILES:.rubic=.test.error)

tests : $(RUBICS_TESTS)

EXECUTABLES=              \
	hello_world             \
	rubic	                  \
	test_hashmap            \
	test_linked_list        \
	test_doubly_linked_list \
	$(RUBICS_TESTS)


### .h DEPENDENCIES

type_handler.h : type.h linked_list.h

type.h : linked_list.h

tree.h : linked_list.h

possible_types_solver.h : doubly_linked_list.h function.h

validation.h : function.h possible_types_solver.h linked_list.h

prototype.h : linked_list.h

function_set.h : prototype.h dictionnary.h function.h

type_updater.h : linked_list.h function.h prototype.h

### CLEANING PART ###


.PHONY: clean mrproper

clean:
	rm -rf *.o *.s lex.yy.c y.tab.c y.tab.h y.output $(RUBICS_OBJS) \
		$(RUBICS_TESTS_OUTPUT) $(RUBICS_TESTS_ERROR) report.aux report.dvi \
		report.log hofer.tar

mrproper: clean
	rm -rf $(EXECUTABLES) *~ report.pdf