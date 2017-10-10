# Dana Compiler 2017

Implementation of a compiler for the Dana programming language for the 2017 Compilers Course in NTUA.

Dependencies:
```
1. LLVM 3.8(llc,opt)
2. flex
3. bison
```

To build everything do

```shell
make
```

To run the compiler with layout do with a sample file `sample.dna`:
```shell
./do.sh -l sample.dna
```
The executable supports a number of flags
```shell
-l     Language with Layout
-b     Language with begin/end blocks(by default enabled).
-f     Program in stdin x86_64 Assembly in stdout
-i     Program in stdin LLVM IR in stdout
-O<n>  Optimization flag where n = {1,2,3}(Applied to the IR and the Assembly code)
```