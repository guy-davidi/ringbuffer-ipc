#include "unity.h"
#include "../include/ring_buffer.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_single_send_recv(void) {
    ring_buffer_t buf = {0};
    uint8_t val = 0xA5, out = 0;

    TEST_ASSERT_EQUAL(RING_BUFFER_OK, ring_buffer_send(&buf, val));
    TEST_ASSERT_EQUAL(RING_BUFFER_OK, ring_buffer_recv(&buf, &out));
    TEST_ASSERT_EQUAL_HEX8(val, out);
}

void test_buffer_full_and_empty(void) {
    ring_buffer_t buf = {0};
    int i;
    for (i = 0; i < RING_BUFFER_SIZE-1; i++)
        TEST_ASSERT_EQUAL(RING_BUFFER_OK, ring_buffer_send(&buf, i));

    TEST_ASSERT_EQUAL(RING_BUFFER_FULL, ring_buffer_send(&buf, 0xFF));

    uint8_t out;
    for (i = 0; i < RING_BUFFER_SIZE-1; i++)
        TEST_ASSERT_EQUAL(RING_BUFFER_OK, ring_buffer_recv(&buf, &out));

    TEST_ASSERT_EQUAL(RING_BUFFER_EMPTY, ring_buffer_recv(&buf, &out));
}

void test_bulk_send_recv(void) {
    ring_buffer_t buf = {0};
    uint8_t in[100], out[100] = {0};
    int i;
    for (i = 0; i < 100; ++i) in[i] = (uint8_t)i;

    TEST_ASSERT_EQUAL(100, ring_buffer_send_bulk(&buf, in, 100));
    TEST_ASSERT_EQUAL(100, ring_buffer_recv_bulk(&buf, out, 100));
    TEST_ASSERT_EQUAL(0, memcmp(in, out, 100));
}

void test_reset_and_query(void) {
    ring_buffer_t buf = {0};
    ring_buffer_send_bulk(&buf, (uint8_t[]){1,2,3,4,5}, 5);
    ring_buffer_reset(&buf);
    TEST_ASSERT_EQUAL(0, ring_buffer_used(&buf));
    TEST_ASSERT_EQUAL(RING_BUFFER_SIZE-1, ring_buffer_free(&buf));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_single_send_recv);
    RUN_TEST(test_buffer_full_and_empty);
    RUN_TEST(test_bulk_send_recv);
    RUN_TEST(test_reset_and_query);
    return UNITY_END();
}
