CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic $(shell pkg-config --cflags gmp)
LDFLAGS = -lm $(shell pkg-config --libs gmp)
EXEC = keygen encrypt decrypt
OBJS = randstate.o numtheory.o ss.o keygen.o encrypt.o decrypt.o

all: $(EXEC)

keygen: keygen.o randstate.o ss.o numtheory.o
	$(CC) $(LDFLAGS) -o keygen keygen.o randstate.o ss.o numtheory.o

encrypt: encrypt.o randstate.o ss.o numtheory.o
	$(CC) $(LDFLAGS) -o encrypt encrypt.o randstate.o ss.o numtheory.o

decrypt: decrypt.o randstate.o ss.o numtheory.o
	$(CC) $(LDFLAGS) -o decrypt decrypt.o randstate.o ss.o numtheory.o

keygen.o: keygen.c ss.c randstate.c numtheory.c
	$(CC) $(CFLAGS) -c keygen.c ss.c randstate.c numtheory.c

encrypt.o: encrypt.c ss.c randstate.c numtheory.c
	$(CC) $(CFLAGS) -c encrypt.c ss.c randstate.c ss.c numtheory.c

decrypt.o: decrypt.c ss.c randstate.c numtheory.c
	$(CC) $(CFLAGS) -c decrypt.c ss.c randstate.c numtheory.c

randstate.o: randstate.c
	$(CC) $(CFLAGS) -c randstate.c

numtheory.o: numtheory.c randstate.c
	$(CC) $(CFLAGS) -c numtheory.c randstate.c

ss.o: ss.c numtheory.c randstate.c
	$(CC) $(CFLAGS) -c ss.c numtheory.c randstate.c

clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[ch]
