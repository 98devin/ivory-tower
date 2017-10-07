
#include <stdlib.h>
#include <stdbool.h>
#include "sl_list.h"


sl_list_t sl_list_init(void **init_elems) {
    sl_list_t sl = malloc(sizeof(struct sl_list));
    if (sl == NULL) return NULL;

    sl->sentinel.next = NULL;
    sl->sentinel.data = NULL;
    sl->length = 0;

    while (init_elems != NULL) {
        sl_list_push(sl, *init_elems++);
    }

    return sl;

}


bool sl_list_push(sl_list_t sl, void *elem) {
    
    sl_node_t n = (sl_node_t) sl;
    
    sl_node_t new_node = malloc(sizeof(struct sl_node));
    if (new_node == NULL) return false;

    new_node->next = n->next;
    new_node->data = elem;
    n->next = new_node;

    sl->length++;

    return true;
}


void *sl_list_pop(sl_list_t sl) {
    if (sl->length == 0) return NULL;

    sl_node_t n = (sl_node_t) sl;
    sl_node_t head = n->next;

    void *data = head->data;
    n->next = head->next;
    free(head);

    sl->length--;

    return data;
}


void *sl_list_peek(sl_list_t sl) {
    if (sl->length == 0) return NULL;
    return sl->sentinel.next->data;
}


bool sl_list_insert_before(sl_list_t sl, sl_node_t no, void *elem) {
    
    sl_node_t n = (sl_node_t) sl;
    
    while (n->next != NULL) {
        if (n->next == no) {
            
            sl_node_t new = malloc(sizeof(struct sl_node));
            if (new == NULL) return false;
            
            new->next = no;
            new->data = elem;

            n->next = new;
            sl->length++;

            return true;
        }
    }

    return false;

}


bool sl_list_insert_after(sl_list_t sl, sl_node_t no, void *elem) {
 
    sl_node_t n = (sl_node_t) sl;

    while (n->next != NULL) {
        if (n->next == no) {

            sl_node_t new = malloc(sizeof(struct sl_node));
            if (new == NULL) return false;

            new->next = no->next;
            new->data = elem;

            no->next = new;
            sl->length++;
            
            return true;

        }
    }

    return false;

}


bool sl_list_insert_by(sl_list_t sl, void *elem, int (*compare)(void*, void*)) {

    sl_node_t n = (sl_node_t) sl;

    while (n->next != NULL && compare(elem, n->next->data) < 0) {
        n = n->next;
    }

    // now `elem` should be inserted between `n` and `n->next`
    
    sl_node_t new = malloc(sizeof(struct sl_node));
    if (new == NULL) return false;

    new->next = n->next;
    new->data = elem;

    n->next = new;
    sl->length++;

    return true;

}


void *sl_list_get_nth(sl_list_t sl, unsigned int n) {

    if (sl->length <= n) return NULL;

    sl_node_t sent = (sl_node_t) sl;

    for (unsigned int i = 0; i < n; i++) {
        sent = sent->next;
    }

    return sent->next->data;

}


bool sl_list_set_nth(sl_list_t sl, void *elem, unsigned int n) {

    if (sl->length <= n) return false;

    sl_node_t sent = (sl_node_t) sl;

    for (unsigned int i = 0; i < n; i++) {
        sent = sent->next;
    }

    sent->next->data = elem;

    return true;

}


void sl_list_map(sl_list_t sl, void *(*f)(void *)) {

    sl_node_t n = sl->sentinel.next;

    while (n != NULL) {
        n->data = f(n->data);
        n = n->next;
    }

}


void sl_list_each(sl_list_t sl, void (*f)(void *)) {

    sl_node_t n = sl->sentinel.next;

    while (n != NULL) {
        f(n->data);
        n = n->next;
    }

}