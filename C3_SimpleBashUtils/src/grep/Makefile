CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11

all: s21_grep

s21_grep: s21_grep.o
	$(CC) $(FLAGS) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c s21_grep.h
	$(CC) $(FLAGS) -c s21_grep.c

clean:
	rm -f s21_grep.o s21_grep