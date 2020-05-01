GCC_FLAGS = -Wall
CRITERION_PATH = /usr/include/criterion/

all:
	gcc -I$(CRITERION_PATH) $(GCC_FLAGS) *.c ./heap/heap.c -lcriterion -o "mysort"

no_test:
	gcc -DNO_TEST $(GCC_FLAGS) *.c ./heap/heap.c -o "mysort"
