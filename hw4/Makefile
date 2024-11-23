SRC=$(wildcard *.c)
CFLAGS = -Wall -pedantic -g -O0 -lpthread

all: hw4boat hw4person

%: %.c
	 $(CC) $(CFLAGS) $< -o $@.out

clean:
	rm -f *.out
