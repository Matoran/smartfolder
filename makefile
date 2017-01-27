# Variables Definition
version = gcc -std=gnu99
CFLAGS = -g -Wall -Wextra -D_GNU_SOURCE -D_XOPEN_SOURCE
# Main compilation
searchfolder : main.o crawler.o destroyer.o filter.o linker.o logger.o parser.o stack.o wrappersyscall.o
	$(version) $^ -o $@
main.o : main.c 
	$(version) -c main.c $(CFLAGS)

%.o: %.c %.h
	$(version) $(CFLAGS) $< -c
clean:
	rm -rf *.o searchfolder
