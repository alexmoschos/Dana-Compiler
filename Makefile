SHELL := /bin/bash
CPP = clang++
FLAGS = -std=c++11 -O2 -g
#LLVMFLAGS = `llvm-config-3.8 --cxxflags --ldflags --libs  --system-libs`
LLVMFLAGS = 
simple: lexer.o parser.o ast.o symbol.o general.o error.o sem.o
	$(CPP) -$(FLAGS) -o $@ $^ -lfl  $(LLVMFLAGS)

lexer: lexer.cpp
	$(CPP) $< -lfl -o lexer

lexer.cpp: danalexer.l ast.h ast.cpp
	flex -s -o lexer.cpp danalexer.l

parser.hpp parser.cpp: parser.y ast.h ast.cpp
	bison -dv -o parser.cpp parser.y

lexer.o: lexer.cpp parser.hpp


parser.o: parser.cpp parser.hpp symbol.c symbol.h
	$(CPP) $(FLAGS) $(LLVMFLAGS)  -c $<
%.o: %.cpp
	$(CPP) $(FLAGS) $(LLVMFLAGS)  -c $<

clean :
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o *~

distclean: clean
	$(RM) simple
