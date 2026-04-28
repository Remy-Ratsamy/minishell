/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 08:54:10 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 20:06:58 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_redirections(t_token *tokens, int length)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current && i < length)
	{
		if (is_redirection(current))
		{
			if (!current->next)
				return (print_unexpected_token(), g_exit_status = 2, 0);
			if (current->next->type != TOKEN_WORD)
			{
				print_parser_redir_error(current->next);
				g_exit_status = 2;
				return (0);
			}
		}
		current = current->next;
		i++;
	}
	return (1);
}

t_command	*parse_command(t_token *tokens, int length)
{
	int			count;
	char		**argv;
	t_redir		*head_redir;
	t_command	*command;

	if (!validate_redirections(tokens, length))
		return (NULL);
	count = count_tokens_word(tokens, length);
	argv = create_argv(tokens, count, length);
	if (!argv)
		return (NULL);
	head_redir = parse_redirections(tokens, length);
	command = malloc(sizeof(t_command));
	if (!command)
		return (free_redirections(head_redir), ft_free_split(argv), NULL);
	command->argv = argv;
	command->redirections = head_redir;
	return (command);
}

t_redir_type	token_to_redir_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_IN);
	if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUT);
	if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	if (type == TOKEN_REDIR_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_NONE);
}
