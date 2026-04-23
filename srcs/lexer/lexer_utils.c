/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:16:02 by gechavia          #+#    #+#             */
/*   Updated: 2025/10/22 10:52:52 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_spaces(char *line, int *i)
{
	while (ft_isspace(line[*i]))
		(*i)++;
}

t_token_type	get_operator_type(char *str)
{
	if (str[0] == '>' && str[1] == '>')
		return (TOKEN_REDIR_APPEND);
	if (str[0] == '<' && str[1] == '<')
		return (TOKEN_REDIR_HEREDOC);
	if (str[0] == '|' && str[1] == '|')
		return (TOKEN_OR);
	if (str[0] == '&' && str[1] == '&')
		return (TOKEN_AND);
	if (str[0] == '|')
		return (TOKEN_PIPE);
	if (str[0] == '<')
		return (TOKEN_REDIR_IN);
	if (str[0] == '>')
		return (TOKEN_REDIR_OUT);
	if (str[0] == '(')
		return (TOKEN_LPAREN);
	if (str[0] == ')')
		return (TOKEN_RPAREN);
	return (TOKEN_ERROR);
}

t_quote_type	get_quote_type(char *line, int *i, char *quote_char)
{
	t_quote_type	quote_type;

	if (line[*i] == '"')
	{
		*quote_char = '"';
		quote_type = DOUBLE_QUOTE;
	}
	else
	{
		*quote_char = '\'';
		quote_type = SINGLE_QUOTE;
	}
	return (quote_type);
}
