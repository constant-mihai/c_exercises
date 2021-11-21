#include <gtest/gtest.h>

extern "C" {

#include "trie/trie.h"
#include "log/log.h"
}

class TestTrie: public ::testing::Test {
    protected:
        void SetUp() override {
            LOG_ADD_DEFAULT_MODULE("trie-test", L_INFO);
        }

        // void TearDown() override {}
};

TEST_F(TestTrie, TestInsert) {
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

TEST_F(TestTrie, TestSearch) {
    node_t trie;
    memset(trie.kv, 0, 10*sizeof(kv_t));

    const char *ip1 = "192.168.1.1";
    trie_insert(&trie, ip1);

    size_t i = 0;
    assert(trie_search_or_remove(&trie, ip1, SEARCH, &i) == 1);
}

TEST_F(TestTrie, TestRemove) {
    node_t trie;
    memset(trie.kv, 0, 10*sizeof(kv_t));

    const char *ip1 = "192.168.1.1";
    trie_insert(&trie, ip1);

    size_t i = 0;
    assert(trie_search_or_remove(&trie, ip1, REMOVE, &i) == 1);
    i = 0;
    assert(trie_search_or_remove(&trie, ip1, SEARCH, &i) == 0);
}
