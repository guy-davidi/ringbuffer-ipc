/*
 * test_ring_buffer.c - Test for Inter-CPU Ring Buffer
 *
 * Copyright (C) 2025 Guy Davidi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/ring_buffer.h"

ring_buffer_t shared_buffer = {0};

void *producer_thread(void *arg) {
    for (int i = 0; i < 2000; ++i) {
        while (ring_buffer_send(&shared_buffer, i % 256) == RING_BUFFER_FULL)
            usleep(10);
    }
    return NULL;
}

void *consumer_thread(void *arg) {
    uint8_t val;
    int count = 0;
    while (count < 2000) {
        if (ring_buffer_recv(&shared_buffer, &val) == RING_BUFFER_OK) {
            printf("Received: %u\n", val);
            count++;
        } else {
            usleep(10);
        }
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;
    pthread_create(&prod, NULL, producer_thread, NULL);
    pthread_create(&cons, NULL, consumer_thread, NULL);
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    return 0;
}
