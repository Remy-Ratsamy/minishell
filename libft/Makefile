# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gechavia <gechavia@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/27 15:55:25 by gechavia           #+#    #+#              #
#    Updated: 2025/10/13 13:14:18 by gechavia          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC	= cc
CFLAGS	= -Wall -Wextra -Werror
NAME = libft.a

SRCS	=	ft_isalpha.c \
		ft_isdigit.c \
		ft_isalnum.c \
		ft_isascii.c \
		ft_isprint.c \
		ft_strlen.c \
		ft_strtrim.c \
		ft_memset.c \
		ft_bzero.c \
		ft_memcpy.c \
		ft_memmove.c \
		ft_strlcpy.c \
		ft_strlcat.c \
		ft_substr.c \
		ft_putchar_fd.c \
		ft_toupper.c \
		ft_strjoin.c \
		ft_tolower.c \
		ft_strchr.c \
		ft_striteri.c \
		ft_strmapi.c \
		ft_itoa.c \
		ft_putstr_fd.c \
		ft_putendl_fd.c \
		ft_split.c \
		ft_strrchr.c \
		ft_strncmp.c \
		ft_memchr.c \
		ft_strdup.c \
		ft_memcmp.c \
		ft_putnbr_fd.c \
		ft_strnstr.c \
		ft_atoi.c \
		ft_calloc.c \
		ft_strcmp.c \
		ft_strlen_tab.c \
		ft_free_split.c \
		ft_atol.c \
		ft_atod.c \
		ft_isspace.c \
		ft_isdelimiter.c

SRCS_BONUS	=	ft_lstnew.c \
		ft_lstadd_front.c \
		ft_lstsize.c \
		ft_lstlast.c \
		ft_lstadd_back.c \
		ft_lstdelone.c \
		ft_lstclear.c \
		ft_lstiter.c \
		ft_lstmap.c 

OBJS	= $(SRCS:.c=.o)
OBJS_BONUS	= $(SRCS_BONUS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: all $(OBJS_BONUS)
	ar rcs $(NAME) $(OBJS_BONUS)

.PHONY: all re fclean clean bonus

