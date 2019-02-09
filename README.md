# Dana Compiler
[![Build Status](https://travis-ci.com/alexmoschos/Dana-Compiler.svg?branch=master)](https://travis-ci.com/alexmoschos/Dana-Compiler)

Dana is a programming language supporting static type safety, low level programming and nested functions. It was created for the compilers class in NTUA. The lexer and parser are built using flex and bison and the code generation is done using LLVM.

## Dependencies:
```
1. LLVM 3.8(llc,opt)
2. flex
3. bison
4. clang and clang++
5. edsger-lib( https://github.com/abenetopoulos/edsger_lib , git submodule)
```

## Building:

To build everything

```shell
make
```

## Execution
To run the compiler on a sample file `sample.dna`:
```shell
./danac sample.dna
```

The executable supports a number of flags
```shell
-f     Program in stdin x86_64 Assembly in stdout.
-i     Program in stdin LLVM IR in stdout.
-O<n>  Optimization flag where n = {0,1,2,3}(Applied to the IR and the Assembly code)
```
The compiled executable will be called `a.out`