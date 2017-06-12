clang++ llvm.cpp -c `llvm-config --cxxflags --libs core ` 


clang++ llvm.o ast.o  `llvm-config --cxxflags --ldflags --libs core --system-libs` -o llvm

