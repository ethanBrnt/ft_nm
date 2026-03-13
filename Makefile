# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eburnet <eburnet@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/12 10:38:36 by eburnet           #+#    #+#              #
#    Updated: 2026/03/12 10:39:57 by eburnet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_nm
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g3
SRCS = main.c
OBJS = $(SRCS:.c=.o)
HEADERS = ft_nm.h

all: $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re