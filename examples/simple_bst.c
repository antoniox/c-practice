#include "stdio.h"
#include "stdlib.h"

#include "bst.h"


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


void destroy_node_hook(Node * node) {
    free(node->key);
    free(node->value);
}


void simple_tree_init(Tree * tree) {
    tree_init(
        tree, compare_keys,
        destroy_node_hook,
        default_insert_node_hook,
        default_remove_node_hook
    );
}


void simple_tree_insert(Tree * tree, int key, float value) {
    int * key_pointer = (int *)malloc(sizeof(int));
    *key_pointer = key;

    float * value_pointer = (float *)malloc(sizeof(float));
    *value_pointer = value;

    tree_insert(tree, key_pointer, value_pointer);
}


void simple_tree_remove(Tree * tree, int key) {
    int * key_pointer = (int *)malloc(sizeof(int));
    *key_pointer = key;

    tree_remove(tree, key_pointer);
    free(key_pointer);
}


int main() {
    Tree tree;
    simple_tree_init(&tree);

    simple_tree_insert(&tree, 4, 1.0);
    simple_tree_insert(&tree, 3, 2.0);
    simple_tree_insert(&tree, 5, 1.0);
    simple_tree_insert(&tree, 7, 9.0);
    simple_tree_insert(&tree, 9, 2.0);

    simple_tree_remove(&tree, 9);
    simple_tree_remove(&tree, 4);

    tree_destroy(&tree);

    return 0;
}
