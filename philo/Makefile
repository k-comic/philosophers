CC = cc

NAME = philo

SRCS =	main.c	utils.c init.c

OBJS = $(SRCS:%.c=%.o)

FLAGS = -Wall -Wextra -Werror

HEADER = header.h

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS) $(HEADER)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all