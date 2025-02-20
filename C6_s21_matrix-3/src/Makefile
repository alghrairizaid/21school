.PHONY: all s21_matrix.a test clean build rebuild gcov_report style valgrind_test

CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11
COV_FLAGS = -fprofile-arcs -ftest-coverage

# Linux-specific test flags
LINUX_FLAGS = -lcheck -lpthread -lrt -lsubunit -lm
# Darwin/Default flags
DEFAULT_FLAGS = -lcheck -lm

SYSTEM := $(shell uname -s)
ifeq ($(SYSTEM), Linux)
    SYSTEM_FLAGS = $(LINUX_FLAGS)
else
    SYSTEM_FLAGS = $(DEFAULT_FLAGS)
endif

SOURCES_ALL = $(wildcard ./core/*.c)
OBJ_LIBRARY := $(patsubst %.c, %.o, $(SOURCES_ALL))

%.o: %.c s21_matrix.h
	$(CC) $(FLAGS) -c $< -o $@

all: clean s21_matrix.a test gcov_report

s21_matrix.a: $(OBJ_LIBRARY)
	ar rcs $@ $(OBJ_LIBRARY)
	ranlib $@ 
	rm -rf ./*/*.o

test: s21_matrix.a TESTS/SUITE_main.c
	$(CC) $(FLAGS) -g TESTS/SUITE_main.c s21_matrix.a -o test.out $(SYSTEM_FLAGS)
	./test.out

test_with_valgrind: test
	valgrind --tool=memcheck \
		--leak-check=full \
		--show-leak-kinds=all \
		--show-reachable=yes \
		--track-origins=yes \
		--verbose \
		--log-file=valgrind_report.txt \
		./test.out

gcov_report: s21_matrix.a TESTS/SUITE_main.c
	@command -v lcov >/dev/null || { echo "Error: lcov not found. Please install lcov."; exit 127; }
	$(CC) $(COV_FLAGS) TESTS/SUITE_main.c $(SOURCES_ALL) -o report.out $(SYSTEM_FLAGS)
	./report.out
	lcov -t "test" -o coverage.info -c -d .
	genhtml -o coverage_report coverage.info
	rm -f *.gcno *.gcda *.info report.out *.gcov
	which xdg-open >/dev/null && xdg-open ./coverage_report/index.html || \
	which google-chrome >/dev/null && google-chrome ./coverage_report/index.html || \
	which firefox >/dev/null && firefox ./coverage_report/index.html || \
	echo "Please open ./coverage_report/index.html in your browser"

style:
	cp ../materials/linters/.clang-format .
	clang-format -n *.h
	clang-format -n ./CORE/*.c
	clang-format -n ./TESTS/*.c ./TESTS/*.h
	rm -rf .clang-format

push:
	git add .
	git commit -m "update code based on date"
	git push origin develop

clean:
	rm -rf *.o *.a *.gcno *.gcda *.info *.out *.gcov
	rm -rf ./report ./coverage_report
	rm -rf report_html
	rm -rf valgrind_report.txt