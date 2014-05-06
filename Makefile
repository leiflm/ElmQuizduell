all: /tmp/quizduell

CC=clang
CFLAGS=`pkg-config --cflags elementary eina ecore libbsd`
LDFLAGS=`pkg-config --libs elementary eina ecore libbsd`

CFLAGS+= -ggdb -Wall

objects := $(patsubst %.c,%.o,$(wildcard *.c))

/tmp/quizduell: $(objects)
	$(CC) -lgcrypt `pkg-config --libs --cflags elementary eina ecore` $(CFLAGS) $(LDFLAGS) $(objects) -o /tmp/quizduell
