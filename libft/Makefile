NAME = libft.a

CC = gcc

FLAGS = -Wall -Wextra -Werror -I ./

P1 = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
       ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
       ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
       ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
       ft_atoi.c ft_calloc.c ft_strdup.c ft_freee.c ft_len_int.c ft_strcmp.c \
	   ft_minf.c \

P2 = ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c \
     ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
     ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c get_next_line.c \
	 ft_strjoin_free.c ft_realloc.c ft_ftoa.c\

PBONUS = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c \
	 ft_lstdelone_bonus.c ft_lstadd_back_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c \
	 ft_lstmap_bonus.c

PALL = $(P1) $(P2)

OBJS = $(PALL:.c=.o)

BPALL = $(PALL) $(PBONUS)

BOBJS = $(BPALL:.c=.o)

.c.o:
	$(CC) $(FLAGS) -c $< -o  $(<:.c=.o)

$(NAME): $(OBJS)
	ar -r $(NAME) $(OBJS)

bonus : $(BOBJS)
	ar -r $(NAME) $(BOBJS)

all: $(NAME)

clean:
	rm -f $(BOBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
