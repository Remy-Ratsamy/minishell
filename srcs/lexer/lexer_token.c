/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 13:14:49 by gechavia          #+#    #+#             */
/*   Updated: 2026/03/13 11:52:33 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_token_type type, t_quote_type quote, char *str, int len)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->str = ft_substr(str, 0, len);
	if (!new_token->str)
		return (free(new_token), NULL);
	new_token->type = type;
	new_token->quote = quote;
	new_token->next = NULL;
	new_token->heredoc_content = NULL;
	new_token->heredoc_fd = -1;
	return (new_token);
}

void	token_add_back(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	current = *lst;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*temp;

	if (!tokens)
		return ;
	current = tokens;
	while (current)
	{
		temp = current->next;
		free(current->str);
		free(current);
		current = temp;
	}
	return ;
}

t_token	*advance_token(t_token *token, int position)
{
	t_token	*current;
	int		i;

	current = token;
	i = 0;
	while (i < position)
	{
		if (current == NULL)
			return (NULL);
		current = current->next;
		i++;
	}
	return (current);
}

int	ft_tokens_size(t_token *lst)
{
	int		i;
	t_token	*temp;

	i = 0;
	temp = lst;
	if (temp == NULL)
		return (0);
	else
	{
		while (temp != NULL)
		{
			temp = temp->next;
			i++;
		}
	}
	return (i);
}
