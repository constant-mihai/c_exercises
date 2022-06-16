#include <gtest/gtest.h>

extern "C" {

#include "binary-tree/btree.h"
#include "log/log.h"
}

class TestBinaryTree: public ::testing::Test {
    protected:
        void SetUp() override {
            LOG_CREATE_DEFAULT("btree-tests", L_INFO);
        }

        // void TearDown() override {}
        int module_idx_g = 0;
};

TEST_F(TestBinaryTree, TestInsert) {
    node_t *root = NULL;
    LOG("Insert root");
    EXPECT_EQ(btree_insert(&root, NULL, 4), 0);
    LOG("Insert 1");
    assert(btree_insert(&root, NULL, 1) == 0);
    LOG("Insert 5");
    assert(btree_insert(&root, NULL, 5) == 0);

    LOG("Assert root");
    assert(root->val == 4);
    assert(root->up == NULL);
    assert(root->left != NULL);
    assert(root->right != NULL);

    LOG("Assert left");
    assert(root->left->up == root);
    assert(root->left->val == 1);

    LOG("Assert right");
    assert(root->right->up == root);
    assert(root->right->val == 5);
}

TEST_F(TestBinaryTree, TestSuccessor) {
    node_t *root = NULL;
    assert(btree_insert(&root, NULL, 4) == 0);
    assert(btree_insert(&root, NULL, 1) == 0);
    assert(btree_insert(&root, NULL, 10) == 0);
    assert(btree_insert(&root, NULL, 11) == 0);
    assert(btree_insert(&root, NULL, 5) == 0);

    node_t *suc = btree_get_successor_node(root);
    assert(suc != NULL);
    assert(suc->val == 5);
}

TEST_F(TestBinaryTree, TestRemove) {
    node_t *root = NULL;
    assert(btree_insert(&root, NULL, 4) == 0);
    assert(btree_insert(&root, NULL, 1) == 0);
    assert(btree_insert(&root, NULL, 10) == 0);
    assert(btree_insert(&root, NULL, 11) == 0);
    assert(btree_insert(&root, NULL, 5) == 0);

    assert(btree_remove(root, 1) == 0);
    assert(root->left == NULL);
    assert(btree_remove(root, 1) == 1);

    assert(btree_remove(root, 10) == 0);
    assert(root->right != NULL);
    assert(root->right->val == 11);

    assert(btree_remove(root, 10) == 1);
    assert(root->right != NULL);
    assert(root->right->val == 11);
}
