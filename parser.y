%code requires {
    #include "tree.h"
}

%{
    #include <stdio.h>
    #include "tree.h"
    int yylex(void);
    int yyerror(char *);
    Node* root;
%}

%locations
 /* %debug */

%union {
    Node* node;
}

    /* Tokens */
%token <node> PROGRAM
%token <node> KEYWORD_BEGIN
%token <node> END
%token <node> VAR
%token <node> AS
%token <node> SC
%token <node> INT
%token <node> BOOL
%token <node> IF
%token <node> THEN
%token <node> ELSE
%token <node> WHILE
%token <node> DO
%token <node> WRITEINT
%token <node> READINT
%token <node> ASGN
%token <node> IDENT
%token <node> NUM
%token <node> BOOLLIT
%token <node> OP2
%token <node> OP3
%token <node> OP4
%token <node> LP
%token <node> RP

    /* Precedence */
%left OP4  /* Relational Operators */
%left OP3  /* Addition/Subtraction */
%left OP2  /* Multiplication/Division */
%right ASGN

%type <node> program declarations type statementSequence statement assignment
%type <node> ifStatement elseClause whileStatement writeInt expression simpleExpression
%type <node> term factor

%%
program:
    PROGRAM declarations KEYWORD_BEGIN statementSequence END {
        $$ = create_node("program", NULL, $1->line_number, 5, $1, $2, $3, $4, $5);
        root = $$;
    }
    ;

declarations:
    /* empty */ { $$ = create_node("declarations", NULL, @$.first_line, 0); }
    | VAR IDENT AS type SC declarations {
        $$ = create_node("declarations", NULL, $1->line_number, 6, $1, $2, $3, $4, $5, $6);
    }
    ;

type:
    INT { $$ = $1; }
    | BOOL { $$ = $1; }
    ;

statementSequence:
    /* empty */ { $$ = create_node("statementSequence", NULL, @$.first_line, 0); }
    | statement SC statementSequence {
        $$ = create_node("statementSequence", NULL, $1->line_number, 3, $1, $2, $3);
    }
    ;

statement:
    assignment { $$ = $1; }
    | ifStatement { $$ = $1; }
    | whileStatement { $$ = $1; }
    | writeInt { $$ = $1; }
    ;

assignment:
    IDENT ASGN expression {
        $$ = create_node("assignment", NULL, $1->line_number, 3, $1, $2, $3);
    }
    | IDENT ASGN READINT {
        $$ = create_node("assignment", NULL, $1->line_number, 3, $1, $2, $3);
    }
    ;

ifStatement:
    IF expression THEN statementSequence elseClause END {
        $$ = create_node("if", NULL, $1->line_number, 6, $1, $2, $3, $4, $5, $6);
    }
    ;

elseClause:
    /* empty */ { $$ = create_node("else", NULL, @$.first_line, 0); }
    | ELSE statementSequence {
        $$ = create_node("else", NULL, $1->line_number, 2, $1, $2);
    }
    ;

whileStatement:
    WHILE expression DO statementSequence END {
        $$ = create_node("while", NULL, $1->line_number, 5, $1, $2, $3, $4, $5);
    }
    ;

writeInt:
    WRITEINT expression {
        $$ = create_node("writeInt", NULL, $1->line_number, 2, $1, $2);
    }
    ;

expression:
    simpleExpression { $$ = $1; }
    | simpleExpression OP4 simpleExpression {
        $$ = create_node("expression", NULL, $2->line_number, 3, $1, $2, $3);
    }
    ;

simpleExpression:
    term OP3 term {
        $$ = create_node("simpleExpr", NULL, $2->line_number, 3, $1, $2, $3);
    }
    | term { $$ = $1; }
    ;

term:
    factor OP2 factor {
        $$ = create_node("term", NULL, $2->line_number, 3, $1, $2, $3);
    }
    | factor { $$ = $1; }
    ;

factor:
    IDENT { $$ = $1; }
    | NUM { $$ = $1; }
    | BOOLLIT { $$ = $1; }
    | LP expression RP {
        $$ = create_node("factor", NULL, $1->line_number, 3, $1, $2, $3);
    }
    ;
%%

int errors = 0;

int yyerror(char *s) {
     fprintf(stderr, "Error: %s\n", s);
     return 0;
}


int main(void) {
      /* yydebug = 1; */
      yyparse();
      /* printf("PARSING SUCCESS\n"); */
      /* print_tree(root, 0);  */
      generate_code(root);
      clean_up();
      return 0;
}

int yywrap() {
    return 1;
}

