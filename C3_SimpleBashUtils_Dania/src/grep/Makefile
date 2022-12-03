NAME = s21_grep
HEADER = s21_grep.h
SRCS = s21_grep.c
OBJS	= $(patsubst %.c,%.o,$(SRCS))
D_FILES = $(patsubst %.c,%.d,$(SRCS))
FLAGS =	-Wall -Wextra -D_GNU_SOURCE 
CC = gcc

all : $(NAME)

%.o : %.c  $(HEADER)
	$(CC) $(FLAGS) -c $< -o $@ -MD

include $(wildcard $(D_FILES))

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS) $(D_FILES)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY : all clean fclean re