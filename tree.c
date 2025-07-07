/*
* @author: Brady Thomas
 * @class: CS 4386
 * @section: .501
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tree.h"

Node* create_node(const char* type, const char* value, const int child_count, ...) {
    Node* node = malloc(sizeof(Node));
    node->type = strdup(type);
    node->value = value ? strdup(value) : NULL;
    node->child_count = child_count;
    node->children = malloc(sizeof(Node*) * child_count);

    va_list args;
    va_start(args, child_count);
    for (int i = 0; i < child_count; i++) {
        node->children[i] = va_arg(args, Node*);
    }
    va_end(args);

    return node;
}

// for debugging
void print_tree(Node* root, int depth) {
    if (!root) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s", root->type);
    if (root->value) printf(" (%s)", root->value);
    printf("\n");

    for (int i = 0; i < root->child_count; i++) {
        print_tree(root->children[i], depth + 1);
    }
}

