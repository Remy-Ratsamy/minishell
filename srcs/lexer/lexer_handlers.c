/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:51:41 by gechavia          #+#    #+#             */
/*   Updated: 2025/10/22 10:54:42 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_operator(char *line, int *i, t_token **tokens)
{
	t_token_type	type;
	t_token			*token;
	int				len;

	type = get_operator_type(&line[*i]);
	if (type == TOKEN_ERROR)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putchar_fd(line[*i], 2);
		ft_putstr_fd("'\n", 2);
		return (-1);
	}
	if (type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC
		|| type == TOKEN_OR || type == TOKEN_AND)
		len = 2;
	else
		len = 1;
	token = create_token(type, NO_QUOTE, &line[*i], len);
	if (!token)
		return (-1);
	token_add_back(tokens, token);
	(*i) += len;
	return (0);
}

int	handle_word(char *line, int *i, t_token **tokens)
{
	int		j;
	int		len;
	t_token	*token;

	j = *i;
	while (!ft_isspace(line[j]) && !ft_isdelimiter(line[j]) && line[j]
		!= '"' && line[j] != '\'' && line[j] != '\0')
		j++;
	len = j - *i;
	token = create_token(TOKEN_WORD, NO_QUOTE, &line[*i], len);
	if (!token)
		return (-1);
	token_add_back(tokens, token);
	*i = j;
	return (0);
}

int	handle_quotes(char *line, int *i, t_token **tokens)
{
	t_quote_type	quote_type;
	t_token			*token;
	char			quote_char;
	int				j;
	int				len;

	quote_type = get_quote_type(line, i, &quote_char);
	j = (*i);
	(*i)++;
	while (line[*i] != quote_char && line[*i] != '\0')
		(*i)++;
	if (line[*i] == '\0')
		return (-1);
	len = *i - j + 1;
	token = create_token(TOKEN_WORD, quote_type, &line[j], len);
	if (!token)
		return (-1);
	token_add_back(tokens, token);
	(*i)++;
	return (0);
}
