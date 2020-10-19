out:=hw2
.PHONY: all clean

all:
	gcc -Wall -lm -pthread -o $(out).bin main.c util.c primes.c

clean:
	rm -rf $(out).bin
