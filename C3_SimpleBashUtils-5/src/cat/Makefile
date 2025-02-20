CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11

all: s21_cat

s21_cat: s21_cat.o
	$(CC) $(FLAGS) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c s21_cat.h
	$(CC) $(FLAGS) -c s21_cat.c -o s21_cat.o

rebuild: clean all

clean:
	rm -f s21_cat s21_cat.o

check:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_cat.c

makeStyle:
	clang-format -i -style=Google *.c
	clang-format -i -style=Google *.h

style:
	clang-format -i -style=Google *.c
	clang-format -i -style=Google *.h