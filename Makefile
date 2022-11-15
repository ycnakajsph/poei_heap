CC ?= gcc
CFLAGS += -Wall --pedantic -Werror
DEBUG_FLAGS += -g

all:
	$(CC) $(LDFLAGS) -o heap $(CFLAGS) heap.c main.c

test:
	$(CC) $(LDFLAGS)  -o heap_tests $(CFLAGS) tests/test_main.c heap.c -lcunit

clean:
	rm -f *.o heap_tests heap
