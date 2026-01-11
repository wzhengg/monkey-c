CC = gcc
OPTS = -Wall -Werror -pedantic -std=c23

OBJS = $(SRCS:.c=.o)

test: $(OBJS)
	$(CC) -o test $(OBJS)

%.o: %.c
	$(CC) $(OPTS) -c $< -o $@

clean:
	rm -f $(OBJS) test
