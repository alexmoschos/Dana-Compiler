clang++ llvm.cpp -c `llvm-config --cxxflags` 


clang++ llvm.o ast.o symbol.o general.o error.o `llvm-config --cxxflags --ldflags --libs core --system-libs` -o llvm

