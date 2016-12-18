#include "stdio.h"
#include "stdlib.h"

#include "bst.h"
#include "deque.h"


int compare_keys(void * left, void * right) {
    int left_value = *((int *) left);
    int right_value = *((int *) right);
    
    if (left_value == right_value) {
        return 0;
    }

    if (left_value < right_value) {
        return -1;
    }

    return 1;
}


void simple_tree_init(Tree * tree) {
    tree_init(tree, compare_keys, default_destroy_node_hook);
}


void simple_tree_insert(Tree * tree, int key) {
    int * key_pointer = (int *)malloc(sizeof(int));
    *key_pointer = key;

    int * value_pointer = (int *)malloc(sizeof(int));
    *value_pointer = 0;
    tree_insert(tree, key_pointer, value_pointer);
}


void check_tree(Tree * tree, int target_sum) {
    Deque deque;
    deque_init(&deque, default_destroy_list_node);

    deque_pushfront(&deque, tree->root);

    Node * node;
    int has_path = 0;

    while ((node = (Node *)deque_front(&deque)) != NULL) {
        deque_popfront(&deque);

        int weight = *((int *)node->key);
        if (node->parent != NULL) {
            weight += *((int *)node->parent->value);
        }

        *((int *)node->value) = weight;

        if (weight == target_sum && node->left == NULL && node->right == NULL) {
            has_path = 1;
            break;
        }

        if (node->left != NULL) {
            deque_pushfront(&deque, node->left);
        }

        if (node->right != NULL) {
            deque_pushfront(&deque, node->right);
        }
    }

    deque_destroy(&deque);

    if (has_path) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
}


int main() {
    Tree tree;
    simple_tree_init(&tree);

    simple_tree_insert(&tree, 4);
    simple_tree_insert(&tree, 3);
    simple_tree_insert(&tree, 5);
    simple_tree_insert(&tree, 7);
    simple_tree_insert(&tree, 6);
    simple_tree_insert(&tree, 9);

    int target_sum = 22;
    check_tree(&tree, target_sum);

    tree_destroy(&tree);

    return 0;
}
