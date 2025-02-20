CC = gcc
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    LDFLAGS = -lcheck -lm --coverage
else
    LDFLAGS = -lcheck -lsubunit -lm --coverage
endif
CFLAGS = -Wall -Werror -Wextra -std=c11
GCOVFLAGS = -fprofile-arcs -ftest-coverage
OBJ = arthmetic.o comparision.o convertors.o help_functions.o other.o
s21_decimal.a = s21_decimal.a
TEST_OBJ = TESTS/SUITE_main.o

# HTML report directories-lcheck -lpthread -lm -lsubunit -lgcov
GCOV_REPORT_DIR = gcov_report

all: clean s21_decimal.a test gcov_report

s21_decimal.a: $(OBJ)
	ar -rc s21_decimal.a $(OBJ)
	ranlib s21_decimal.a
	rm *.o

test: $(TEST_OBJ) s21_decimal.a
	$(CC) $(CFLAGS) $(LDFLAGS) -c *.c
	$(CC) $(CFLAGS) -o test.out $(TEST_OBJ) *.o $(LDFLAGS)
	make run_tests

run_tests:
	./test.out

# Generate gcov report -fprofile-arcs -ftest-coverage coverage.info --output-directory
gcov_report: run_tests
	mkdir -p $(GCOV_REPORT_DIR)
	lcov --capture --directory . --output-file coverage.info
	lcov --remove coverage.info '*/tests/*' '*/usr/*' --output-file coverage.info --ignore-errors unused
	genhtml coverage.info --output-directory $(GCOV_REPORT_DIR)
	make open_report

open_report:
	@if [ "$(UNAME)" = "Darwin" ]; then \
		open $(GCOV_REPORT_DIR)/index.html; \
	elif [ "$(UNAME)" = "Linux" ]; then \
		if grep -iq microsoft /proc/version; then \
			echo "You are using a terminal in Windows OS (WSL)!"; \
			echo "You can open html report using cmd command line enter command 'start gcov_report/index.html' ..."; \
		else \
			xdg-open $(GCOV_REPORT_DIR)/index.html; \
		fi \
	else \
		echo "Unsupported OS"; \
	fi

arthmetic.o: arthmetic.c s21_decimal.h
	$(CC) $(CFLAGS) -c arthmetic.c -o arthmetic.o

comparision.o: comparision.c s21_decimal.h
	$(CC) $(CFLAGS) -c comparision.c -o comparision.o

convertors.o: convertors.c s21_decimal.h
	$(CC) $(CFLAGS) -c convertors.c -o convertors.o

help_functions.o: help_functions.c s21_help.h
	$(CC) $(CFLAGS) -c help_functions.c -o help_functions.o

other.o: other.c s21_help.h
	$(CC) $(CFLAGS) -c other.c -o other.o

TESTS/SUITE_main.o: TESTS/SUITE_main.c s21_decimal.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c TESTS/SUITE_main.c -o TESTS/SUITE_main.o

check:
	clang-format -style=Google -n ./*.c ./*.h

rebuild: clean all

clean:
	rm -f $(OBJ) s21_decimal.a test.out TESTS/SUITE_main.o
	rm -f coverage.info
	rm -rf $(GCOV_REPORT_DIR)
	rm -f *.gcda *.gcno TESTS/*.gcda TESTS/*.gcno

.PHONY: all clean test run_tests gcov_report open_report
