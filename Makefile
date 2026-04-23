# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/12 11:54:28 by gechavia          #+#    #+#              #
#    Updated: 2026/03/13 16:58:13 by gechavia         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Wextra

LEXER_SRCS = srcs/lexer/lexer.c \
			srcs/lexer/lexer_handlers.c \
			srcs/lexer/lexer_utils.c \
			srcs/lexer/lexer_token.c

AST_SRCS = srcs/ast/ast_node.c \
			srcs/ast/ast_utils.c 

PARSER_SRCS = srcs/parser/parser.c \
			srcs/parser/parser_utils.c \
			srcs/parser/parser_command.c \
			srcs/parser/parser_command_utils.c \
			srcs/parser/parser_utils2.c \
			srcs/parser/parser_applyredir.c \
			srcs/parser/parser_create_argv.c \

EXPAND_SRCS = srcs/expander/expand.c \
			srcs/expander/expand_utils.c \
			srcs/expander/expand_utils2.c \
			srcs/expander/expand_wildcard.c \

EXEC_SRCS = srcs/exec/exec_and.c \
			srcs/exec/exec_command.c \
			srcs/exec/exec_or.c \
			srcs/exec/exec_path.c \
			srcs/exec/exec_redir_utils.c \
			srcs/exec/exec_redir.c \
			srcs/exec/exec_utils.c \
			srcs/exec/exec.c \
			srcs/exec/exec_pipeline_utils.c \
			srcs/exec/exec_pipeline.c \
			srcs/exec/exec_cleanup.c \

BUILTINS_SRCS = srcs/builtins/builtins_cd.c \
				srcs/builtins/builtins_utils.c \
				srcs/builtins/builtins_export.c \
				srcs/builtins/builtins_unset.c \
				srcs/builtins/builtins_exit.c \
				srcs/builtins/builtins_simple.c \
				srcs/builtins/builtins.c \

HEREDOC_SRCS = srcs/heredoc/heredoc.c \
			   srcs/heredoc/handle_single_heredoc.c \

MAIN_SRCS = srcs/main_utils.c \
			srcs/main_utils2.c \

SRCS = srcs/main.c $(LEXER_SRCS) $(AST_SRCS) $(PARSER_SRCS) $(EXPAND_SRCS) $(EXEC_SRCS) $(HEREDOC_SRCS) $(BUILTINS_SRCS) $(MAIN_SRCS)

OBJS = $(SRCS:.c=.o)

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I includes -I $(LIBFT_DIR) -c $< -o $@
	
$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re