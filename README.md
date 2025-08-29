# TL13 -> C Compiler
A compiler built from scratch for the TL13 toy language.  
Implements **lexical analysis, parsing, semantic checks, and C code generation** using Flex and Bison.  

This project translates TL13 source code into valid C, handling:
- Expressions & arithmetic
- Control flow (`if`, `while`)
- Symbol tables & type checking
- Built-in procedures (`readInt`, `writeInt`)

See [language specifications](https://github.com/bthomas218/TL13-To-C-Compiler/blob/main/language-specs.md) for full TL13 grammar, lexical rules, and semantics.

## Example
### *Input in TL13*
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
### *Output in C*
```C
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int readInt() {
	int x;
	scanf("%d", &x);
	return x;
}

int X;

int main() {
	X = readInt();
	
	if (X > 0) {
		printf("%d\n", X);
	} else {
		printf("%d\n", 0);
	}
	return 0;
}
```

## Compile Time Error Checking
### *Input in TL13*
```tl13
program
   var SMALLER as int ;
   var BIGGER as int ;
   var TEMP as int ;
begin
   BIGGER := readInt ;
   SMALLER := readInt ;

   if 13 then
      TEMP := SMALLER ;
      TEMP1 := 2147483648 ;
      SMALLER := BIGGER ;
      BIGGER := TEMP ;
   end ;

   while 12 do
      BIGGER := BIGGER - SMALLER ;

      if SMALLER > BIGGER then
         TEMP := SMALLER ;
         SMALLER := BIGGER ;
         BIGGER := TEMP ;
      end ;
   end ;
   writeInt BIGGER ;
end
```

### *Output in Error Log*
```pgsql
ERROR[line 9]: Expression guarding If Statement must be boolean, got 'int'
ERROR[line 11]: Undeclared Variable 'TEMP1'
ERROR[line 11]: Type mismatch in assignment to 'TEMP1'. Expected 'undefined' but got 'int'
ERROR[line 11]: Integer literal '2147483648' out of range (must be 0..2147483647)
ERROR[line 16]: Expression guarding While Statement must be boolean, got 'int'

Compilation failed with 5 semantic error(s).
```

## Requirements
- GCC
- Flex
- Bison
- [uthash](https://troydhanson.github.io/uthash/)
## Build and Run
```bash
git clone https://github.com/bthomas218/TL13-To-C-Compiler
cd tl13-compiler
chmod +x run.sh
./run.sh
```
Write your own programs in TL13 in the input.txt file

## What I learned
- Implemented a full compiler pipeline (lexer → parser → semantic analysis → codegen).  
- Hands-on with **Flex**, **Bison**, and AST construction.  
- Deepened understanding of type systems, symbol tables, and control-flow lowering.  
