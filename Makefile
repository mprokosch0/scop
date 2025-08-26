NAME = scop

CC = cc

CFLAGS = -Wall -Wextra -Werror -Iincludes -Iglew-2.2.0/include -O3 -I$(LIBFT_PATH) -g

LDFLAGS = -Lglew-2.2.0/lib -lglfw -lGLEW -lGL -lm

SRCS = 	srcs/main.c srcs/parsing.c srcs/rotate.c\

OBJ_DIR = obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[1;33m
WHITE = \033[0m

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS)
	export LD_LIBRARY_PATH=glew-2.2.0/lib:$LD_LIBRARY_PATH

$(LIBFT):
	@echo "$(GREEN)Compiling the Libft ..."
	@make bonus -sC $(LIBFT_PATH)
	@echo "$(GREEN)Compiling Complete !$(WHITE)"

clean:
	@echo "$(BLUE)Removing objects files ..."
	@make -sC $(LIBFT_PATH) clean
	@rm -rf $(OBJ_DIR)
	@echo "$(BLUE)Removing Complete !$(WHITE)"

fclean: clean
	@echo "$(YELLOW)Removing archives and executables ..."
	@rm -f $(LIBFT)
	@rm -f $(NAME)
	@echo "$(YELLOW)Removing Complete !$(WHITE)"

re: fclean all

re_bonus: fclean bonus

.PHONY: all clean fclean re bonus re_bonus