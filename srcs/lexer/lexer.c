/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:35:07 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/20 14:52:26 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_to_last(t_token **tokens, char *line, int *i)
{
	t_token	*last;
	char	*segment;
	char	*joined;

	last = *tokens;
	while (last->next)
		last = last->next;
	if (handle_quotes(line, i, tokens) == -1)
		return (-1);
	segment = last->next->str;
	joined = ft_strjoin(last->str, segment);
	if (!joined)
		return (-1);
	free(last->str);
	last->str = joined;
	free(segment);
	free(last->next);
	last->next = NULL;
	return (0);
}

static int	append_word_to_last(t_token **tokens, char *line, int *i)
{
	t_token	*last;
	char	*segment;
	char	*joined;

	last = *tokens;
	while (last->next)
		last = last->next;
	if (handle_word(line, i, tokens) == -1)
		return (-1);
	segment = last->next->str;
	joined = ft_strjoin(last->str, segment);
	if (!joined)
		return (-1);
	free(last->str);
	last->str = joined;
	free(segment);
	free(last->next);
	last->next = NULL;
	return (0);
}

static int	merge_adjacent(t_token **tokens, char *line, int *i)
{
	while (line[*i] && !ft_isspace(line[*i]) && !ft_isdelimiter(line[*i]))
	{
		if (line[*i] == '"' || line[*i] == '\'')
		{
			if (append_to_last(tokens, line, i) == -1)
				return (-1);
		}
		else
		{
			if (append_word_to_last(tokens, line, i) == -1)
				return (-1);
		}
	}
	return (0);
}

static int	lex_token(char *line, int *i, t_token **tokens)
{
	if (line[*i] == '"' || line[*i] == '\'')
	{
		if (handle_quotes(line, i, tokens) == -1)
			return (-1);
	}
	else if (ft_isdelimiter(line[*i]))
	{
		if (handle_operator(line, i, tokens) == -1)
			return (-1);
		return (1);
	}
	else if (handle_word(line, i, tokens) == -1)
		return (-1);
	if (merge_adjacent(tokens, line, i) == -1)
		return (-1);
	return (0);
}

t_token	*lexer(char *line)
{
	t_token	*tokens;
	int		i;
	int		ret;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		skip_spaces(line, &i);
		if (line[i] == '\0')
			break ;
		ret = lex_token(line, &i, &tokens);
		if (ret == -1)
			return (free_tokens(tokens), NULL);
	}
	return (tokens);
}
