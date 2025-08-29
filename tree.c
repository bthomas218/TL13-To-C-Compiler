#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "tree.h"
#include "uthash.h"

// For presentation
int indent_level = 0;
void indent() {
    for (int i = 0; i < indent_level; i++) {
        printf("\t");
    }
}

// For error handling, Semantic error count
int error_count = 0;

//Symbol Table Functions
SymbolTable* symbol_table = NULL;
void insert_symbol(const char* name, const char* type) {
    SymbolTable* s;
    HASH_FIND_STR(symbol_table, name, s);
    if (s == NULL) {
        s = malloc(sizeof(SymbolTable));
        s->name = strdup(name);
        s->entry.name = strdup(name);
        s->entry.type = strdup(type);
        s->entry.initialized = true;
        HASH_ADD_KEYPTR(hh, symbol_table, s->name, strlen(s->name), s);
    } else {
        s->entry.initialized = true;
    }
}

const char* check_symbol(const char* name, int line_number) {
    SymbolTable* s;
    HASH_FIND_STR(symbol_table, name, s);
    if (s == NULL) { // Undeclared Variable error
        fprintf(stderr, "ERROR[line %d]: Undeclared Variable '%s'\n", line_number, name);
        error_count++;
        return "undefined"; // dummy value
    }
    return s->entry.type; // Returns identifier type for comparison
}

void free_symbols() {
    SymbolTable *s, *tmp;
    HASH_ITER(hh, symbol_table, s, tmp) {
        HASH_DEL(symbol_table, s);
        free(s->name);
        free(s->entry.name);
        free(s->entry.type);
        free(s);
    }
}

// AST Functions
Node* create_node(const char* type, const char* value, int line_number, int child_count, ...) {
    Node* node = malloc(sizeof(Node));
    node->type = strdup(type);
    node->value = value ? strdup(value) : NULL;
    node->child_count = child_count;
    node->line_number = line_number;
    node->children = malloc(sizeof(Node*) * child_count);

    va_list args;
    va_start(args, child_count);
    for (int i = 0; i < child_count; i++) {
        node->children[i] = va_arg(args, Node*);
    }
    va_end(args);

    return node;
}

// For debugging
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

