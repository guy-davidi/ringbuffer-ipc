/**
 * @file ring_buffer.c
 * @brief Lock-free ring buffer implementation (see ring_buffer.h).
 */

#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t *buffer)
{
    atomic_store_explicit(&buffer->head, 0, memory_order_relaxed);
    atomic_store_explicit(&buffer->tail, 0, memory_order_relaxed);
}

int ring_buffer_send(ring_buffer_t *buffer, uint8_t val)
{
    uint32_t tail = atomic_load_explicit(&buffer->tail, memory_order_relaxed);
    uint32_t head = atomic_load_explicit(&buffer->head, memory_order_acquire);

    uint32_t next_tail = (tail + 1) % RING_BUFFER_SIZE;
    if (next_tail == head)
        return RING_BUFFER_FULL;

    buffer->data[tail] = val;
    atomic_thread_fence(memory_order_release);
    atomic_store_explicit(&buffer->tail, next_tail, memory_order_release);

    return RING_BUFFER_OK;
}

int ring_buffer_recv(ring_buffer_t *buffer, uint8_t *out)
{
    uint32_t head = atomic_load_explicit(&buffer->head, memory_order_relaxed);
    uint32_t tail = atomic_load_explicit(&buffer->tail, memory_order_acquire);

    if (head == tail)
        return RING_BUFFER_EMPTY;

    *out = buffer->data[head];
    atomic_thread_fence(memory_order_release);
    atomic_store_explicit(&buffer->head, (head + 1) % RING_BUFFER_SIZE, memory_order_release);

    return RING_BUFFER_OK;
}

int ring_buffer_send_bulk(ring_buffer_t *buffer, const uint8_t *src, int len)
{
    int sent = 0;
    while (sent < len) {
        uint32_t tail = atomic_load_explicit(&buffer->tail, memory_order_relaxed);
        uint32_t head = atomic_load_explicit(&buffer->head, memory_order_acquire);
        uint32_t next_tail = (tail + 1) % RING_BUFFER_SIZE;

        if (next_tail == head)
            break; // full

        buffer->data[tail] = src[sent++];
        atomic_thread_fence(memory_order_release);
        atomic_store_explicit(&buffer->tail, next_tail, memory_order_release);
    }
    return sent;
}

int ring_buffer_recv_bulk(ring_buffer_t *buffer, uint8_t *dst, int len)
{
    int recvd = 0;
    while (recvd < len) {
        uint32_t head = atomic_load_explicit(&buffer->head, memory_order_relaxed);
        uint32_t tail = atomic_load_explicit(&buffer->tail, memory_order_acquire);

        if (head == tail)
            break; // empty

        dst[recvd++] = buffer->data[head];
        atomic_thread_fence(memory_order_release);
        atomic_store_explicit(&buffer->head, (head + 1) % RING_BUFFER_SIZE, memory_order_release);
    }
    return recvd;
}

void ring_buffer_reset(ring_buffer_t *buffer)
{
    atomic_store_explicit(&buffer->head, 0, memory_order_relaxed);
    atomic_store_explicit(&buffer->tail, 0, memory_order_relaxed);
    atomic_thread_fence(memory_order_release);
}

int ring_buffer_used(const ring_buffer_t *buffer)
{
    uint32_t head = atomic_load_explicit(&buffer->head, memory_order_acquire);
    uint32_t tail = atomic_load_explicit(&buffer->tail, memory_order_acquire);
    return (tail + RING_BUFFER_SIZE - head) % RING_BUFFER_SIZE;
}

int ring_buffer_free(const ring_buffer_t *buffer)
{
    return RING_BUFFER_SIZE - 1 - ring_buffer_used(buffer);
}
