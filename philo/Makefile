CC					=	cc
CFLAGS				=	-Wall -Wextra -Werror
#F_SANITIZE			=	-fsanitize=thread
DEBUG_FLAGS			=	-g3 $(F_SANITIZE)
NAME				=	philo
HEADER_FOLDER		=	headers/
HEADER				=	$(HEADER_FOLDER)philosophers.h
SRC_FOLDER			=	sources/
SRC_FILES			=	monitoring.c \
						mutex.c \
						mutex2.c \
						parsing.c \
						print.c \
						threads.c \
						utils.c \
						wrapper.c
SRC					=	$(addprefix $(SRC_FOLDER), $(SRC_FILES))
OBJ_FOLDER			=	objects/
OBJ					=	$(addprefix $(OBJ_FOLDER), $(SRC_FILES:.c=.o))

all					:	$(NAME)

$(NAME)				: $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(DEBUG_FLAGS) -o $(NAME)

$(OBJ_FOLDER)%.o	: $(SRC_FOLDER)%.c $(HEADER)
	@mkdir -p $(OBJ_FOLDER)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -c $< -o $@

clean				:
	@rm -rf $(OBJ_FOLDER)

fclean				: clean
	@rm -rf $(NAME)

re					: fclean all

.PHONY: all clean fclean re
.DELETE_ON_ERROR: