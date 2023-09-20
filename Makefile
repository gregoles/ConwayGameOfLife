CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
OBJS = life.c universe.o

all: life

life: $(OBJS)
		$(CC) -o life *.c -lm -lcurses

sorting.o: life.c
		$(CC) $(CFLAGS) -c *.c

clean:
		rm -f life *.o

format:
		clang-format -i -style=file *.[ch]





