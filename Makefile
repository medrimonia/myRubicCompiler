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
AR_FLAGS = --exclude-vcs -zcf
archive: mrproper report.pdf $(AR_SOURCE)
	$(AR) $(AR_FLAGS) hofer.tar.gz $(AR_SOURCE)

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

# GENERIC RULES

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.s: %.ll
	-llc $<

%.ll: %.rubic rubic
	./rubic <$< >$@
%.test : %.s
	$(CC) -o $@ $<

# EXECUTABLE RULES
rubic: y.tab.o lex.yy.o context.o dictionnary.o hashmap.o linked_list.o    \
			 code_generator.o tree.o function.o type_handler.o type.o variable.o \
			 constant_string_handler.o doubly_linked_list.o                      \
			 possible_types_solver.o validation.o prototype.o function_set.o     \
			 type_updater.o
	$(CC) -o $@ $^ $(LDFLAGS)

test_doubly_linked_list: doubly_linked_list.o test_doubly_linked_list.o
	$(CC) -o $@ $^

test_hashmap: hashmap.o test_hashmap.o linked_list.o
	$(CC) -o $@ $^

test_linked_list: linked_list.o test_linked_list.o
	$(CC) -o $@ $^


# OBJECT RULES

code_generator.o : code_generator.h constant_string_handler.h function.h \
									 prototype.h type.h type_handler.h type_updater.h \
									 validation.h variable.h

constant_string_handler.o : constant_string_handler.h

context.o : context.h variable.h type_handler.h

dictionnary.o : dictionnary.h

doubly_linked_list.o: doubly_linked_list.h

function.o : function.h code_generator.h dictionnary.h function_set.h \
					   prototype.h type_handler.h

function_set.o : function_set.h

hashmap.o: hashmap.h

linked_list.o: linked_list.h

possible_types_solver.o: possible_types_solver.h type.h type_handler.h

prototype.o : prototype.h dictionnary.h type.h type_handler.h variable.h

test_doubly_linked_list.o: doubly_linked_list.h

test_hashmap.o: hashmap.h

test_linked_list.o: linked_list.h

tree.o : tree.h function.h type_handler.h

type.o : type.h

type_handler.o : type_handler.h

type_updater.o : type_updater.h function.h function_set.h linked_list.h

validation.o : validation.h function_set.h prototype.h type.h type_handler.h \
							 type_updater.h

variable.o : variable.h

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

code_generator.h : tree.h

context.h : dictionnary.h variable.h

dictionnary.h : hashmap.h

function.h : context.h linked_list.h tree.h

function_set.h : dictionnary.h function.h prototype.h

hashmap.h : linked_list.h 

possible_types_solver.h : doubly_linked_list.h function.h

prototype.h : linked_list.h type.h

tree.h : context.h linked_list.h

type.h : linked_list.h

type_handler.h : linked_list.h type.h

type_updater.h : function.h linked_list.h prototype.h

validation.h : function.h linked_list.h possible_types_solver.h

variable.h : linked_list.h

### CLEANING PART ###


.PHONY: clean mrproper

clean:
	rm -rf *.o *.s lex.yy.c y.tab.c y.tab.h y.output $(RUBICS_OBJS) \
		$(RUBICS_TESTS_OUTPUT) $(RUBICS_TESTS_ERROR) report.aux report.dvi \
		report.log hofer.tar.gz

mrproper: clean
	rm -rf $(EXECUTABLES) *~ report.pdf