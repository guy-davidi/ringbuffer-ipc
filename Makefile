CC=gcc
CFLAGS=-O2 -Wall -Iinclude
LDFLAGS=-lpthread

SRC=src/ring_buffer.c
TEST=test/test_ring_buffer.c
OBJ=$(SRC:.c=.o)

.PHONY: all clean test

all: test

test: $(OBJ) $(TEST)
	$(CC) $(CFLAGS) -o ring_buffer_test $(TEST) $(OBJ) $(LDFLAGS)

clean:
	rm -f ring_buffer_test src/*.o
