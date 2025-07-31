CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror
#F_SANITIZE	=	-fsanitize=address
DEBUG_FLAGS =	-g $(F_SANITIZE)
NAME		=	philosophers
HEADER		=	philosophers.h

SRC			=	$(wildcard *.c)
OBJ			=	$(SRC:.c=.o)

all			:	$(NAME)

$(NAME)		: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(DEBUG_FLAGS) -o $(NAME)

%.o			: %.c $(HEADER)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

clean		:
	@rm -f $(OBJ)

fclean		: clean
	@rm -f $(NAME)

re			: fclean all

.PHONY: all clean fclean re submodules
