CC ?= gcc
CFLAGS += -Wall --pedantic -Werror
DEBUG_FLAGS += -g

all:
	$(CC) $(LDFLAGS) -o tas $(CFLAGS) tas.c main.c

test:
	$(CC) $(LDFLAGS)  -o tas_tests $(CFLAGS) tests/test_main.c tas.c -lcunit

clean:
	rm -f *.o tas_tests tas
