#ifndef __HASH_H__
#define __HASH_H__

typedef struct {
    char *key;
    void *value;
} hash_table_entry_t;

typedef struct {
    hash_table_entry_t *entries;
    uint32_t len;
    uint32_t cap;
} hash_t;

hash_t *hash_create(int cap);

void hash_destroy(hash_t *ht);

// https://stackoverflow.com/questions/7666509/hash-function-for-string 
uint64_t hash_function(const char *key);

int hash_set(hash_t *ht, const char *key, void *value, size_t value_len);

int hash_get(hash_t *ht, const char *key, void **value);

int hash_del(hash_t *ht, const char *key);

typedef void (*callback_fn)(void *arg, ...);

void hash_foreach(hash_t *ht, callback_fn cb, va_list argp);

size_t hash_len(hash_t *ht);

size_t hash_cap(hash_t *ht);

void hash_init();

#endif
