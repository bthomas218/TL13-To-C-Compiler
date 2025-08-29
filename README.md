# TL13 To C Compiler
*Compiler Design Project*

A complete compiler for a TL13 from scratch using Flex and Bison to implement lexical analysis, recursive descent parsing, and abstract syntax tree generation.
Can translate TL13 into presentable equivalent C code with support for expressions, control flow, function calls for TL13's built-in procedures, symbol table handling, and semantic analysis to catch compile-time errors.

## Example
```tl13
program
    var X as int ;
    begin
        X := readInt ;
        if X > 0 then
            writeInt X ;
        else
            writeInt 0;
        end ;
    end ;
end
```
```C
```


# Build Instructions
To build the project copy the files to a directory in your terminal, and use the included run.sh program to compile TL13 code in a file titled "*input.txt*". Also requires uthash from https://troydhanson.github.io/uthash/ 
