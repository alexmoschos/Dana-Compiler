simple: lexer.o parser.o
	gcc -o $@ $^ -lfl

lexer: lex.yy.c
	gcc $< -lfl -o lexer

lexer.c: danalexer.l
	flex -s -o lexer.c danalexer.l

parser.h parser.c: parser.y
	bison -dv -o parser.c parser.y

lexer.o: lexer.c parser.h
clean : 
	$(RM) lexer.c parser.c parser.h parser.output *.o *~

distclean: clean
	$(RM) simple
