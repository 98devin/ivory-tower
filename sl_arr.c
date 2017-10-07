
#include <stdlib.h>
#include <stdbool.h>

#include "sl_arr.h"

#define INITIAL_CAPACITY 8
#define GROWTH_FACTOR 2



sl_arr_t sl_arr_init(void **init_elems) {

    sl_arr_t sl = malloc(sizeof(struct sl_arr));
    if (sl == NULL) return NULL;

    sl->data = malloc(INITIAL_CAPACITY * sizeof(void *));
    if (sl->data == NULL) return NULL;

    sl->capacity = INITIAL_CAPACITY;
    sl->length   = 0;
    sl->init_pos = 0;

    while (init_elems) {
        sl_arr_push(sl, *init_elems++);
    }

    return sl;
}


static bool sl_arr_grow(sl_arr_t sl) {
    
    unsigned long new_cap = sl->capacity * GROWTH_FACTOR;

    void **new_data = malloc(new_cap * sizeof(void *));
    
    for (unsigned long i = sl->init_pos, j = 0; i < sl->length; i++, i %= sl->capacity) {
        new_data[j++] = sl->data[i];
    }

    free(sl->data);
    sl->data     = new_data;
    sl->capacity = new_cap;
    sl->init_pos = 0;

    return true;
}


bool sl_arr_push(sl_arr_t sl, void *elem) {

    if (sl->length + 1 > sl->capacity) {
        if (!sl_arr_grow(sl)) return false;
    }

    sl->data[(sl->init_pos + sl->length++) % sl->capacity] = elem;
    return true;
}


void *sl_arr_pop(sl_arr_t sl) {

    void *data = sl->data[(sl->init_pos + sl->length - 1) % sl->capacity];
    sl->length--;

    return data;

}


void *sl_arr_peek(sl_arr_t sl) {
    return sl->data[(sl->init_pos + sl->length - 1) % sl->capacity];
}


inline bool sl_arr_append(sl_arr_t sl, void *elem) {
    return sl_arr_push(sl, elem);
}


inline bool sl_arr_enqueue(sl_arr_t sl, void *elem) {
    return sl_arr_push(sl, elem);
}


void *sl_arr_dequeue(sl_arr_t sl) {

    void *data = sl->data[sl->init_pos++];

    sl->init_pos %= sl->capacity;
    sl->length--;

    return data;

}


void *sl_arr_get_nth(sl_arr_t sl, unsigned int n) {
    if (n > sl->length) return NULL;
    return sl->data[(sl->init_pos + n) % sl->capacity];
}


bool sl_arr_set_nth(sl_arr_t sl, void *elem, unsigned int n) {
    if (n > sl->length) return false;
    sl->data[(sl->init_pos + n) % sl->capacity] = elem;
    return true;
}


void sl_arr_map(sl_arr_t sl, void *(*f)(void *)) {
    for (unsigned long i = 0; i < sl->length; i++, i %= sl->capacity) {
        sl->data[i] = f(sl->data[i]);
    }
}


void sl_arr_each(sl_arr_t sl, void (*f)(void *)) {
    for (unsigned long i = 0; i < sl->length; i++, i %= sl->capacity) {
        f(sl->data[i]);
    }
}

