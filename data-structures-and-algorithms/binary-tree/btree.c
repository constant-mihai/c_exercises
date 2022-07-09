#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "log/log.h"

#include "btree.h"

static void _btree_unlink(node_t *n1) {
    assert(n1 != NULL);
    if (n1->up != NULL) {
        if (n1->up->val > n1->val) {
            n1->up->left = NULL;
        }
        if (n1->up->val < n1->val) {
            n1->up->right= NULL;
        }
    }

    n1->up = NULL;
    n1->left = NULL;
    n1->right = NULL;
}

static void _btree_reset_links(node_t *n1, node_t *n2) {
    // attach n1 neighbours to n2
    if (n1->up != NULL) {
        if (n1->up->val > n2->val) {
            n1->up->left = n2;
        }
        if (n1->up->val < n2->val) {
            n1->up->right = n2;
        }
    }
}

static void _btree_replace(node_t *n1, node_t *n2) {
    // attach n2 to new neighbours.
    n2->up = n1->up;
    n2->left = n1->left;
    n2->right= n1->right;

    _btree_reset_links(n1, n2);

    n1->left->up = n2;
    n1->right->up = n2;
    return;
}

static int _btree_remove_node(node_t *n) {
    HR_LOG("remove: %d", n->val);
    // if leaf delete.
    if (n->left == NULL && n->right == NULL) {
        HR_LOG("leaf node");
        _btree_unlink(n);
        free(n);
        return 0;
    }

    // if it has two children find successor
    if (n->left != NULL && n->right != NULL) {
        HR_LOG("%d node has two children", n->val);
        node_t *suc = btree_get_successor_node(n);
        HR_LOG("successor is: %d", suc->val);
        _btree_replace(n, suc);
        // n will now be a leaf and can be removed.
        free(n);
        return 0;
    }

    // if has child swap and delete
    if (n->left != NULL) {
        HR_LOG("has left child");
        n->val = n->left->val;
        free(n->left);
        return 0;
    } else if (n->right != NULL) {
        HR_LOG("has right child");
        n->val = n->right->val;
        free(n->right);
        return 0;
    }

    return 1;
}

void btree_init() {
    LOG_CREATE_DEFAULT("btree", LOG_LEVEL_INFO);
}

int btree_insert(node_t **n, node_t *parent, int val) {
    if (n == NULL) {
        HR_LOG("n cannot be null.");
        return -1;
    }

    if (*n == NULL) {
        HR_LOG("Creating leaf node: %d", val);
        *n = (node_t*)calloc(1, sizeof(node_t));
        if (*n == NULL) {
            HR_LOG("oom");
            return -1;
        }
        (*n)->val = val;
        (*n)->up = parent;
    } else {
        HR_LOG("node exists, check left/right");
        node_t **left = &(*n)->left;
        node_t **right = &(*n)->right;
        if ((*n)->val > val) {
            HR_LOG("%d > %d",(*n)->val ,val);
            HR_LOG("Insert node left");
            btree_insert(left, *n, val);
        } else if ((*n)->val < val) {
            HR_LOG("Insert node right");
            HR_LOG("%d < %d",(*n)->val ,val);
            btree_insert(right, *n, val);
        }

    }
    return 0;
}

node_t *btree_get_successor_node(node_t *n) {
    assert(n != NULL);

    // find left most leaf of the node to the right.
    node_t *successor = n->right;
    assert(successor != NULL);

    while (successor->left != NULL) {
        successor = successor->left;
    }

    return successor;
}

int btree_remove(node_t *n, int val) {
    if (n == NULL) {
        HR_LOG("value: %d not found", val);
        return 1;
    }

    int ret = 1;
    if (n->val > val) ret = btree_remove(n->left, val);
    if (n->val < val) ret = btree_remove(n->right, val);
    if (n->val == val) ret = _btree_remove_node(n);

    return ret;
}

// TODO
int btree_find(node_t *n, int val) {
    (void) n;
    (void) val;

    return 0;
}

// TODO
int btree_dump(node_t *n) {
    (void) n;

    return 0;
}
