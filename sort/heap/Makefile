GCC_FLAGS = -Wall
CRITERION_PATH = /usr/include/criterion/

all:
	gcc -I$(CRITERION_PATH) $(GCC_FLAGS) *.c -lcriterion -o "heap"
	./heap --verbose

no_test:
	gcc -DNO_TEST $(GCC_FLAGS) *.c -o "heap"
