#pragma once

typedef struct kv {
    struct node *child;
} kv_t;

typedef struct node {
    kv_t kv[10];
} node_t;

typedef enum {
    SEARCH,
    REMOVE,
}trie_sor_e;


int trie_insert(node_t *trie, const char* ip);

int trie_search_or_remove(node_t *trie,
                           const char* ip,
                           trie_sor_e sor,
                           size_t *i);
