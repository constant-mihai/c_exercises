#pragma once

typedef struct node {
    void* value;
    struct node *next;
    struct node *prev;
} node_t;

typedef struct {
    node_t *head;
    node_t *tail;
} list_t;

extern list_t *list_create();
// insert after prev
extern void list_insert(list_t *list, node_t *prev, void* value);
extern void list_append(list_t *list, void* value);
extern int list_remove(list_t *n, void* value);
extern node_t *list_pop(list_t *n);
extern void list_destroy(list_t **list);

typedef void (*print_node_fn)(void *);
void print_node(void* value);
void list_foreach(node_t *n, print_node_fn pn);
