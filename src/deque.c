#include "stdlib.h"

#include "deque.h"


void deque_init(Deque * deque, void (*destroy_node)(ListNode *)) {
    deque->begin = NULL;
    deque->end = NULL;
    deque->destroy_node = destroy_node;
}

void destroy(ListNode * node, void (*destroy_node)(ListNode *)) {
    ListNode * next = node->next;

    destroy_node(node);
    free(node);

    if (next != NULL) {
        destroy(next, destroy_node);
    }
}


void deque_destroy(Deque * deque) {
    if (deque->begin != NULL) {
        destroy(deque->begin, deque->destroy_node);
    }
}


void deque_pushfront(Deque * deque, void * value) {
    ListNode * new_node = (ListNode *)malloc(sizeof(ListNode));
    new_node->value = value;
    new_node->prev = NULL;

    if (deque->begin == NULL) {
        deque->begin = new_node;
        deque->end = new_node;
        new_node->next = NULL;
    } else {
        new_node->next = deque->begin;
        (new_node->next)->prev = new_node;
        deque->begin = new_node;
    }
}


void * deque_front(Deque * deque) {
    if (deque->begin == NULL) {
        return NULL;
    }

    return (deque->begin)->value;
}


void deque_popfront(Deque * deque) {
    if (deque->begin == NULL) {
        return;
    }

    if (deque->begin == deque->end) {
        deque->end = NULL;
        deque->destroy_node(deque->begin);
        free(deque->begin);
        deque->begin = NULL;
    } else {
        ListNode * node = deque->begin;
        deque->begin = node->next;
        (node->next)->prev = NULL;
        deque->destroy_node(node);
        free(node);
    }
}


void deque_pushback(Deque * deque, void * value) {
    ListNode * new_node = (ListNode *)malloc(sizeof(ListNode));
    new_node->value = value;
    new_node->next = NULL;

    if (deque->end == NULL) {
        deque->begin = new_node;
        deque->end = new_node;
        new_node->prev = NULL;
    } else {
        new_node->prev = deque->end;
        (new_node->prev)->next = new_node;
        deque->end = new_node;
    }
}


void * deque_back(Deque * deque) {
    if (deque->end == NULL) {
        return NULL;
    }

    return (deque->end)->value;
}


void deque_popback(Deque * deque) {
    if (deque->end == NULL) {
        return;
    }

    if (deque->begin == deque->end) {
        deque->begin = NULL;
        deque->destroy_node(deque->end);
        free(deque->end);
        deque->end = NULL;
    } else {
        ListNode * node = deque->end;
        deque->end = node->prev;
        (node->prev)->next = NULL;
        deque->destroy_node(node);
        free(node);
    }
}


void default_destroy_list_node(ListNode * node) {}
