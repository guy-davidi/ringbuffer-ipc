/*
 * ring_buffer.c - Inter-CPU Ring Buffer
 *
 * Copyright (C) 2025 Guy Davidi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include "ring_buffer.h"

static inline void memory_barrier() {
    __asm__ volatile ("" ::: "memory");
}

int ring_buffer_send(ring_buffer_t *buffer, uint8_t val) {
    uint32_t tail = buffer->tail;
    uint32_t head = buffer->head;
    uint32_t next_tail = (tail + 1) % RING_BUFFER_SIZE;

    if (next_tail == head)
        return RING_BUFFER_FULL;

    buffer->data[tail] = val;
    memory_barrier();
    buffer->tail = next_tail;

    return RING_BUFFER_OK;
}

int ring_buffer_recv(ring_buffer_t *buffer, uint8_t *out) {
    uint32_t head = buffer->head;
    uint32_t tail = buffer->tail;

    if (head == tail)
        return RING_BUFFER_EMPTY;

    *out = buffer->data[head];
    memory_barrier();
    buffer->head = (head + 1) % RING_BUFFER_SIZE;

    return RING_BUFFER_OK;
}
