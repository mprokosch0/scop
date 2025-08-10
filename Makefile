NAME = scop

CC = cc

CFLAGS = -Wall -Wextra -Werror -Iincludes -O3 -I$(MLX_PATH) -I$(LIBFT_PATH) -g

MLXFLAGS = -lmlx -L$(MLX_PATH) -lXext -lX11 -lm 

SRCS = 	srcs/main.c \
		srcs/key_hooks.c \
		srcs/mlx_utils.c \
		srcs/parsing.c \
		srcs/calculs.c \
		

OBJ_DIR = obj
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)
OBJS_BONUS = $(SRCS_BONUS:%.c=$(OBJ_DIR)/%.bonus.o)

MLX_PATH = ./minilibx-linux
MLX = $(MLX_PATH)/libmlx.a

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

$(OBJ_DIR)/%.bonus.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT) $(MLX)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(MLX) $(LIBFT) $(MLXFLAGS)

bonus: $(OBJS_BONUS) $(LIBFT) $(MLX)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS_BONUS) $(MLX) $(LIBFT) $(MLXFLAGS)

$(MLX):
	@echo "$(GREEN)Compiling the MinilibX ..."
	@make -sC $(MLX_PATH)
	@echo "$(GREEN)Compiling Complete !$(WHITE)"

$(LIBFT):
	@echo "$(GREEN)Compiling the Libft ..."
	@make bonus -sC $(LIBFT_PATH)
	@echo "$(GREEN)Compiling Complete !$(WHITE)"

clean:
	@echo "$(BLUE)Removing objects files ..."
	@make -sC $(MLX_PATH) clean
	@make -sC $(LIBFT_PATH) clean
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)/*.bonus.o
	@echo "$(BLUE)Removing Complete !$(WHITE)"

fclean: clean
	@echo "$(YELLOW)Removing archives and executables ..."
	@rm -f $(LIBFT)
	@rm -f $(NAME)
	@echo "$(YELLOW)Removing Complete !$(WHITE)"

re: fclean all

re_bonus: fclean bonus

.PHONY: all clean fclean re bonus re_bonus