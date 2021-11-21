#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "list/list.h"
#include "log/log.h"

list *list_create() {
    list *l = (list*)malloc(sizeof(list));

    return l;
}

// insert after prev
int list_insert(node_t *prev, int value) {
    node_t *n = (node_t*)malloc(sizeof(node_t));

    n->next = prev->next;
    n->prev = prev;

    if (prev->next != NULL) {
        prev->next->prev = n;
    }
    prev->next = n;

    n->value = value;
    return 0;
}

int list_remove(node_t *n, int value) {
    (void) n;
    (void) value;

    return 0;
}

void print_node(int value) {
    LOG("value: %d", value);
    return;
}

void list_foreach(node_t *n, print_node_fn pn) {
    while(n != NULL) {
        pn(n->value);
        n = n->next;
    }
}
