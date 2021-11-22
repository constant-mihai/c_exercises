#pragma once

#include "hash/hash.h"

#define MAX_HASH_SIZE 1000

// each node has a hash table.
// the keys represent the next letter.
// the values are pointers to the next nodes.
typedef struct node {
    hash_t *hash;
} node_t;

typedef enum {
    SEARCH,
    REMOVE,
}sor_e;

int validate_word(const char* word);

node_t* trie_create();

node_t* set_child(node_t *trie, const char* c, int is_leaf);

int trie_dict_insert(node_t *trie, const char* word);

int trie_dict_search_or_remove(node_t *trie,
                           const char* word,
                           sor_e sor,
                           size_t *i);

int is_leaf(node_t *trie);

void trie_print(node_t *trie, const char *prefix);
