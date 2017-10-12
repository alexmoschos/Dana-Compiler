# Dana Compiler 2017

## Authors:
```
Alexander Moschos
Giorgos Petrou
```
Implementation of a compiler for the Dana programming language for the 2017 Compilers Course in NTUA.

Dependencies:
```
1. LLVM 3.8(llc,opt)
2. flex
3. bison
4. clang and clang++
5. sed
6. dd
7. edsger-lib( https://github.com/abenetopoulos/edsger_lib )
```

To build everything do

```shell
make
```

To run the compiler with layout do with a sample file `sample.dna`:
```shell
./do.sh -l sample.dna
```
To run the compiler with begin/end blocks with input in stdin do:
```shell
./do.sh -b -O3
```

The executable supports a number of flags
```shell
-l     Language with Layout(by default enabled).
-b     Language with begin/end blocks(by default disabled).
-f     Program in stdin x86_64 Assembly in stdout.
-i     Program in stdin LLVM IR in stdout.
-O<n>  Optimization flag where n = {0,1,2,3}(Applied to the IR and the Assembly code)
```
The compiled executable will be called `a.out`