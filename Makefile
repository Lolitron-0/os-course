SRC=$(wildcard *.c)
CFLAGS = -Wall -pedantic

%: %.c
	 $(CC) $(CFLAGS) $< -o $@.out

clean:
	rm -f *.out
