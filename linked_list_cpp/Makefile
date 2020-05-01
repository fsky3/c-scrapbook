GCC_FLAGS = -Wall
CRITERION_PATH = /usr/include/criterion/

all:
	g++ -O2 -I$(CRITERION_PATH) $(GCC_FLAGS) *.cpp -lcriterion -o "test"
	./test --verbose
