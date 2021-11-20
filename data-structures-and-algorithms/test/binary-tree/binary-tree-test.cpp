#include <gtest/gtest.h>

#include "binary-tree/btree.h"
#include "log/log.h"

int module_idx_g = 0;

// Demonstrate some basic assertions.
TEST(TestBinaryTree, TestInsert) {
    LOG_ADD_DEFAULT_MODULE("btree-tests", "unit-tests", L_INFO);
    node_t *root = NULL;
    LOG("Insert root");
    assert(btree_insert(&root, NULL, 4) == 0);
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
