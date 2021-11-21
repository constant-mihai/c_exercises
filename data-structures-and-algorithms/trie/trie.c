#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "trie/trie.h"
#include "log/log.h"

int trie_insert(node_t *trie, const char* ip) {
    for (size_t i=0; i<strlen(ip); i++) {
        char c = ip[i];
        if ( c == '.' ) {
            continue;
        }
        int p = atoi(&c);
        //LOG("inserting %d", p);
        assert(p >= 0 && p < 10);
        trie->kv[p].child = (node_t*)calloc(1, sizeof(node_t));
        trie = trie->kv[p].child;
    }
    return 0;
}

int trie_search_or_remove(node_t *trie,
                           const char* ip,
                           trie_sor_e sor,
                           size_t *i) {
    assert(i!=NULL);
    char c = ip[*i];
    int found = 0;
    LOG("iteration %lu: %c", *i, c);

    //TODO check here for syntax errors
    if ( c == '.' ) {
        (*i)++;
        c = ip[*i];
        LOG("iteration    %c",  c);
    }
    int p = atoi(&c);
    // TODO if p is the first digit then check p != 0

    assert(p >= 0 && p < 10);
    if (trie->kv[p].child != NULL) {
        (*i)++;
        found = trie_search_or_remove(trie->kv[p].child, ip, sor, i);
        if (found == 1 && sor == REMOVE) {
            free(trie->kv[p].child);
            trie->kv[p].child = NULL;
        }
    }
    return (strlen(ip) == (*i)) ? 1 : 0;
}
