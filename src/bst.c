#include "stdlib.h"

#include "bst.h"


void tree_init(
    Tree * tree,
    int (*compare_keys)(void *, void *), 
    void (*destroy_node_hook)(Node *),
    void (*insert_node_hook)(Node *),
    Node * (*remove_node_hook)(Node *)
) {
    tree->root = NULL;
    tree->compare_keys = compare_keys;
    tree->destroy_node_hook = destroy_node_hook;
    tree->insert_node_hook = insert_node_hook;
    tree->remove_node_hook = remove_node_hook;
}


void subtree_destroy(Node * node, void (*destroy_node_hook)(Node *)) {
    if (node->left != NULL) {
        subtree_destroy(node->left, destroy_node_hook);
    }

    if (node->right != NULL) {
        subtree_destroy(node->right, destroy_node_hook);
    }

    destroy_node_hook(node);
    free(node);
}


void tree_destroy(Tree * tree) {
    if (tree->root != NULL) {
        subtree_destroy(tree->root, tree->destroy_node_hook);
    }

    tree->root = NULL;
    tree->compare_keys = NULL;
    tree->insert_node_hook = NULL;
    tree->remove_node_hook = NULL;
    tree->destroy_node_hook = NULL;
}


void subtree_insert(
    Node * node, Node * new_node,
    int (*compare_keys)(void *, void *)
) {
    int cmp = compare_keys(new_node->key, node->key);

    if (cmp == 1 && node->right == NULL) {
        node->right = new_node;
        new_node->parent = node;
    } else if (cmp == -1 && node->left == NULL) {
        node->left = new_node;
        new_node->parent = node;
    } else {
        subtree_insert(
            cmp == 1 ? node->right : node->left,
            new_node, compare_keys
        );
    }
}


void tree_insert(Tree * tree, void * key, void * value) {
    Node * new_node = (Node *) malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;

    if (tree->root == NULL) {
        tree->root = new_node;
        new_node->parent = NULL;
    } else {
        subtree_insert(tree->root, new_node, tree->compare_keys);
    }

    tree->insert_node_hook(new_node);
}


Node * subtree_lookup(
    Node * node, void * key,
    int (*compare_keys)(void *, void *)
) {
    int cmp = compare_keys(key, node->key);

    if (cmp == 0) {
        return node;
    }

    Node * candidate = cmp == 1 ? node->right : node->left;

    if (candidate != NULL) {
        return subtree_lookup(candidate, key, compare_keys);
    }

    return NULL;
}


void * tree_lookup(Tree * tree, void * key) {
    if (tree->root == NULL) {
        return NULL;
    }

    Node * node = subtree_lookup(tree->root, key, tree->compare_keys);
    if (node == NULL) {
        return NULL;
    }

    return node->value;
}


void tree_remove(Tree * tree, void * key) {
    if (tree->root == NULL) {
        return;
    }

    Node * node = subtree_lookup(tree->root, key, tree->compare_keys);
    if (node == NULL) {
        return;
    }

    Node * possible_root = tree->remove_node_hook(node);

    if (node == tree->root) {
        tree->root = possible_root;
    }

    tree->destroy_node_hook(node);
}


void default_destroy_node_hook(Node * node) {}
void default_insert_node_hook(Node * node) {}


Node * default_remove_node_hook(Node * node) {
    Node * candidate = NULL;

    if (node->left == NULL) {
        candidate = node->right;
    } else if (node->right == NULL) {
        candidate = node->left;
    } else {
        // pick smallest key which is bigger than node->key
        candidate = node->left;
        while (candidate->right != NULL) {
            candidate = candidate->right;
        }
    }

    Node * parent = node->parent;
    if (parent != NULL) {
        if (parent->left == node) {
            parent->left = candidate;
        } else {
            parent->right = candidate;
        }
    }

    if (candidate == NULL) {
        return NULL;
    }

    Node * candidate_parent = candidate->parent;
    if (candidate_parent->left == candidate) {
        candidate_parent->left = NULL;
    } else {
        candidate_parent->right = NULL;
    }

    candidate->parent = node->parent;
    candidate->left = node->left;
    candidate->right = node->right;

    return candidate;
}


void subtree_traverse(
    Node * node,
    void (*visit_node)(Node *), Order order
) {
    if (order == PREFIX) {
        visit_node(node);
    }

    if (node->left != NULL) {
        subtree_traverse(node->left, visit_node, order);
    }

    if (order == INFIX) {
        visit_node(node);
    }

    if (node->right != NULL) {
        subtree_traverse(node->right, visit_node, order);
    }

    if (order == POSTFIX) {
        visit_node(node);
    }
}


void tree_traverse(
    Tree * tree,
    void (*visit_node)(Node *),
    Order order
) {
    if (tree->root == NULL) {
        return;
    }

    subtree_traverse(tree->root, visit_node, order);
}
