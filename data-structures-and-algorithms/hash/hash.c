#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "hash.h"

#define LOG_ENABLE 1

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) do { if (LOG_ENABLE == 1) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__); } while (0)


hash_t *hash_create(int cap) {
    hash_t *ht = calloc(1, sizeof(hash_t));
    if (ht == NULL) {
        return NULL;
    }
    ht->len = 0;
    ht->cap = cap;

    ht->entries = calloc(ht->cap, sizeof(hash_table_entry_t));
    if (ht->entries == NULL) {
        free(ht);
        return NULL;
    }

    return ht;
}

void hash_destroy(hash_t *ht) {
    if (ht == NULL) {
        return; 
    }

    for(size_t i=0; i<ht->cap; i++) {
        printf("removing: %s", ht->entries[i].key);
        free(ht->entries[i].key);
    }

    free(ht->entries);
    free(ht);
}

// https://stackoverflow.com/questions/7666509/hash-function-for-string 
uint64_t hash_function(const char *key) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

size_t hash_get_index(hash_t *ht, const char* key) {
    uint64_t hash = hash_function(key);
    return (size_t)(hash % (ht->cap -1 ));
}

int hash_set(hash_t *ht, const char *key, void *value) {
    if (ht == NULL || key == NULL) {
        return -1;
    }

    if (ht->len >= ht->cap / 2) {
        // TODO expand table here
    }

    size_t index = hash_get_index(ht, key);

    while (ht->entries[index].key != NULL) {
        if (strcmp(key, ht->entries[index].key) == 0) {
            LOG("Found key %s at index %ld", key, index);
            ht->entries[index].value = value;
            goto done;
        }
        index++;
        if (index >= ht->cap) {
            index = 0;
        }
    }

    LOG("Inserting key %s at index %ld", key, index);
    ht->entries[index].key = strdup(key);
    if (ht->entries[index].key == NULL) {
        return -1;
    }
    ht->entries[index].value = value;

done:
    ht->len++;
    return 0;
}

int hash_find_or_del(hash_t *ht, const char *key, void **value, int del) {
    if (ht == NULL || key == NULL) {
        return -1;
    }

    *value = NULL;

    size_t index = hash_get_index(ht, key);
    size_t start_index = index;

    const char *op = ((del == 1) ? "deleting index" : "found index");
    while (ht->entries[index].key != NULL) {
        if (strcmp(key, ht->entries[index].key) == 0) {
            LOG("%s: %lu, key: %s",
                op,
                index,
                ht->entries[index].key);
            if (!del) {
                *value = ht->entries[index].value;
            } else {
                free(ht->entries[index].value);
                free(ht->entries[index].key);
                ht->entries[index].value = NULL;
                ht->entries[index].key = NULL;
                ht->len--;
            }
            return 0;
        }
        index++;
        if (index >= ht->cap) {
            index = 0;
        } else if (index == start_index) {
            return 1;
        }
    }

    return 1;
}

int hash_get(hash_t *ht, const char *key, void **value) {
    return hash_find_or_del(ht, key, value, 0);
}

int hash_del(hash_t *ht, const char *key) {
    void * val = 0;
    return hash_find_or_del(ht, key, &val, 1);
}

void hash_foreach(hash_t *ht, callback_fn cb, va_list argp) {
    for (size_t i=0; i<ht->len; i++) {
        if (ht->entries[i].key != NULL) {
            cb(ht->entries[i].key, argp);
        }
    }
}

size_t hash_len(hash_t *ht) {
    return ht->len;
}

size_t hash_cap(hash_t *ht) {
    return ht->cap;
}

int main(int argc,  char **argv) {
    (void)argc;
    (void)argv;
    printf("main\n");

    hash_t * ht = hash_create(100);
    int a = 1;
    int b = 2;
    int err = 0;
    err = hash_set(ht, "a", &a);
    err = hash_set(ht, "b", &b);

    int *pa = malloc(sizeof(int));
    void **vpa = (void*)pa;
    err = hash_get(ht, "a", vpa);

    (void)err;

    printf("a: %d\n", *(int*)(*vpa));

    hash_destroy(ht);
    return 0;
}
