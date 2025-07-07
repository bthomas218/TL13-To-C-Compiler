    /*
    * @author: Brady Thomas
    * @class: CS 4386
    * @section: .501
    */

%{
#include <stdio.h>
#include "parser.tab.h"
#include "tree.h"
#define YYSTYPE Node*
%}

%%

    /* Built-In Procedures */
"writeInt"    { yylval.node = create_node("WRITEINT", yytext, 0); return WRITEINT; }
"readInt"     { yylval.node = create_node("READINT", yytext, 0); return READINT; }

    /* Keywords */
"if"          { yylval.node = create_node("IF", yytext, 0); return IF; }
"then"        { yylval.node = create_node("THEN", yytext, 0); return THEN; }
"else"        { yylval.node = create_node("ELSE", yytext, 0); return ELSE; }
"begin"       { yylval.node = create_node("KEYWORD_BEGIN", yytext, 0); return KEYWORD_BEGIN; }
"end"         { yylval.node = create_node("END", yytext, 0); return END; }
"while"       { yylval.node = create_node("WHILE", yytext, 0); return WHILE; }
"do"          { yylval.node = create_node("DO", yytext, 0); return DO; }
"program"     { yylval.node = create_node("PROGRAM", yytext, 0); return PROGRAM; }
"var"         { yylval.node = create_node("VAR", yytext, 0); return VAR; }
"as"          { yylval.node = create_node("AS", yytext, 0); return AS; }
"int"         { yylval.node = create_node("INT", yytext, 0); return INT; }
"bool"        { yylval.node = create_node("BOOL", yytext, 0); return BOOL; }

    /* Symbols and Operators */
"("           { yylval.node = create_node("LP", yytext, 0); return LP; }
")"           { yylval.node = create_node("RP", yytext, 0); return RP; }
":="          { yylval.node = create_node("ASGN", yytext, 0); return ASGN; }
";"           { yylval.node = create_node("SC", yytext, 0); return SC; }
"*"|"div"|"mod" { yylval.node = create_node("OP2", yytext, 0); return OP2; }
"+"|"-"        { yylval.node = create_node("OP3", yytext, 0); return OP3; }
"="|"!="|"<"|">"|"<="|">=" { yylval.node = create_node("OP4", yytext, 0); return OP4; }

    /* Numbers Literals and Identifiers */
[0-9]+        { yylval.node = create_node("NUM", yytext, 0); return NUM; }
false|true    { yylval.node = create_node("BOOLLIT", yytext, 0); return BOOLLIT; }
[A-Z][A-Z0-9]* { yylval.node = create_node("IDENT", yytext, 0); return IDENT; }
    /* Whitespace */
[ \t\n\r]+      { }
%%


