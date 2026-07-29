NAME	= philo

CC		= cc
FLAGS	= -pthread -Wall -Wextra -Werror

SRCS	= fork.c utils.c philo.c monitor_and_routine.c dead_fun_and_eat.c its_true.c
OBJS	= $(SRCS:.c=.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c philo.h
	$(CC) $(FLAGS) -c $< -o $@


clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all