#include <gtest/gtest.h>

extern "C" {

#include "trie-dict/trie-dict.h"
#include "log/log.h"
}

class TestTrieDict: public ::testing::Test {
    protected:
        void SetUp() override {
            LOG_ADD_DEFAULT_MODULE("trie-dict-test", L_INFO);
        }

        // void TearDown() override {}
};

TEST_F(TestTrieDict, TestSearch) {
    node_t *trie = trie_create();

    const char *word[] = {
        "a",
        "alpha",
        "alphabet",
        "bat",
        "baton",
        "battery",
        "battering",
        "croissant",
    };

    for (size_t i=0; i < sizeof(word)/sizeof(word[0]); i++) {
        LOG("inserting %s", word[i]);
        if (trie_dict_insert(trie, word[i]) != 0) {
            LOG("Error inserting %s", word[i]);
        }
    }

    printf("Printing tree\n");
    const char *prefix = 0;
    trie_print(trie, prefix);
    printf("\n");

    for (size_t i=0; i < sizeof(word)/sizeof(word[0]); i++) {
        //size_t count = 0;
        //assert(trie_dict_search_or_remove(&trie, word[i], SEARCH, &count) == 1);
    }
}

TEST_F(TestTrieDict, TestRemove) {
    node_t *trie = trie_create(); 

    const char *word[] = {
        "alphabet",
        "bat",
        "baton",
        "battery",
    };

    for (size_t i=0; i < sizeof(word)/sizeof(word[0]); i++) {
        if (trie_dict_insert(trie, word[i]) != 0) {
            LOG("Error inserting %s", word[i]);
        }
    }

    for (size_t i=0; i < sizeof(word)/sizeof(word[0]); i++) {
        //size_t count = 0;
        //assert(trie_dict_search_or_remove(&trie, word[i], REMOVE, &count) == 1);
    }

    for (size_t i=0; i < sizeof(word)/sizeof(word[0]); i++) {
        //size_t count = 0;
        //assert(trie_dict_search_or_remove(&trie, word[i], REMOVE, &count) == 0);
    }
}
