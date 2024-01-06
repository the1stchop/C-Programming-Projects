CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
EXEC = encode decode
OBJS = trie.o word.o io.o encode.o decode.o

all: $(EXEC)

encode: encode.o trie.o word.o io.o
	$(CC) -o encode encode.o trie.o word.o io.o

decode: decode.o trie.o word.o io.o
	$(CC) -o decode decode.o trie.o word.o io.o

encode.o: encode.c trie.c word.c io.c
	$(CC) $(CFLAGS) -c encode.c trie.c word.c io.c

decode.o: decode.c trie.c word.c io.c
	$(CC) $(CFLAGS) -c decode.c trie.c word.c io.c

trie.o: trie.c
	$(CC) $(CFLAGS) -c trie.c

word.o: word.c
	$(CC) $(CFLAGS) -c word.c

io.o: io.c
	$(CC) $(CFLAGS) -c io.c

clean:
	rm -f $(EXEC) $(OBJS)
format:
	clang-format -i -style=file *.[ch]
