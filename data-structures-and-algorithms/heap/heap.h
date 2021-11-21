#pragma once

typedef struct {
    uint32_t *array;
    int last;
    size_t cap;
} heap_t;

size_t heap_get_left_child(size_t index);

size_t heap_get_right_child(size_t index);

size_t heap_get_parent(size_t index);

int _heap_has_greater_child(heap_t *heap, size_t index);

size_t _heap_get_larger_index(heap_t *heap, size_t index);

void _heap_sink(heap_t *heap);

void _heap_float(heap_t *heap);

int heap_insert(heap_t *heap, int val);

int heap_pop(heap_t *heap);
