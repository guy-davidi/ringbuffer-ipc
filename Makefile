CC=gcc
CFLAGS=-O2 -Wall -Iinclude -Itest
LDFLAGS=-lpthread

SRC=src/ring_buffer.c
OBJ=$(SRC:.c=.o)
BUILD_DIR=build

.PHONY: all clean test demo

all: $(BUILD_DIR)/ring_buffer_test demo

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/ring_buffer_test: $(OBJ) test/test_ring_buffer.c test/unity.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ test/test_ring_buffer.c test/unity.c $(OBJ) $(LDFLAGS)

test: $(BUILD_DIR)/ring_buffer_test

demo: $(BUILD_DIR)/shm_writer $(BUILD_DIR)/shm_reader

$(BUILD_DIR)/shm_writer: $(OBJ) demo/shm_writer.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ demo/shm_writer.c $(OBJ) $(LDFLAGS) -lrt

$(BUILD_DIR)/shm_reader: $(OBJ) demo/shm_reader.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ demo/shm_reader.c $(OBJ) $(LDFLAGS) -lrt

clean:
	rm -f $(BUILD_DIR)/ring_buffer_test $(BUILD_DIR)/shm_writer $(BUILD_DIR)/shm_reader src/*.o
