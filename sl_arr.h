
#pragma once

#include <stdbool.h>


typedef struct sl_arr {
    unsigned long capacity;
    unsigned long length;
    unsigned long init_pos;
    void **data;
} *sl_arr_t;




sl_arr_t sl_arr_init(void **init_elems);


bool  sl_arr_push(sl_arr_t, void *elem);

void *sl_arr_pop(sl_arr_t);

void *sl_arr_peek(sl_arr_t);


bool sl_arr_append(sl_arr_t, void *elem);


bool  sl_arr_enqueue(sl_arr_t, void *elem);

void *sl_arr_dequeue(sl_arr_t);


void *sl_arr_get_nth(sl_arr_t, unsigned int n);

bool  sl_arr_set_nth(sl_arr_t, void *elem, unsigned int n);


void sl_arr_map (sl_arr_t, void *(*f)(void *));

void sl_arr_each(sl_arr_t, void  (*f)(void *));
