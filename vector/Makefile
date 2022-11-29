
PHONY: run_all, valgrind_all, run_tests, run_tests-verbose, \
	valgrind_tests, run_bonus_tests, valgrind_bonus_tests, clean

CCFLAGS = -Wall -Wvla -Wextra -Werror -g -std=c++14

CC = g++

run_all:
	run_tests
	run_bonus_tests

valgrind_all:
	valgrind_tests
	valgrind_bonus_tests

run_tests: tests
	tests --color_output=true

run_tests-verbose: tests
	tests --color_output=true -l all

valgrind_tests: tests
	valgrind --track-origins=yes tests

run_bonus_tests: bonus_tests
	bonus_tests

valgrind_bonus_tests: bonus_tests
	valgrind --track-origins=yes bonus_tests


tests: ex6_tests.cpp vl_vector.h
	$(CC) $(CCFLAGS) $< -o tests

bonus_tests: ex6_bonus_tests.cpp vl_string.h vl_vector.h
	$(CC) $(CCFLAGS) $< -o bonus_tests

clean:
	rm tests
	rm bonus_tests