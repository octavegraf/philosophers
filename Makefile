CC					=	cc
CFLAGS				=	-Wall -Wextra -Werror
#F_SANITIZE			=	-fsanitize=address
DEBUG_FLAGS			=	-g3 $(F_SANITIZE)
NAME				=	philo
HEADER_FOLDER		=	headers/
HEADER				=	$(HEADER_FOLDER)philosophers.h
SRC_FOLDER			=	sources/
SRC					=	$(wildcard $(SRC_FOLDER)*.c)
OBJ_FOLDER			=	objects/
OBJ					=	$(SRC:$(SRC_FOLDER)%.c=$(OBJ_FOLDER)%.o)

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