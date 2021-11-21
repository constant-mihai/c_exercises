#include <gtest/gtest.h>

extern "C" {

#include "hash/hash.h"
#include "log/log.h"
}

TEST(TestHash, TestSet) {
    hash_t * ht = hash_create(100);
    int a = 1;
    int b = 2;
    int err = 0;
    err = hash_set(ht, "a", &a, sizeof(int));
    assert(err == 0);
    assert(ht->entries[hash_get_index(ht, "a")].key != NULL);
    assert(!strcmp(ht->entries[hash_get_index(ht, "a")].key, "a"));

    err = hash_set(ht, "b", &b, sizeof(int));
    assert(ht->entries[hash_get_index(ht, "b")].key != NULL);
    assert(!strcmp(ht->entries[hash_get_index(ht, "b")].key, "b"));
    hash_destroy(ht);
}

TEST(TestHash, TestGet) {
    hash_t * ht = hash_create(100);
    int a = 1;
    int b = 2;
    int err = 0;
    void *val = malloc(sizeof(int));
    err = hash_set(ht, "a", &a, sizeof(int));
    assert(err == 0);
    err = hash_get(ht, "a", &val);
    assert(err == 0);
    assert(*(int*)val == 1);

    err = hash_set(ht, "b", &b, sizeof(int));
    assert(err == 0);
    err = hash_get(ht, "b", &val);
    assert(err == 0);
    assert(*(int*)val == 2);
}

TEST(TestHash, TestDel) {
    hash_t * ht = hash_create(100);
    int a = 1;
    int b = 2;
    int err = 0;
    void *val = malloc(sizeof(int));
    err = hash_set(ht, "a", &a, sizeof(int));
    assert(err == 0);
    err = hash_get(ht, "a", &val);
    assert(err == 0);
    assert(*(int*)val == 1);
    err = hash_del(ht, "a");
    assert(err == 0);
    err = hash_get(ht, "a", &val);
    assert(err == 1);

    err = hash_set(ht, "b", &b, sizeof(int));
    assert(err == 0);
    err = hash_del(ht, "b");
    assert(err == 0);
    err = hash_get(ht, "b", &val);
    assert(err == 1);
}
