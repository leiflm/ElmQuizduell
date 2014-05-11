all: /tmp/quizduell

CC=clang
CFLAGS=`pkg-config --cflags elementary eina ecore libbsd json-c`
LDFLAGS=`pkg-config --libs elementary eina ecore libbsd json-c`

CFLAGS+= -ggdb -Wall -I.

objects := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.c,%.o,$(wildcard view/*.c))

/tmp/quizduell: $(objects)
	$(CC) -lgcrypt `pkg-config --libs --cflags elementary eina ecore` $(CFLAGS) $(LDFLAGS) $(objects) -o /tmp/quizduell

clean:
	rm -rf /tmp/quizduell
	rm -rf *.o
