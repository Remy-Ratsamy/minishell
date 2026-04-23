/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_create_argv.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/20 14:32:45 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_invalid_token(t_token *current)
{
	if (current->type == TOKEN_LPAREN || current->type == TOKEN_RPAREN)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `('\n", 2);
		return (1);
	}
	return (0);
}

static t_token	*skip_redirection(t_token *current, int *token_index,
		int length)
{
	if (current->type == TOKEN_REDIR_APPEND
		|| current->type == TOKEN_REDIR_HEREDOC
		|| current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT)
	{
		(*token_index)++;
		current = current->next;
		if (current && *token_index < length)
		{
			(*token_index)++;
			current = current->next;
		}
	}
	return (current);
}

static int	add_word(char **argv, int *i, t_token *current)
{
	if (current->type == TOKEN_WORD)
	{
		if (current->quote == NO_QUOTE && current->str[0] == '\0')
			return (0);
		argv[*i] = remove_quote(current->str);
		if (!argv[*i])
			return (1);
		(*i)++;
	}
	return (0);
}

static int	fill_argv(char **argv, t_token *tokens, int length)
{
	int		i;
	int		token_index;
	t_token	*current;

	i = 0;
	token_index = 0;
	current = tokens;
	while (current && token_index < length)
	{
		if (is_invalid_token(current))
			return (-1);
		if (is_redirection(current))
		{
			current = skip_redirection(current, &token_index, length);
			continue ;
		}
		if (!current)
			break ;
		if (add_word(argv, &i, current))
			return (-1);
		token_index++;
		current = current->next;
	}
	argv[i] = NULL;
	return (0);
}

char	**create_argv(t_token *tokens, int count, int length)
{
	char	**argv;
	int		i;

	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (i < count)
	{
		argv[i] = NULL;
		i++;
	}
	if (fill_argv(argv, tokens, length) == -1)
	{
		ft_free_split(argv);
		return (NULL);
	}
	return (argv);
}
