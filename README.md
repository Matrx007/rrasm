# rrasm


Assembler designed to produce machine code for my custom CPU ([Matrx007/rrcpu](https://github.com/Matrx007/rrcpu)).

The syntax is very simple, because

# Compiling

I have not written a makefile, so right now just use
```
g++ -Wall -std=c++2a src/assembler.cpp -o assembler
```
or
```
clang++ -Wall -std=c++20 src/assembler.cpp -o assembler
```

To test AST parser, run
```
./assembler tests/test.rrasm
```

# Roadmap


1. Map instrutions & registers ✅ Done  
2. Instruction parsing 🛠 W.I.P  
3. Actions & other functions ❌ TO-DO  