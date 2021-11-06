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
        //LOG("%c:", ip[i]);
        char c = ip[i];
        if ( c == '.' ) {
            continue;
        }
        int p = atoi(&c);
        LOG("%d:", p);
        assert(p >= 0 && p < 10);
        trie->kv[p].child = (node_t*)calloc(1, sizeof(node_t));
        trie = trie->kv[p].child;
    }
    return 0;
}

int trie_search(node_t *trie, const char* ip, size_t *i) {
    assert(i!=NULL);
    LOG("iteration %lu:", *i);
    char c = ip[*i];
    //TODO check here for syntax errors;
    if ( c == '.' ) {
        (*i)++;
        c = ip[*i];
    }
    int p = atoi(&c);
    assert(p >= 0 && p < 10);
    if (trie->kv[p].child != NULL) {
        (*i)++;
        return trie_search(trie->kv[p].child, ip, i);
    }
    return 1;
}

int trie_remove(node_t *trie, const char* ip) {
    size_t i = 0;
    if (trie_search(trie, ip, &i) == 0) {
        LOG("Full match, removing");

        // TODO
        return 0;
    }

    return 1;
}

void test_search() {
    node_t trie;
    memset(trie.kv, 0, 10*sizeof(kv_t));

    const char *ip1 = "192.168.1.1";
    trie_insert(&trie, ip1);

    size_t i = 0;
    assert(trie_search(&trie, ip1, &i) == 0);
}

void test_insert() {
    node_t trie;
    memset(trie.kv, 0, 10*sizeof(kv_t));

    const char *ip1 = "192.168.1.1";
    trie_insert(&trie, ip1);

    assert(trie.kv[0].child == NULL);
    assert(trie.kv[1].child != NULL);
    assert(trie.kv[1].child->kv[0].child == NULL);
    assert(trie.kv[1].child->kv[1].child == NULL);
    assert(trie.kv[1].child->kv[9].child != NULL);
}

int main(int argc, char ** argv) {
    LOG("main");
    //test_insert();
    test_search();
    //test_remove();

    return 0;
}
