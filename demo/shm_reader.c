#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "../include/ring_buffer.h"

#define SHM_NAME "ipc_ringbuf"
#define RINGBUF_SIZE sizeof(ring_buffer_t)
#define ITEMS 1000

int main() {
    printf("Starting reader...\n");
    fflush(stdout);

    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        printf("SHM_NAME was: '%s'\n", SHM_NAME);
        return 1;
    }
    ring_buffer_t *buf = mmap(NULL, RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (buf == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    for (int i = 0; i < ITEMS; ++i) {
        uint8_t val;
        while (ring_buffer_recv(buf, &val) == RING_BUFFER_EMPTY) {
            usleep(100);
        }
        printf("Read %d\n", val);
        fflush(stdout);
        if (val != (i & 0xFF)) {
            printf("Data mismatch at %d! Got %d\n", i, val);
            fflush(stdout);
        }
    }

    printf("Reader done. Press Enter to exit.\n");
    fflush(stdout);
    getchar();
    munmap(buf, RINGBUF_SIZE);
    close(shm_fd);
    return 0;
}
