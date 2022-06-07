#include <gtest/gtest.h>

extern "C" {

#include "heap/heap.h"
#include "log/log.h"
}

#define HEAP_SIZE 100

class TestHeap: public ::testing::Test {
    protected:
        void SetUp() override {
            LOG_ADD_DEFAULT_MODULE("heap-test", L_INFO);
        }

        // void TearDown() override {}
};

TEST_F(TestHeap, TestInsert) {
    // TODO las = -1 is _very_ unintuitive. Adds a create function.
    heap_t heap = {
        .array = (uint32_t*)calloc(HEAP_SIZE, sizeof(uint32_t)),
        .last = -1,
        .cap = HEAP_SIZE,
    };
    assert(heap.array != NULL);

    heap_insert(&heap, 10);
    heap_insert(&heap, 10);
    heap_insert(&heap, 6);
    heap_insert(&heap, 9);
    heap_insert(&heap, 8);
    heap_insert(&heap, 7);
    heap_insert(&heap, 20);

    LOG("Dump heap:");
    for (int i=0; heap.last >= i; i++) {
        LOG("%d", heap.array[i]);
    }
}

TEST_F(TestHeap, TestRemove) {
    heap_t heap = {
        .array = (uint32_t*)calloc(HEAP_SIZE, sizeof(uint32_t)),
        .last = -1,
        .cap = HEAP_SIZE,
    };
    assert(heap.array != NULL);

    heap_insert(&heap, 10);
    heap_insert(&heap, 11);
    heap_insert(&heap, 6);
    heap_insert(&heap, 9);
    heap_insert(&heap, 8);
    heap_insert(&heap, 7);
    heap_insert(&heap, 20);

    LOG("Dump heap:");
    for (int i=0; heap.last >= i; i++) {
        LOG("%d", heap.array[i]);
    }

    heap_pop(&heap);
    LOG("Dump heap:");
    for (int i=0; heap.last >= i; i++) {
        LOG("%d", heap.array[i]);
    }

    heap_pop(&heap);
    LOG("Dump heap:");
    for (int i=0; heap.last >= i; i++) {
        LOG("%d", heap.array[i]);
    }

    heap_pop(&heap);

    LOG("Dump heap:");
    for (int i=0; heap.last >= i; i++) {
        LOG("%d", heap.array[i]);
    }
}