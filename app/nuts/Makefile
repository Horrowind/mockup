SHELL := /bin/bash

all:
	gcc -g3 -O0 -mno-avx -Werror -I../../lib nuts.c -o nuts

macro_debug:
	gcc -g -E -P -Werror -I../../lib nuts.c | clang-format > nuts.pp.c
	gcc -g -O0 -mno-avx -Werror -fpreprocessed -I../../lib nuts.pp.c -o nuts

release:
	gcc -g -O3 -Werror -march=native -I../../lib nuts.c -o nuts

profile:
	gcc -g -fprofile-generate -O3 -Werror  -I../../lib nuts.c -o nuts

profile_use:
	gcc -g -fprofile-use -O3 -Werror -I../../lib nuts.c -o nuts
