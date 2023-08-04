# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: adugain <adugain@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/19 21:41:34 by adugain           #+#    #+#              #
#    Updated: 2023/08/04 11:09:20 by adugain          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

LIBFT = libft/libft.a

SRC = pipex.c pipex_utils.c pipex_errors.c cmds.c

CFLAGS	= -Wextra -Wall -Werror -MMD -g3

CC = gcc

OBJ_DIR = ./objs/

OBJ = $(SRC:.c=.o)

OBJ := $(addprefix $(OBJ_DIR), $(OBJ))

DEP := $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

$(LIBFT) :
	make -C ./libft
clean:
	rm -rf $(OBJ_DIR)
	make fclean -C ./libft

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

$(OBJ_DIR)%.o : %.c
	@mkdir -p $(@D)
	${CC} ${CFLAGS} -c $< -o $@

-include $(DEP)