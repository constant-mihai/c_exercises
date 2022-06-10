#include <string.h>

#include "buffer.h"
#include "utils/utils.h"

buffer_t *buffer_create(size_t size) {
    buffer_t *buf = (buffer_t*)calloc(1, sizeof(buffer_t));
    buf->val = (void*)malloc(size * sizeof(char));
    buf->cap = size;
    buf->len = 0;
    buf->pos = 0;

    return buf;
}

void buffer_cpy(buffer_t *dst, buffer_t *src) {
    if (dst->cap == 0) {
        MALLOC(dst->val, src->cap, void);
        dst->cap = src->cap;
    }
    if (dst->cap < src->cap) {
        REALLOC(dst->val, src->cap, void);
        dst->cap = src->cap;
    }
    memcpy(dst->val, src->val, src->cap);
    dst->len = src->len;
    dst->pos = src->pos;
}

void buffer_memcpy(buffer_t *dst, void *src, size_t size) {
    if (dst->cap == 0) {
        MALLOC(dst->val, size, void);
        //TODO do I trust that the other attributes are set correctly?
        dst->cap = size;
    }
    if (dst->cap < size) {
        REALLOC(dst->val, size, void);
        dst->cap = size;
    }
    memcpy(dst->val, src, size);
    dst->len = size;
}

void buffer_append(buffer_t *dst, buffer_t *src) {
    if (dst->cap == 0) {
        MALLOC(dst->val, src->cap, void);
        dst->cap = src->cap;
        dst->len = src->len;
        dst->pos = 0;
    }
    if (dst->cap - dst->len < src->cap) {
        REALLOC(dst->val, dst->len + src->cap, void);
        dst->cap = dst->len + src->cap;
    }

    memcpy(dst->val+dst->len, src->val, src->len);
    dst->len += src->len;
}

void buffer_append_mem(buffer_t *dst, void *src, size_t size) {
    if (dst->cap == 0) {
        MALLOC(dst->val, size, void);
        //TODO do I trust that the other attributes are set correctly?
        dst->cap = size;
    }
    if (dst->cap - dst->len < size) {
        REALLOC(dst->val, dst->len + size, void);
        dst->cap = dst->len + size;
    }
    memcpy(dst->val+dst->len, src, size);
    dst->len += size;
}

void *buffer_advance(buffer_t *buf, ssize_t len) {
    if (buf->pos == buf->len ||
        (buf->pos == 0 && len < 0)) {
        return NULL;
    }

    void *ret = buf->val + buf->pos;
    buf->pos+=len;
    return ret; 
}

void buffer_destroy(buffer_t **buf) {
    free((*buf)->val);
    free(*buf);
}
