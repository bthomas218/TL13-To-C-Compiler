#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo -e "${GREEN}Running flex...${NC}"
flex lexer.l || { echo -e "${RED}Flex failed.${NC}"; exit 1; }

echo -e "${GREEN}Running bison...${NC}"
bison -d parser.y || { echo -e "${RED}Bison failed.${NC}"; exit 1; }

echo -e "${GREEN}Compiling with gcc...${NC}"
gcc -o mycompiler parser.tab.c lex.yy.c tree.c -lfl || { echo -e "${RED}GCC compilation failed.${NC}"; exit 1; }

echo -e "${GREEN}Running myCompiler...${NC}"
TEMP_OUT=$(mktemp)
ERROR_LOG="compile_errors.log"

# Redirect stdout to temp, stderr to error log
if ./mycompiler < input.txt > "$TEMP_OUT" 2> "$ERROR_LOG"; then
    mv "$TEMP_OUT" out.c
    echo -e "${GREEN}Compilation successful. Output written to out.c${NC}"
    rm -f "$ERROR_LOG" # clean up error log if there were no issues
else
    echo -e "${RED}Compiler run failed. Check $ERROR_LOG for details.${NC}"
    rm "$TEMP_OUT"
    exit 1
fi

