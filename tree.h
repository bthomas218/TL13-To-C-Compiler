#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include "uthash.h"

// Struct for nodes of parse tree
typedef struct Node {
 char* type; // Token type
 char* value; // Actual Text of Tokens
 int child_count; // Number of children
 int line_number; // Self-explanatory
 struct Node** children; // Pointers to children
} Node;

// Struct for Symbol Table entries
typedef struct Symbol {
 char* name;
 char* type;
 int* line_number;
 bool initialized;
} Symbol;

// Struct for Symbol Table
typedef struct SymbolTable {
 char* name; // Cuz the key is a var name
 Symbol entry;
 UT_hash_handle hh; // Makes this struct hashable
} SymbolTable;
extern SymbolTable* symbol_table;

// AST functions
Node* create_node(const char* type, const char* value, int line_number, int child_count, ...);
void print_tree(Node* root, int depth); // for debugging
void generate_code(Node *root);
const char* infer_type(Node* root);

// Symbol Table Functions
void insert_symbol(const char* name, const char* type);
const char* check_symbol(const char* name, int line_number); // Returns identifier type for comparison
void free_symbols();

// For semantic errors and freeing memory used by table
void clean_up();
#endif //TREE_H
