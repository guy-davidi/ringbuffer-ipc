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

int ring_buffer_send_bulk(ring_buffer_t *buffer, const uint8_t *src, int len) {
    int sent = 0;
    while (sent < len) {
        uint32_t tail = buffer->tail;
        uint32_t head = buffer->head;
        uint32_t next_tail = (tail + 1) % RING_BUFFER_SIZE;

        if (next_tail == head)
            break; // full

        buffer->data[tail] = src[sent++];
        memory_barrier();
        buffer->tail = next_tail;
    }
    return sent;
}

int ring_buffer_recv_bulk(ring_buffer_t *buffer, uint8_t *dst, int len) {
    int recvd = 0;
    while (recvd < len) {
        uint32_t head = buffer->head;
        uint32_t tail = buffer->tail;
        if (head == tail)
            break; // empty

        dst[recvd++] = buffer->data[head];
        memory_barrier();
        buffer->head = (head + 1) % RING_BUFFER_SIZE;
    }
    return recvd;
}

void ring_buffer_reset(ring_buffer_t *buffer) {
    buffer->head = 0;
    buffer->tail = 0;
    memory_barrier();
}

int ring_buffer_used(const ring_buffer_t *buffer) {
    uint32_t head = buffer->head;
    uint32_t tail = buffer->tail;
    return (tail + RING_BUFFER_SIZE - head) % RING_BUFFER_SIZE;
}

int ring_buffer_free(const ring_buffer_t *buffer) {
    return RING_BUFFER_SIZE - 1 - ring_buffer_used(buffer);
}
