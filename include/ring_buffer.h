/**
 * @file ring_buffer.h
 * @brief Lock-free ring buffer for inter-CPU/process communication.
 *
 * This ring buffer is designed for single-producer, single-consumer scenarios.
 * Safe for inter-CPU shared memory when used with C11 atomics.
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stddef.h>
#include <stdatomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Return codes for ring buffer operations */
enum {
    RING_BUFFER_OK = 0,    /**< Operation successful */
    RING_BUFFER_FULL = 1,  /**< Buffer is full */
    RING_BUFFER_EMPTY = 2  /**< Buffer is empty */
};

/** Ring buffer size (must be a power of two for best performance) */
#define RING_BUFFER_SIZE 1024

/**
 * @struct ring_buffer_t
 * @brief Ring buffer object.
 *
 * @var ring_buffer_t::head
 *      Index of the next element to read (consumer).
 * @var ring_buffer_t::tail
 *      Index of the next element to write (producer).
 * @var ring_buffer_t::data
 *      Buffer data storage.
 */
typedef struct ring_buffer {
    atomic_uint head;
    atomic_uint tail;
    uint8_t data[RING_BUFFER_SIZE];
} ring_buffer_t;

/**
 * @brief Initialize the ring buffer (sets head and tail to zero).
 * @param buffer Pointer to ring buffer.
 */
void ring_buffer_init(ring_buffer_t *buffer);

/**
 * @brief Send a single byte to the ring buffer.
 * @param buffer Pointer to ring buffer.
 * @param val Byte to send.
 * @return RING_BUFFER_OK if success, RING_BUFFER_FULL if buffer is full.
 */
int ring_buffer_send(ring_buffer_t *buffer, uint8_t val);

/**
 * @brief Receive a single byte from the ring buffer.
 * @param buffer Pointer to ring buffer.
 * @param out Pointer to output variable.
 * @return RING_BUFFER_OK if success, RING_BUFFER_EMPTY if buffer is empty.
 */
int ring_buffer_recv(ring_buffer_t *buffer, uint8_t *out);

/**
 * @brief Send multiple bytes to the ring buffer.
 * @param buffer Pointer to ring buffer.
 * @param src Data to send.
 * @param len Number of bytes to send.
 * @return Number of bytes actually sent.
 */
int ring_buffer_send_bulk(ring_buffer_t *buffer, const uint8_t *src, int len);

/**
 * @brief Receive multiple bytes from the ring buffer.
 * @param buffer Pointer to ring buffer.
 * @param dst Destination buffer.
 * @param len Number of bytes to receive.
 * @return Number of bytes actually received.
 */
int ring_buffer_recv_bulk(ring_buffer_t *buffer, uint8_t *dst, int len);

/**
 * @brief Reset the ring buffer to empty state.
 * @param buffer Pointer to ring buffer.
 */
void ring_buffer_reset(ring_buffer_t *buffer);

/**
 * @brief Get number of bytes currently stored in buffer.
 * @param buffer Pointer to ring buffer.
 * @return Number of bytes used.
 */
int ring_buffer_used(const ring_buffer_t *buffer);

/**
 * @brief Get number of bytes available for writing.
 * @param buffer Pointer to ring buffer.
 * @return Number of bytes free.
 */
int ring_buffer_free(const ring_buffer_t *buffer);

#ifdef __cplusplus
}
#endif

#endif // RING_BUFFER_H
