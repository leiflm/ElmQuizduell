
CC=clang
CFLAGS=`pkg-config --cflags elementary eina ecore libbsd json-c`
LDFLAGS=`pkg-config --libs elementary eina ecore libbsd json-c`

CFLAGS+= -ggdb -Wall -I.

objects := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.c,%.o,$(wildcard view/*.c))
header := *.h view/*.h

all: /tmp/quizduell

%.o: %.c $(header)
	$(CC) $< -c $(CFLAGS) -o $@

/tmp/quizduell: $(objects) qkgermany.pem
	$(CC) -lgcrypt $(LDFLAGS) $(objects) -o /tmp/quizduell
	cp qkgermany.pem /tmp/

clean:
	rm -rf /tmp/quizduell
	rm -rf *.o
	rm -rf view/*.o
