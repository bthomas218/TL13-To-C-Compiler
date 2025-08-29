# TL13 -> C Compiler
A compiler built from scratch for the TL13 toy language.  
Implements **lexical analysis, parsing, semantic checks, and C code generation** using Flex and Bison.  

This project translates TL13 source code into valid C, handling:
- Expressions & arithmetic
- Control flow (`if`, `while`)
- Symbol tables & type checking
- Built-in procedures (`readInt`, `writeInt`)

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

