NAME	   =	s21_cat
INCLUDES =	-I includes
HEADER	 =	s21_cat.h
SRCS	   =	s21_cat.c
OBJS	=	s21_cat.o
FLAGS	   =	-Wall -Wextra -Werror
CC	  	 =	gcc

all:	$(NAME)

%.o:	%.c  $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean:	clean
	rm -rf $(NAME)

re:		fclean all

.PHONY:		all clean fclean re