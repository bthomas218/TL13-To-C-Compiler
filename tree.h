/*
* @author: Brady Thomas
 * @class: CS 4386
 * @section: .501
 */

#ifndef TREE_H
#define TREE_H
// Struct for nodes of parse tree
typedef struct Node {
 char* type;            // Variable name or token type
 char* value;           // Used for tokens
 int child_count;       // Number of children
 struct Node** children; // Pointers to children
} Node;

Node* create_node(const char* type, const char* value, const int child_count, ...);
void print_tree(Node* root, int depth); // for debugging
#endif //TREE_H
