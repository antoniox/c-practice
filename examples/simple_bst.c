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


int node_depth(Node * node) {
    int depth = 0;
    while (node->parent != NULL) {
        ++depth;
        node = node->parent;
    }
    return depth;
}


void simple_tree_traverse(Tree * tree, void (*visit_node)(int, float, int)) {
    void simple_visit_node(Node * node) {
        visit_node(
            *((int *)(node->key)),
            *((float *)(node->value)),
            node_depth(node)
        );
    }

    tree_traverse(tree, simple_visit_node, PREFIX);
}


void print_node(int key, float value, int depth) {
    printf("Node: key -> %d, value -> %f, depth -> %d\n", key, value, depth);
}


int main() {
    Tree tree;
    simple_tree_init(&tree);

    simple_tree_insert(&tree, 4, 1.0);
    simple_tree_insert(&tree, 3, 2.0);
    simple_tree_insert(&tree, 5, 1.0);
    simple_tree_insert(&tree, 7, 9.0);
    simple_tree_insert(&tree, 6, 9.0);
    simple_tree_insert(&tree, 9, 2.0);

    simple_tree_traverse(&tree, print_node);
    printf("==============================\n");

    simple_tree_remove(&tree, 9);

    simple_tree_traverse(&tree, print_node);
    printf("==============================\n");

    simple_tree_remove(&tree, 4);

    simple_tree_traverse(&tree, print_node);
    printf("==============================\n");

    tree_destroy(&tree);

    return 0;
}
