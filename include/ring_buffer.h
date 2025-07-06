/*
 * ring_buffer.h - Inter-CPU Ring Buffer
 *
 * Copyright (C) 2025 Guy Davidi
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

#define RING_BUFFER_SIZE 1024
#define RING_BUFFER_OK    0
#define RING_BUFFER_FULL  1
#define RING_BUFFER_EMPTY 2

typedef struct {
    volatile uint32_t head;
    volatile uint32_t tail;
    volatile uint8_t data[RING_BUFFER_SIZE];
} ring_buffer_t;

#ifdef __cplusplus
extern "C" {
#endif

int ring_buffer_send(ring_buffer_t *buffer, uint8_t val);
int ring_buffer_recv(ring_buffer_t *buffer, uint8_t *out);

#ifdef __cplusplus
}
#endif

#endif // RING_BUFFER_H
