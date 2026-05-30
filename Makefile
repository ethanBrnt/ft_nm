# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/12 10:38:36 by eburnet           #+#    #+#              #
#    Updated: 2026/05/30 12:02:13 by eburnet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_nm
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
SRCS = main.c quick_sort.c nm_32.c nm_64.c
OBJS = $(SRCS:.c=.o)
HEADERS = ft_nm.h
LIBFT_PATH = ./libft

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_PATH)/libft.a
	$(CC) $(CFLAGS) $(OBJS) -L $(LIBFT_PATH) -lft  -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_PATH)/libft.a:
	make -C $(LIBFT_PATH)

clean:
	rm *.o
	make -C $(LIBFT_PATH) clean
	
fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_PATH) fclean
	
re: fclean all

.PHONY : all clean fclean re