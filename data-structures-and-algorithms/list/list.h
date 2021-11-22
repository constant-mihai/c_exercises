#pragma once

typedef struct node {
    int value;
    struct node *next;
    struct node *prev;
} node_t;

typedef struct {
    node_t *head;
    node_t *tail;
} list;

list *list_create();

// insert after prev
int list_insert(node_t *prev, int value);

int list_remove(node_t *n, int value);

typedef void (*print_node_fn)(int);

void print_node(int value);

void list_foreach(node_t *n, print_node_fn pn);
