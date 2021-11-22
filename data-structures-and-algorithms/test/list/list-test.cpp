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

TEST_F(TestList, TestInsert) {
    list *l = list_create();
    l->head = (node_t*)calloc(1, sizeof(node_t));
    l->head->value = 10;
    l->tail = l->head;

    int err = list_insert(l->head, 11);
    assert(err != 0);

    list_foreach(l->head, print_node);
}
