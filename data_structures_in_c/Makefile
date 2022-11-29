CC = gcc

CCFLAGS = -Wall -Wextra -Wvla -Werror -g -c -std=c99

.PHONY = all clean

all: libhashmap.a libhashmap_tests.a

clean:
	rm -f libhashmap.a vector.o pair.o hashmap.o libhashmap_tests.a \
	test_suite.o

libhashmap.a: vector.o pair.o hashmap.o
	ar rcs $@ $^
libhashmap_tests.a: test_suite.o vector.o pair.o hashmap.o
	ar rcs $@ $^
vector.o: vector.c vector.h
	$(CC) $(CCFLAGS) $< -o $@
pair.o: pair.c pair.h
	$(CC) $(CCFLAGS) $< -o $@
hashmap.o: hashmap.c hashmap.h
	$(CC) $(CCFLAGS) $< -o $@
test_suite.o: test_suite.c test_suite.h
	$(CC) $(CCFLAGS) $< -o $@
