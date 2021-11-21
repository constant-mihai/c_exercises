#pragma once

typedef struct node {
    int val;
    struct node *left;
    struct node *right;
    struct node *up;
} node_t;

void btree_init();

int btree_insert(node_t **n, node_t *parent, int val);

node_t *btree_get_successor_node(node_t *n);

void _btree_unlink(node_t *n1);

void _btree_reset_links(node_t *n1, node_t *n2);

void _btree_replace(node_t *n1, node_t *n2);

int _btree_remove_node(node_t *n);

int btree_remove(node_t *n, int val);

int btree_find(node_t *n, int val);

int btree_dump(node_t *n);

