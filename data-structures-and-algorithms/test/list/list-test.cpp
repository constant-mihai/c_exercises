#include <gtest/gtest.h>

extern "C" {

#include "list/list.h"
#include "log/log.h"
}

class TestList: public ::testing::Test {
    protected:
        void SetUp() override {
            LOG_ADD_DEFAULT_MODULE("list-test", L_INFO);
        }

        // void TearDown() override {}
};

TEST_F(TestList, TestOperations) {
    list_t *l = list_create();

    // {}
    list_insert(l, l->head, 11);
    node_t *tmp = l->head;
    // { 11 }
    list_append(l, 13);
    // { 11 13 }
    list_insert(l, tmp, 12);
    // { 11 12 13 }
    list_foreach(l->head, print_node);

    list_remove(l, 12);
    list_foreach(l->head, print_node);
    list_remove(l, 11);
    list_foreach(l->head, print_node);
    list_remove(l, 13);
    list_foreach(l->head, print_node);

    LOG("Test pop");
    // {}
    list_insert(l, l->head, 11);
    tmp = l->head;
    // { 11 }
    list_append(l, 13);
    // { 11 13 }
    list_insert(l, tmp, 12);
    // { 11 12 13 }
    list_foreach(l->head, print_node);

    node_t *n = list_pop(l);
    assert(n != NULL);
    assert(n->value == 13);
    list_foreach(l->head, print_node);
    free(n);

    n = list_pop(l);
    assert(n != NULL);
    assert(n->value == 12);
    list_foreach(l->head, print_node);
    free(n);

    n = list_pop(l);
    assert(n != NULL);
    assert(n->value == 11);
    list_foreach(l->head, print_node);
    free(n);
}
