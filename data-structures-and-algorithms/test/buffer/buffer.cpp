#include <gtest/gtest.h>

extern "C" {
#include "buffer/buffer.h"
#include "log/log.h"
}

class TestBuffer: public ::testing::Test {
    protected:
        void SetUp() override {
            LOG_ADD_DEFAULT_MODULE("buffer-test", L_INFO);
        }

        // void TearDown() override {}
};

TEST_F(TestBuffer, TestOperations) {
    buffer_t *src = buffer_create(3 * sizeof(int));
    assert(src->cap == 3 *sizeof(int));

    buffer_t dst = {};
    buffer_cpy(&dst, src);
    assert(dst.cap == 3 * sizeof(int));

    int v[5] = { 1, 2, 3, 4, 5};
    buffer_memcpy(&dst, &v, sizeof(v));
    assert(dst.cap == sizeof(v));
    assert(*(int*)dst.val + 0 == 1);
    assert(*(int*)dst.val + 4 == 5);

    *((int*)src->val+0) = 100;
    *((int*)src->val+1) = 101;
    src->len += 2*sizeof(int);

    buffer_append(&dst, src);
    assert(dst.cap == sizeof(v) + src->cap);
    assert(*((int*)dst.val + 5) == 100);

    size_t expected_size = dst.cap + sizeof(v) - (dst.cap - dst.len);
    buffer_append_mem(&dst, &v, sizeof(v));
    assert(dst.cap == expected_size);

    void *it = NULL;
    while ((it = buffer_advance(&dst, sizeof(int))) != NULL) {
        LOG("value: %d", *(int*)it);
    }
}
