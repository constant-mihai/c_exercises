#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__)

typedef struct kv {
    struct node *child;
} kv_t;

typedef struct node {
    kv_t kv[10];
} node_t;

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

typedef enum {
    SEARCH,
    REMOVE,
}sor_e;

int trie_search_or_remove(node_t *trie,
                           const char* ip,
                           sor_e sor,
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

void trie_test_search() {
    node_t trie;
    memset(trie.kv, 0, 10*sizeof(kv_t));

    const char *ip1 = "192.168.1.1";
    trie_insert(&trie, ip1);

    size_t i = 0;
    assert(trie_search_or_remove(&trie, ip1, SEARCH, &i) == 1);
}

void trie_test_remove() {
    node_t trie;
    memset(trie.kv, 0, 10*sizeof(kv_t));

    const char *ip1 = "192.168.1.1";
    trie_insert(&trie, ip1);

    size_t i = 0;
    assert(trie_search_or_remove(&trie, ip1, REMOVE, &i) == 1);
    i = 0;
    assert(trie_search_or_remove(&trie, ip1, SEARCH, &i) == 0);
}

void trie_test_insert() {
    node_t trie;
    memset(trie.kv, 0, 10*sizeof(kv_t));

    const char *ip1 = "192.168.1.1";
    trie_insert(&trie, ip1);

    assert(trie.kv[0].child == NULL);
    assert(trie.kv[1].child != NULL);
    assert(trie.kv[1].child->kv[0].child == NULL);
    assert(trie.kv[1].child->kv[1].child == NULL);
    assert(trie.kv[1].child->kv[9].child != NULL);
    assert(trie.kv[1].child->kv[9].child->kv[2].child != NULL);
    assert(trie.kv[1].child->kv[9].child->kv[2].child->kv[0].child == NULL);
    assert(trie.kv[1].child->kv[9].child->kv[2].child->kv[1].child != NULL);
}

/* int main() {*/
/*     LOG("main");*/
/*     trie_testinsert();*/
/*     trie_testsearch();*/
/*     trie_testremove();*/

/*     return 0;*/
/* }*/