void generate_code(Node* root) {
  if(!root) return;

  if (strcmp(root->type, "program") == 0) {
  //PROGRAM
  printf("#include <stdio.h>\n");
  printf("#include <stdlib.h>\n");
  printf("#include <stdbool.h>\n\n");

  printf("int readInt() {\n\tint x;\n\tscanf(\"%%d\", &x);\n\treturn x;\n}\n\n"); // had to define readint somewhere

  generate_code(root->children[1]); // Generate declarations
  printf("\nint main() {\n"); // BEGIN
      indent_level++;
  generate_code(root->children[3]); // Generate statement sequence
      indent_level--;
  printf("\treturn 0;\n}\n"); // END
  }

  else if (strcmp(root->type, "declarations") == 0) {
    if(root->child_count == 0) { // Do nothing
    } else {
        // translates to C code: VAR <name> AS <type>; to <type> <name>;
        const char *var_name = root->children[1]->value;
        const char *var_type = root->children[3]->value;
        insert_symbol(var_name, var_type); // declare variable here
        printf("%s %s;\n", var_type, var_name);

        // Generate additional declarations
        generate_code(root->children[5]);
    }
  }

  else if (strcmp(root->type, "statementSequence") == 0) {
       if(root->child_count == 0) { // do nothing
       } else {
           indent();
           generate_code(root->children[0]); // Generate statement
           generate_code(root->children[2]); // Generate more statement sequences
       }
   }

  else if (strcmp(root->type, "assignment") == 0) {
      const char *var_name = root->children[0]->value;
      const char *var_type = check_symbol(var_name, root->children[0]->line_number); // check if variable was declared + get type
      const char *rhs_type = infer_type(root->children[2]);
      const int line_number = root->line_number;

      if (strcmp(var_type, rhs_type) != 0) { // Type mismatch error
          fprintf(stderr, "ERROR[line %d]: Type mismatch in assignment to '%s'. Expected '%s' but got '%s'\n", line_number, var_name, var_type, rhs_type);
          error_count++;
      }

      printf("%s = ", var_name); // IDENT (variable name) + AS (=)
      generate_code(root->children[2]); // Generate the expression
      printf(";\n"); // Only assignment should end in semicolon in c
  } else if (strcmp(root->type, "READINT") == 0) {
      printf("readInt()");
  }

  else if (strcmp(root->type, "if") == 0) {
      const char *expr_type = infer_type(root->children[1]);
      const int line_number = root->children[1]->line_number;
      if (strcmp(expr_type, "bool") != 0) { // Expression guard must be boolean
          fprintf(stderr, "ERROR[line %d]: Expression guarding If Statement must be boolean, got '%s'\n", line_number, expr_type);
          error_count++;
      }

      printf("\n");
      indent();
      printf("if ("); // IF
      generate_code(root->children[1]); // Generate expression
      printf(") {\n"); // THEN

      indent_level++;
      generate_code(root->children[3]); // Generate statement sequence
      indent_level--;
      indent();
      printf("}");

      // check if elseClause exists
      if (root->children[4] != NULL && root->children[4]->child_count > 0) {
          printf(" else {\n");
          indent_level++;
          generate_code(root->children[4]); // Generate elseClause
          indent_level--;
          indent();
          printf("}");

      }
      printf("\n"); // END
  }

  else if (strcmp(root->type, "else") == 0) {
      if(root->child_count == 0) { // Do nothing
      } else {
          generate_code(root->children[1]); // Generate statementSequence
      }
  }

  else if (strcmp(root->type, "while") == 0) {
      const char *expr_type = infer_type(root->children[1]);
      const int line_number = root->children[1]->line_number;
      if (strcmp(expr_type, "bool") != 0) { // Expression guard must be boolean
          fprintf(stderr, "ERROR[line %d]: Expression guarding While Statement must be boolean, got '%s'\n", line_number, expr_type);
          error_count++;
      }
      printf("\n");
      indent();
      printf("while ("); // WHILE
      generate_code(root->children[1]); // Generate expression
      printf(") {\n"); // DO
      indent_level++;
      generate_code(root->children[3]); // Generate statementSequence
      indent_level--;
      indent();
      printf("}\n\n"); //END
  }

  else if (strcmp(root->type, "writeInt") == 0) {
      const char *expr_type = infer_type(root->children[1]);
      const int line_number = root->children[1]->line_number;
      if (strcmp(expr_type, "int") != 0) { // Write int expression must evaluate to int
          fprintf(stderr, "ERROR[line %d]: writeInt expression must evaluate to integer got '%s'\n", line_number, expr_type);
          error_count++;
      }
      printf("printf(\"%%d\\n\", "); // Write int in TL13 is printing to console + new line
      generate_code(root->children[1]);
      printf(");\n");
  }

  else if (strcmp(root->type, "expression") == 0) {
      generate_code(root->children[0]); // lhs simple expr

      //OP4 comparator
      const char* op = root->children[1]->value;
      if (strcmp(op, "=") == 0) {
          printf(" == "); // Translate TL13 = (comparator) to C ==
      } else {
          printf(" %s ", op);
      }

      generate_code(root->children[2]); // rhs simple expr
  } else if (strcmp(root->type, "simpleExpr") == 0) {
      generate_code(root->children[0]); // lhs term
      printf(" %s ", root->children[1]->value); // OP3 +/- operators
      generate_code(root->children[2]); // rhs term
  } else if (strcmp(root->type, "term") == 0) {
      generate_code(root->children[0]); // lhs factor

      const char* op = root->children[1]->value;
      if (strcmp(op, "div") == 0) {
          printf(" / "); // Translate TL13 div to C division operator /
      } else if (strcmp(op, "mod") == 0) {
          printf(" %% "); // Translate TL13 mod to C modulo operator %
      } else {
          printf(" %s ", op); // *
      }

      generate_code(root->children[2]); // rhs factor
  } else if (strcmp(root->type, "factor") == 0) {
      printf("("); //LP
      generate_code(root->children[1]); // Generate expression
      printf(")"); //RP
  }
  else if (strcmp(root->type, "IDENT") == 0) {
      const char *var_name = root->value;
      const int line_number = root->line_number;
      check_symbol(var_name, line_number); // check for declared variable
      printf("%s", root->value);
  } else if (strcmp(root->type, "NUM") == 0) {
      const long val = strtol(root->value, NULL, 10);
      const int line_number = root->line_number;
      if (val < 0 || val > 2147483647) { //
          fprintf(stderr, "ERROR[line %d]: Integer literal '%ld' out of range (must be 0..2147483647)\n", line_number, val);
          error_count++;
      }
      printf("%s", root->value);
  } else if (strcmp(root->type, "BOOLLIT") == 0) {
      printf("%s", root->value);
  }
}

const char* infer_type(Node* root) {
    if (strcmp(root->type, "NUM") == 0) return "int";
    if (strcmp(root->type, "BOOLLIT") == 0) return "bool";
    if (strcmp(root->type, "IDENT") == 0) return check_symbol(root->value, root->line_number);

    if (strcmp(root->type, "expression") == 0) {
        // Comparators OP4 should return bool
        const char* lhs = infer_type(root->children[0]);
        const char* rhs = infer_type(root->children[2]);
        const int line_number = root->children[1]->line_number; // I think using OP's line number is most precise?
        if (strcmp(lhs, rhs) != 0) {
            fprintf(stderr, "ERROR[line %d]: Type mismatch in comparison: '%s' vs '%s'\n", line_number, lhs, rhs);
            error_count++;
        }
        return "bool";
    }

    if (strcmp(root->type, "simpleExpr") == 0 || strcmp(root->type, "term") == 0) {
        const char* lhs = infer_type(root->children[0]);
        const char* rhs = infer_type(root->children[2]);
        const int line_number = root->children[1]->line_number;
        if (strcmp(lhs, rhs) != 0) {
            fprintf(stderr, "ERROR[line %d]: Type mismatch in arithmetic expression: '%s' vs '%s'\n", line_number, lhs, rhs);
            error_count++;
        }
        return "int";  // OP2 OP3 Arimethic Operators return int
    }

    if (strcmp(root->type, "factor") == 0 && root->child_count == 3) {
        // ( expression )
        return infer_type(root->children[1]);
    }

    return "int"; // Default case
}


// crashes program also frees memory used by symbol table
void clean_up() {
    free_symbols();
    if (error_count > 0) {
        fprintf(stderr, "\nCompilation failed with %d semantic error(s).\n", error_count);
        exit(1);
    }
}
