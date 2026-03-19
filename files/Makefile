NAME    = ft_ping

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror
LDFLAGS = -lm

SRCS    = main.c \
          ping.c \
          utils.c

OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c ft_ping.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
