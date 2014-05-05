all: /tmp/quizduell

objects := $(patsubst %.c,%.o,$(wildcard *.c))

Quizduell_View.o: Quizduell_View.c
	clang -Wall -ggdb -lgcrypt `pkg-config --libs --cflags elementary eina ecore` -I/usr/local/include/ Quizduell_View.c -o Quizduell_View.o

/tmp/quizduell: $(objects)
	clang -Wall -ggdb -lgcrypt `pkg-config --libs --cflags elementary eina ecore` -I/usr/local/include/ $(objects) -o /tmp/quizduell
