CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
EXEC = sorting
OBJS = sorting.o shell.o heap.o quick.o batcher.o stats.o set.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $(EXEC) $(OBJS)

sorting.o: sorting.c shell.c quick.c batcher.c stats.c set.c
	$(CC) $(CFLAGS) -c sorting.c shell.c heap.c quick.c batcher.c stats.c set.c

shell.o: shell.c stats.c set.c
	$(CC) $(CFLAGS) -c shell.c stats.c set.c

heap.o: heap.c stats.c
	$(CC) $(CFLAGS) -c heap.c stats.c

quick.o: quick.c stats.c
	$(CC) $(CFLAGS) -c quick.c stats.c

batcher.o: batcher.c stats.c
	$(CC) $(CFLAGS) -c batcher.c stats.c

stats.o: stats.c
	$(CC) $(CFLAGS) -c stats.c

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[ch]
