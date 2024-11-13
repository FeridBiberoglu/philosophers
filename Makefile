NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
CFLAGS += -g
# CFLAGS += -g -fsanitize=thread

SRCS = main.c \
       init.c \
       routine.c \
       utils.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re 