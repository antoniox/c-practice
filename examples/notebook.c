#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "bst.h"


int compare_keys(void * left, void * right) {
    return strcmp((char *)left, (char *)right);
}


void simple_tree_init(Tree * tree) {
    tree_init(tree, compare_keys, default_destroy_node_hook);
}


void simple_tree_insert(Tree * tree, char * name, char * number) {
    char * old_number = tree_lookup(tree, name);

    if (old_number == NULL) {
        printf("OK\n");
        tree_insert(tree, name, number);
    } else {
        printf("Changed. Old value = %s\n", old_number);
        tree_remove(tree, name);
        tree_insert(tree, name, number);
    }
}


void simple_tree_lookup(Tree * tree, char * name) {
    char * number = tree_lookup(tree, name);

    if (number == NULL) {
        printf("NO\n");
    } else {
        printf("%s\n", number);
    }
}


typedef enum Action {
    INSERT,
    LOOKUP,
    STOP
} Action;


typedef struct Command {
    Action action;
    char * name;
    char * number;
} Command;


void read_command(Command * command) {
    char command_token[8];

    char * name = NULL;
    char * number = NULL;

    scanf("%s", command_token);

    if (strncmp(command_token, "STOP", 4) == 0) {
        command->action = STOP;
    } else {
        name = (char *)malloc(sizeof(char) * 50);

        if (strncmp(command_token, "FIND", 4) == 0) {
            command->action = LOOKUP;
            scanf(" %s", name);
        } else {
            command->action = INSERT;
            number = (char *)malloc(sizeof(char) * 20);
            scanf(" %s %s", name, number);
        }
    }

    command->name = name;
    command->number = number;
}


int main() {
    Tree tree;
    simple_tree_init(&tree);

    Command command;

    while (1) {
        read_command(&command);
        if (command.action == STOP) {
            break;
        }

        switch (command.action) {
        case INSERT:
            simple_tree_insert(&tree, command.name, command.number);
            break;
        case LOOKUP:
            simple_tree_lookup(&tree, command.name);
            free(command.name);
            break;
        }
    }

    tree_destroy(&tree);

    return 0;
}
