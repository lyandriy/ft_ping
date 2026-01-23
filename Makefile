SRC = main.c \
	  utils.c \

OBJ = $(SRC:.c=.o)

NAME = ft_ping

CC = gcc
CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(NAME)

re: fclean all

.PHONY: all, claen, fclean, re

