SRC=$(wildcard *.c)
CFLAGS = -Wall -pedantic -g -O0 -lpthread

all: hw3boat hw3person

%: %.c
	 $(CC) $(CFLAGS) $< -o $@.out

clean:
	rm -f *.out
