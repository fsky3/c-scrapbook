GCC_FLAGS = -Wall
CRITERION_PATH = /usr/include/criterion/

all:
	gcc -O2 -I$(CRITERION_PATH) $(GCC_FLAGS) *.c -lcriterion -o "test"
	./test --verbose
