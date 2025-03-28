CFLAGS  = -std=c++11 -Wall -Werror -pedantic -fsanitize=address,undefined
SRC     = main.cc lib.cc token.cc ast.cc value.cc lexer.cc \
	  parser.cc inbuf.cc scope.cc
CC      = g++

all: $(SRC)
	$(CC) $(CFLAGS) $^
