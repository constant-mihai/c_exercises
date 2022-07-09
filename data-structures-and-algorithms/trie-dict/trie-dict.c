#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "trie-dict/trie-dict.h"
#include "log/log.h"

int validate_word(const char* word) {
    char c = 0;
    if (strlen(word) == 0) return 0;

    for (size_t i=0; i<strlen(word); i++) {
        c = word[i];
        if ( ! ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
            HR_LOG("Error inserting %s;  %c: is not an alphabetic character.",
                word, c);
            return 0;
        }
    }

    return 1;
}

node_t* trie_create() {
    node_t *trie = calloc(1, sizeof(node_t));
    trie->hash = hash_create(MAX_HASH_SIZE);

    if (trie->hash == NULL) {
        HR_LOG("Error creating hash.");
        free(trie);
        return NULL;
    }

    // This will also be a leaf.
    hash_set(trie->hash, "*", (void*) NULL, 0);

    return trie;
}

node_t* set_child(node_t *trie, const char* c, int is_leaf) {
    node_t *child = 0;
    int hg = hash_get(trie->hash, c, (void*)&child);
    if (hg < 0) {
        HR_LOG("Error getting %s", c);
        return NULL;
    }

    if (hg != 0 /* don't overwrite old values */ ) {
        child = (node_t*)calloc(1, sizeof(node_t));
        child->hash = hash_create(MAX_HASH_SIZE);
        int hs = hash_set(trie->hash, c, (void*) child, sizeof(node_t));
        if (hs < 0) {
            HR_LOG("Error setting %s", c);
            return NULL;
        }
    }
    if (is_leaf) {
        hash_set(child->hash, "*", (void*) NULL, 0);
    }

    return child;
}

int trie_dict_insert(node_t *trie, const char* word) {
    if (validate_word(word) != 1) return -1;

    hash_del(trie->hash, "*");

    char c[2] = {};
    for (size_t i=0; i<strlen(word); i++) {
        memcpy(c, word+i, 1);
        c[1] = '\0';

        trie = set_child(trie, c, (i == strlen(word) - 1));
        if (trie == NULL) {
            HR_LOG("Error setting child for %s.", c);
            return 1;
        }
    }
    return 0;
}

int trie_dict_search_or_remove(node_t *trie,
                           const char* word,
                           sor_e sor,
                           size_t *i) {
    int found = 0;
    char c[2] = {};
    node_t *child = NULL;

    assert(i!=NULL);
    if (validate_word(word)) return -1;

    memcpy(c, word+(*i), 1);
    c[1] = '\0';

    HR_LOG("iteration %lu: %s", *i, c);

    int hg = hash_get(trie->hash, c, (void*)&child);
    if (hg < -1) {
        HR_LOG("Error getting %s", c);
        return -1;
    }

    if (hg == 0) {
        (*i)++;
        found = trie_dict_search_or_remove(child, word, sor, i);
        if (found == 1 && sor == REMOVE) {
            if (*i == strlen(word)) {
                hash_del(trie->hash, "*");
            }
            if (hash_len(child->hash) == 0) {
                // If it doesn't have any other children remove.
                free(child);
                child = NULL;
            } else {
                // Delete the current character from the hash.
                if (hash_del(child->hash, c) != 0) {
                    HR_LOG("Error deleting character %s.", c);
                    return -1;
                }
            }
        }
    }
    return (strlen(word) == (*i)) ? 1 : 0;
}

int is_leaf(node_t *trie) {
    node_t *val = 0;

    // If the hash contains *, this means that all the previous keys form a word.
    if (hash_len(trie->hash) == 1 && hash_get(trie->hash, "*", (void*)&val) == 0) {
        return 1;
    }

    return 0;
}

void trie_print(node_t *trie, const char *prefix) {
    int extended_len = 0;
    char *word = 0;

    if (is_leaf(trie)) {
        printf("%s\n", prefix);
        return;
    }

    for (size_t i=0; i<hash_cap(trie->hash); i++) {
        // optimize here for hash len.
        if (trie->hash->entries[i].key != NULL) {
            if (prefix == NULL) {
                extended_len = 2;
                word = calloc(extended_len, sizeof(char));
            } else {
                extended_len = strlen(prefix) + 2 /* we need to append the next key + '\0' */;
                word = calloc(extended_len, sizeof(char));
                strcat(word, prefix);
            }

            int is_star = (strcmp(trie->hash->entries[i].key, "*") == 0);
            if (!is_star) {
                strcat(word, trie->hash->entries[i].key);
                trie_print(trie->hash->entries[i].value, word);
            } else {
                printf("%s\n", word);
            }
        }
    }
}
