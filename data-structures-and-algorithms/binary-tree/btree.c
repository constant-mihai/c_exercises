#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__)

typedef struct node {
    int val;
    struct node *left;
    struct node *right;
    struct node *up;
} node_t;

int btree_insert(node_t **n, node_t *parent, int val) {
    if (n == NULL) {
        LOG("n cannot be null.");
        return -1;
    }

    if (*n == NULL) {
        LOG("Creating leaf node: %d", val);
        *n = (node_t*)calloc(1, sizeof(node_t));
        if (*n == NULL) {
            LOG("oom");
            return -1;
        }
        (*n)->val = val;
        (*n)->up = parent;
    } else {
        LOG("node exists, check left/right");
        node_t **left = &(*n)->left;
        node_t **right = &(*n)->right;
        if ((*n)->val > val) {
            LOG("%d > %d",(*n)->val ,val);
            LOG("Insert node left");
            btree_insert(left, *n, val);
        } else if ((*n)->val < val) {
            LOG("Insert node right");
            LOG("%d < %d",(*n)->val ,val);
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

void _btree_unlink(node_t *n1) {
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

void _btree_reset_links(node_t *n1, node_t *n2) {
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

void _btree_replace(node_t *n1, node_t *n2) {
    // attach n2 to new neighbours.
    n2->up = n1->up;
    n2->left = n1->left;
    n2->right= n1->right;

    _btree_reset_links(n1, n2);

    n1->left->up = n2;
    n1->right->up = n2;
    return;
}

int _btree_remove_node(node_t *n) {
    LOG("remove: %d", n->val);
    // if leaf delete.
    if (n->left == NULL && n->right == NULL) {
        LOG("leaf node");
        _btree_unlink(n);
        free(n);
        return 0;
    }

    // if it has two children find successor
    if (n->left != NULL && n->right != NULL) {
        LOG("%d node has two children", n->val);
        node_t *suc = btree_get_successor_node(n);
        LOG("successor is: %d", suc->val);
        _btree_replace(n, suc);
        // n will now be a leaf and can be removed.
        free(n);
        return 0;
    }

    // if has child swap and delete
    if (n->left != NULL) {
        LOG("has left child");
        n->val = n->left->val;
        free(n->left);
        return 0;
    } else if (n->right != NULL) {
        LOG("has right child");
        n->val = n->right->val;
        free(n->right);
        return 0;
    }

    return 1;
}

int btree_remove(node_t *n, int val) {
    if (n == NULL) {
        LOG("value: %d not found", val);
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

int btree_dump(node_t *n);

void btree_test_insert() {
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

void btree__test_successor() {
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

void btree_test_remove() {
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

/* int main(int argc, char ** argv) {*/
/*     LOG("main");*/
/*     btree_insert();*/
/*     btree_successor();*/
/*     btree_remove();*/

/*     const char* str1 = "abc";*/
/*     const char* str2 = "abd";*/

/*     LOG("cmp: %d", strcmp(str1, str2));*/

/*     return 0;*/
/* }*/
