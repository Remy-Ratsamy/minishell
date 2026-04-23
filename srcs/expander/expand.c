/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 11:28:47 by gechavia          #+#    #+#             */
/*   Updated: 2026/02/23 16:22:59 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_token_str(t_expand_tokens *expand_tokens, char *str)
{
	char	*expanded;

	expand_tokens->str = str;
	expanded = expand(expand_tokens);
	return (expanded);
}

static void	expand_heredoc_content(t_expand_tokens *expand_tokens,
		t_token *tokens)
{
	char	*expanded_heredoc;

	if (tokens->type != TOKEN_REDIR_HEREDOC || !tokens->heredoc_content)
		return ;
	expanded_heredoc = expand_token_str(expand_tokens, tokens->heredoc_content);
	if (expanded_heredoc)
	{
		free(tokens->heredoc_content);
		tokens->heredoc_content = expanded_heredoc;
	}
}

static void	expand_token(t_expand_tokens *expand_tokens, t_token *tokens)
{
	char	*expanded_str;

	if (tokens->quote == SINGLE_QUOTE)
		return ;
	expanded_str = expand_token_str(expand_tokens, tokens->str);
	if (!expanded_str)
		return ;
	expand_heredoc_content(expand_tokens, tokens);
	if (tokens->type == TOKEN_REDIR_HEREDOC)
	{
		free(expanded_str);
		return ;
	}
	free(tokens->str);
	tokens->str = expanded_str;
	if (tokens->quote == NO_QUOTE && ft_strchr(tokens->str, '*'))
		expand_wildcard_token(tokens);
}

void	expander(t_token *tokens, char **envp)
{
	t_token			*current;
	t_expand_tokens	expand_tokens;

	current = tokens;
	expand_tokens.envp = envp;
	while (current)
	{
		expand_token(&expand_tokens, current);
		current = current->next;
	}
	return ;
}

char	*expand(t_expand_tokens *tokens)
{
	int		total_len;
	char	*expanded_str;

	total_len = expanded_len(tokens->str, tokens->envp);
	expanded_str = malloc(sizeof(char) * (total_len + 1));
	if (!expanded_str)
		return (NULL);
	tokens->result = expanded_str;
	if (!copy_expanded_str(tokens))
	{
		free(expanded_str);
		return (NULL);
	}
	return (expanded_str);
}
