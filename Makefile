lexel: lex.yy.c
	gcc $< -lfl -o lexer

lex.yy.c: danalexer.l
	flex $<

clean : 
	$(RM) *.o lex.yy.c

distclean: clean
	$(RM) lexer
