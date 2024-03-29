#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "heap/heap.h"
#include "log/log.h"

//TODO add a create function

size_t heap_get_left_child(size_t index) {
    return index*2+1;
}

size_t heap_get_right_child(size_t index) {
    return index*2+2;
}

size_t heap_get_parent(size_t index) {
    return (index-1)/2;
}

int _heap_has_greater_child(heap_t *heap, size_t index) {
    assert(heap != NULL);
    size_t li = heap_get_left_child(index);
    size_t ri = heap_get_right_child(index);

    return ((heap->array[li] != 0) &&
        (heap->array[li] > heap->array[index])) ||
        ((heap->array[ri] != 0) &&
        (heap->array[ri] > heap->array[index]));
}

size_t _heap_get_larger_index(heap_t *heap, size_t index) {
    assert(heap != NULL);

    if (heap->array[heap_get_right_child(index)] != 0) {
        if (heap->array[heap_get_right_child(index)] > heap->array[heap_get_left_child(index)]) {
            return heap_get_right_child(index);
        }
    }

    return heap_get_left_child(index);
}

void _heap_sink(heap_t *heap) {
    assert(heap != NULL);

    size_t index = 0;
    size_t larger_index = 0;
    while (_heap_has_greater_child(heap, index)) {
        larger_index = _heap_get_larger_index(heap, index);
        int tmp = heap->array[index];
        heap->array[index] = heap->array[larger_index];
        heap->array[larger_index] = tmp;
        index = larger_index;
    }
}

void _heap_float(heap_t *heap) {
    assert(heap != NULL);
    size_t current_index = heap->last;
    size_t parent_index = heap_get_parent(current_index);
    while (current_index > 0 && heap->array[current_index] > heap->array[parent_index]) {
        // swap new node with parent.
        int tmp = heap->array[current_index];
        heap->array[current_index] = heap->array[parent_index];
        heap->array[parent_index] = tmp;
        current_index = heap_get_parent(current_index);
        parent_index = heap_get_parent(current_index);
    }
}

int heap_insert(heap_t *heap, int val) {
    assert(heap != NULL);
    assert(heap->cap > 0);

    heap->last++;
    heap->array[heap->last] = val;
    heap->cap--;

    _heap_float(heap);

    return 0;
}

int heap_pop(heap_t *heap) {
    assert(heap != NULL);

    int last = heap->array[heap->last];
    heap->array[heap->last] = 0;
    heap->last--;
    heap->cap++;

    int ret = heap->array[0];
    heap->array[0] = last;

    _heap_sink(heap);

    return ret;
}
