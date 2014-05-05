all: /tmp/quizduell

objects := $(patsubst %.c,%.o,$(wildcard *.c))

/tmp/quizduell: $(objects)
	clang -Wall -ggdb -lgcrypt -I/usr/local/include/ $(objects) -o /tmp/quizduell
