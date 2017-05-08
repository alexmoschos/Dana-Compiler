FLAGS = -std=c++11 -O2
simple: lexer.o parser.o ast.o symbol.o general.o error.o
	g++ -o $@ $^ -lfl

lexer: lexer.cpp
	g++ $< -lfl -o lexer

lexer.cpp: danalexer.l ast.h ast.cpp
	flex -s -o lexer.cpp danalexer.l

parser.hpp parser.cpp: parser.y ast.h ast.cpp
	bison -dv -o parser.cpp parser.y

lexer.o: lexer.cpp parser.hpp


parser.o: parser.cpp parser.hpp symbol.c symbol.h
%.o: %.cpp
	g++ $(FLAGS) -c $<

clean :
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o *~

distclean: clean
	$(RM) simple
