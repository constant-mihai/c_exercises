#include <gtest/gtest.h>

extern "C" {

#include "list/list.h"
#include "log/log.h"
}

class TestList: public ::testing::Test {
    protected:
        void SetUp() override {
            LOG_CREATE_DEFAULT("list-test", LOG_LEVEL_INFO);
        }

        // void TearDown() override {}
};

TEST_F(TestList, TestOperations) {
    list_t *l = list_create();
    int v[3] = { 11, 12, 13 };

    // {}
    list_insert(l, l->head, (void*)&v[0]);
    node_t *tmp = l->head;
    // { 11 }
    list_append(l, (void*)&v[2]);
    // { 11 13 }
    list_insert(l, tmp, (void*)&v[1]);
    // { 11 12 13 }
    list_foreach(l->head, print_node);

    list_remove(l, (void*)&v[1]);
    list_foreach(l->head, print_node);
    list_remove(l, (void*)&v[0]);
    list_foreach(l->head, print_node);
    list_remove(l, (void*)&v[2]);
    list_foreach(l->head, print_node);

    LOG("Test pop");
    // {}
    list_insert(l, l->head, (void*)&v[0]);
    tmp = l->head;
    // { 11 }
    list_append(l, (void*)&v[2]);
    // { 11 13 }
    list_insert(l, tmp, (void*)&v[1]);
    // { 11 12 13 }
    list_foreach(l->head, print_node);

    node_t *n = list_pop(l);
    assert(n != NULL);
    assert(*(int*)n->value == 13);
    list_foreach(l->head, print_node);
    free(n);

    n = list_pop(l);
    assert(n != NULL);
    assert(*(int*)n->value == 12);
    list_foreach(l->head, print_node);
    free(n);

    n = list_pop(l);
    assert(n != NULL);
    assert(*(int*)n->value == 11);
    list_foreach(l->head, print_node);
    free(n);

    list_destroy(&l);
}
