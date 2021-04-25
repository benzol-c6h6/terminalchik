all:
	gcc main.c function.c -o main
leaks:
	gcc main.c function.c -o main -fsanitize=address
cppcheck:
	cppcheck --enable=all --inconclusive --std=posix *.c