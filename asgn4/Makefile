CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LDFLAGS = -lncurses
EXEC = life
OBJS = life.o universe.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXEC) $(OBJS)

life.o: life.c universe.c
	$(CC) $(CFLAGS) -c life.c universe.c

universe.o: universe.c
	$(CC) $(CFLAGS) -c universe.c

clean:
	rm -f $(EXEC) $(OBJS)

format:
	clang-format -i -style=file *.[ch]

