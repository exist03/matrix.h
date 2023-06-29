#.PHONY: all clean

CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11

ifeq ($(shell uname), Linux)
	LINUX_LIBS=-lsubunit -lrt -lpthread -lm
endif

all: s21_matrix.a

s21_matrix.a: s21_matrix.o
	ar r s21_matrix.a s21_matrix.o
	ranlib s21_matrix.a

s21_matrix.o:
	$(CC) $(CFLAGS) -c s21_matrix.c

test:
	$(CC) matrix_test.c s21_matrix.a -o check.out -lcheck $(LINUX_LIBS)
	./check.out

gcov_report: matrix_test.c	s21_matrix.c
	clear
	$(CC) *.c -o coverage.o -lcheck $(LINUX_LIBS) -fprofile-arcs -ftest-coverage
	./coverage.o
	lcov -c -d . -o coverage.info
	genhtml coverage.info -o coverage
	open coverage/index.html

clean:
	rm -rf *.o *.out *.a
	rm -rf *.gcda *.gcno
	rm -rf coverage coverage.info
