#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct buffer {
    void* val;
    size_t len;
    size_t cap;
    size_t pos;
} buffer_t;

extern buffer_t *buffer_create(size_t size);
//TODO, should I have copy just for len? that way I don't have to copy cap-len.
extern void buffer_cpy(buffer_t *dst, buffer_t *src);
extern void buffer_memcpy(buffer_t *dst, void *src, size_t size);
extern void buffer_append(buffer_t *dst, buffer_t *src);
extern void buffer_append_mem(buffer_t *dst, void *src, size_t size);
extern void *buffer_advance(buffer_t *buf, ssize_t len);
extern void buffer_destroy(buffer_t **buf);
