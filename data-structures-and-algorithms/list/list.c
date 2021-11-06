#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define LOG_INFO "[%s:%d]: "
#define LOG_INFO_VAL __func__,__LINE__
#define LOG(msg, ...) printf(LOG_INFO msg "\n", LOG_INFO_VAL, ##__VA_ARGS__)

typedef struct node {
    int value;
    struct node *next;
    struct node *prev;
} node_t;

typedef struct {
    node_t *head;
    node_t *tail;
} list;

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

int list_remove(node_t *n, int value);

typedef void (*print_node_fn)(int);

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

int main(int argc, char ** argv) {
    LOG("main");
    list *l = list_create();
    l->head = (node_t*)calloc(1, sizeof(node_t));
    l->head->value = 10;
    l->tail = l->head;

    int err = list_insert(l->head, 11);
    if (err != 0) {
        LOG("error inserting");
        return -1;
    }

    list_foreach(l->head, print_node);

    return 0;
}
