SHELL := /bin/bash
CPP = clang++
FLAGS = -std=c++11 -g -O2 -Qunused-arguments -Wno-unknown-warning-option -Wall
#LLVMFLAGS = `llvm-config-3.8 --cxxflags --ldflags --libs  --system-libs`
LLVMFLAGS = `llvm-config-3.8 --cxxflags --ldflags --libs all --system-libs`

all : simple libs

libs :
	cd edsger_lib-master; bash libs.sh

simple: lexer.o parser.o ast.o symbol.o general.o error.o sem.o llvm.o opt.o
	$(CPP) -$(FLAGS) -o $@ $^ -lfl  $(LLVMFLAGS)

block: blocklexer.o blockparser.o ast.o symbol.o general.o error.o sem.o llvm.o opt.o
	$(CPP) -$(FLAGS) -o $@ $^ -lfl  $(LLVMFLAGS)

lexer: lexer.cpp
	$(CPP) $< -lfl -o lexer

lexer.cpp: danalexer.l ast.h ast.cpp
	flex -s -o lexer.cpp danalexer.l

blocklexer.cpp: blocklexer.l ast.h ast.cpp
	flex -s -o blocklexer.cpp blocklexer.l

parser.hpp parser.cpp: parser.y ast.h ast.cpp
	bison -dv -o parser.cpp parser.y

blockparser.hpp blockparser.cpp: blockparser.y ast.h ast.cpp
	bison -dv -o blockparser.cpp blockparser.y

lexer.o: lexer.cpp parser.hpp

blocklexer.o: blocklexer.cpp blockparser.hpp

llvm.o: llvm.cpp llvm.h
	$(CPP) llvm.cpp -c -$(FLAGS)  `llvm-config-3.8 --cxxflags`

opt.o: opt.cpp opt.h
	$(CPP) opt.cpp -c -$(FLAGS)  $(LLVMFLAGS)

parser.o: parser.cpp parser.hpp symbol.c symbol.h
	$(CPP) $(FLAGS) -c $<

blockparser.o: blockparser.cpp blockparser.hpp symbol.c symbol.h
	$(CPP) $(FLAGS) -c $<

%.o: %.cpp
	$(CPP) $(FLAGS) -c $<

clean :
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o *~ *.output blockparser.cpp blockparser.hpp blocklexer.cpp

distclean: clean
	$(RM) simple block