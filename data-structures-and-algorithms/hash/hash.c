#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define LOG_INFO "[%s][%s]%s:%d\t\t"
#define LOG_INFO_VAL __TIME__,__FILE__,__func__,__LINE__
#define LOG(msg, ...) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__)

typedef struct {
    char *key;
    void *value;
} hash_table_entry;

typedef struct {
    hash_table_entry *entries;
    uint32_t len;
    uint32_t cap;
} hash_table;

hash_table *create(int cap) {
    hash_table *ht = calloc(1, sizeof(hash_table));
    if (ht == NULL) {
        return NULL;
    }
    ht->len = 0;
    ht->cap = cap;

    ht->entries = calloc(ht->cap, sizeof(hash_table_entry));
    if (ht->entries == NULL) {
        free(ht);
        return NULL;
    }

    return ht;
}

void destroy(hash_table *ht) {
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

    while (c = *key++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int set(hash_table *ht, const char *key, void *value) {
    if (ht == NULL || key == NULL || value == NULL) {
        return -1;
    }

    if (ht->len >= ht->cap / 2) {
        // TODO expand table here
    }

    uint64_t hash = hash_function(key);
    size_t index = (size_t)(hash % (ht->cap -1 ));

    while (ht->entries[index].key != NULL) {
        if (strcmp(key, ht->entries[index].key) == 0) {
            ht->entries[index].value = value;
            return 0;
        }
        index++;
        if (index >= ht->cap) {
            index = 0;
        }
    }

    ht->entries[index].key = strdup(key);
    if (ht->entries[index].key == NULL) {
        return -1;
    }
    ht->entries[index].value = value;

    return 0;
}

int get(hash_table *ht, const char *key, void **value) {
    if (ht == NULL || key == NULL) {
        return -1;
    }

    uint64_t hash = hash_function(key);
    size_t index = (size_t)(hash % (ht->cap -1));
    *value = NULL;

    while (ht->entries[index].key != NULL) {
        LOG("index: %lu, key: %s, value: %d\n",
            index,
            ht->entries[index].key,
            *(int*)(ht->entries[index].value));
        if (strcmp(key, ht->entries[index].key) == 0) {
            *value = ht->entries[index].value;
            return 0;
        }
    }

    return 1;
}

int main(int argc,  char **argv) {
    printf("main\n");

    hash_table * ht = create(100);
    int a = 1;
    int b = 2;
    int err = 0;
    err = set(ht, "a", &a);
    err = set(ht, "b", &b);

    int *pa = malloc(sizeof(int));
    void **vpa = (void*)pa;
    err = get(ht, "a", vpa);

    printf("a: %d\n", *(int*)(*vpa));

    return 0;
}
