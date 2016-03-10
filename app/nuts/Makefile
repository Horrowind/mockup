SHELL := /bin/bash

all:
	bison -dv nuts.y
	flex -f --header-file=flex.h --nodefault nuts.l
	gcc -O0 -g -std=gnu11 main.c nuts.tab.c lex.yy.c parse.c -I../../lib -o nuts
test:
	./test.sh
