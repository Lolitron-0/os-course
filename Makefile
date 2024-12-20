SRC=$(wildcard *.c)
CFLAGS = -Wall -pedantic -g -O0 -lpthread

%: %.c
	 $(CC) $(CFLAGS) $< -o $@.out

clean:
	rm -f *.out
