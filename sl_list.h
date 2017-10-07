
#pragma once

#include <stdbool.h>


typedef struct sl_node {
    struct sl_node *next;
    void *data;
} *sl_node_t;


typedef struct sl_list {
    struct sl_node sentinel; // The sentinel node of the list    
    unsigned int length;
} *sl_list_t;




sl_list_t sl_list_init(void **init_elems);


bool  sl_list_push(sl_list_t, void *elem);

void *sl_list_pop (sl_list_t);

void *sl_list_peek(sl_list_t);


bool sl_list_insert_before(sl_list_t, sl_node_t, void *elem);

bool sl_list_insert_after (sl_list_t, sl_node_t, void *elem);

bool sl_list_insert_by    (sl_list_t, void *elem, int (*compare)(void*, void*));


void *sl_list_get_nth(sl_list_t, unsigned int n);

bool  sl_list_set_nth(sl_list_t, void *elem, unsigned int n);


void sl_list_map(sl_list_t, void *(*f)(void *));

void sl_list_each(sl_list_t, void (*f)(void *));