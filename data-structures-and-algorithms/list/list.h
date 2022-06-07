#pragma once

typedef struct node {
    int value;
    struct node *next;
    struct node *prev;
} node_t;

typedef struct {
    node_t *head;
    node_t *tail;
} list_t;

extern list_t *list_create();

// insert after prev
extern void list_insert(list_t *list, node_t *prev, int value);

extern void list_append(list_t *list, int value);

extern int list_remove(list_t *n, int value);

extern node_t *list_pop(list_t *n);

typedef void (*print_node_fn)(int);

void print_node(int value);

void list_foreach(node_t *n, print_node_fn pn);
