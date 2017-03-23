simple: lexer.o parser.o
	g++ -o $@ $^ -lfl

lexer: lexer.cpp
	g++ $< -lfl -o lexer

lexer.cpp: danalexer.l
	flex -s -o lexer.cpp danalexer.l

parser.hpp parser.cpp: parser.y
	bison -dv -o parser.cpp parser.y

lexer.o: lexer.cpp parser.hpp
clean : 
	$(RM) lexer.cpp parser.cpp parser.hpp parser.output *.o *~

distclean: clean
	$(RM) simple
