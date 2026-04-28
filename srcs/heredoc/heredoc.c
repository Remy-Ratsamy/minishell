/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:54:14 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 20:09:10 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static int	print_heredoc_error(t_token *token)
{
	if (token)
		fprintf(stderr,
			"minishell: syntax error near unexpected token '%s'\n", token->str);
	else
		fprintf(stderr,
			"minishell: syntax error near unexpected token 'newline'\n");
	g_exit_status = 2;
	return (-1);
}

int	process_heredoc(t_token *tokens, t_cleanup *cleanup)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_HEREDOC)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
				return (print_heredoc_error(current->next));
			if (handle_single_heredoc(current, cleanup) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
