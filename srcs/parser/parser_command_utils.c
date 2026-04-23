/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by gechavia          #+#    #+#             */
/*   Updated: 2026/03/13 17:02:02 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirection(t_token *token)
{
	return (token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_REDIR_HEREDOC
		|| token->type == TOKEN_REDIR_APPEND);
}

int	count_tokens_word(t_token *token, int length)
{
	int		count;
	t_token	*current;
	int		i;

	i = 0;
	count = 0;
	current = token;
	while (current && i < length)
	{
		if (is_redirection(current))
		{
			current = current->next;
			if (current && i++ < length)
				current = current->next;
			i++;
			continue ;
		}
		if (current->type == TOKEN_WORD)
			count++;
		current = current->next;
		i++;
	}
	return (count);
}

void	free_redirections(t_redir *redirections)
{
	t_redir	*current;
	t_redir	*temp;

	if (!redirections)
		return ;
	current = redirections;
	while (current)
	{
		temp = current->next;
		if (current->file)
			free(current->file);
		if (current->heredoc_content)
			free(current->heredoc_content);
		free(current);
		current = temp;
	}
	return ;
}

int	print_parser_redir_error(t_token *token)
{
	if (token->type != TOKEN_WORD)
	{
		ft_putstr_fd("minishell :syntax error near unexpected token `", 2);
		ft_putstr_fd(token->str, 2);
		ft_putstr_fd("'\n", 2);
		return (-1);
	}
	return (0);
}

void	print_unexpected_token(void)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
		2);
}
