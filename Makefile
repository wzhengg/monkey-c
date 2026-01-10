CC = gcc
OPTS = -Wall -Werror -pedantic -std=c23

test: test.o token.o lexer.o
	gcc -o test test.o token.o lexer.o

test.o: test.c
	$(CC) $(OPTS) -c test.c

token.o: token.c
	$(CC) $(OPTS) -c token.c

lexer.o: lexer.c
	$(CC) $(OPTS) -c lexer.c

clean:
	rm -f token.o lexer.o monkey
