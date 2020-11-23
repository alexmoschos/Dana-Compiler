SHELL := /bin/bash
CPP = clang++
C = clang
CFLAGS = -std=c11 -Wall
CXXFLAGS = -std=c++11 -Qunused-arguments -Wno-unknown-warning-option -Wall
LLVMFLAGS = `llvm-config-10 --cxxflags --ldflags --libs all --system-libs`

all : simple libs

libs :
	cd edsger_lib; bash libs.sh

simple: lexer.o parser.o ast.o symbol.o general.o error.o sem.o llvm.o opt.o
	$(CPP) -$(CXXFLAGS) -o $@ $^ $(LLVMFLAGS)

lexer: lexer.cpp
	$(CPP) $< -lfl -o lexer

lexer.cpp: danalexer.l ast.h ast.cpp
	flex -s -o lexer.cpp danalexer.l

parser.hpp parser.cpp: parser.y ast.h ast.cpp
	bison -dv -o parser.cpp parser.y

lexer.o: lexer.cpp parser.hpp

llvm.o: llvm.cpp llvm.h
	$(CPP) llvm.cpp -c -$(CXXFLAGS) `llvm-config-10 --cxxflags`

opt.o: opt.cpp opt.h
	$(CPP) opt.cpp -c -$(CXXFLAGS) $(LLVMFLAGS)

parser.o: parser.cpp parser.hpp symbol.c symbol.h
	$(CPP) $(CXXFLAGS) -c $<

%.o: %.cpp
	$(CPP) $(CXXFLAGS) -c $<

%.o: %.c
	$(C) $(CFLAGS) -c $<

clean :
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o *~ *.output

distclean: clean
	$(RM) simple