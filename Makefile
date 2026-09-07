NAME = codexion

SRCS = helper.c init.c main.c parsing.c threads.c coders_cycle.c \
	dongle.c dongle_utils.c heap.c monitor.c cleanup.c single_coder.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
