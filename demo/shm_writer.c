#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include "../include/ring_buffer.h"

#define SHM_NAME "ipc_ringbuf"
#define RINGBUF_SIZE sizeof(ring_buffer_t)
#define ITEMS 1000

int main() {
    printf("Starting writer...\n");
    fflush(stdout);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        printf("SHM_NAME was: '%s'\n", SHM_NAME);
        return 1;
    }
    ftruncate(shm_fd, RINGBUF_SIZE);
    ring_buffer_t *buf = mmap(NULL, RINGBUF_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (buf == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    buf->head = buf->tail = 0;

    for (int i = 0; i < ITEMS; ++i) {
        while (ring_buffer_send(buf, (uint8_t)(i & 0xFF)) == RING_BUFFER_FULL) {
            usleep(100); // Wait for space
        }
        printf("Wrote %d\n", i & 0xFF);
        fflush(stdout);
    }

    printf("Writer done. Press Enter to cleanup and exit.\n");
    fflush(stdout);
    getchar();
    munmap(buf, RINGBUF_SIZE);
    shm_unlink(SHM_NAME);
    close(shm_fd);
    return 0;
}
