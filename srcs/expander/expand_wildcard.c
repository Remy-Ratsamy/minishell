/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 15:15:23 by gechavia          #+#    #+#             */
/*   Updated: 2026/03/09 16:08:38 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	match_wildcard(char *str, char *pattern)
{
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern == '*')
	{
		while (*pattern == '*')
			pattern++;
		if (*pattern == '\0')
			return (1);
		while (*str)
		{
			if (match_wildcard(str, pattern))
				return (1);
			str++;
		}
		return (0);
	}
	if (*str == '\0' || *pattern != *str)
		return (0);
	return (match_wildcard(str + 1, pattern + 1));
}

static t_token	*new_word_token(char *name)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = TOKEN_WORD;
	token->quote = NO_QUOTE;
	token->str = ft_strdup(name);
	token->heredoc_content = NULL;
	token->next = NULL;
	return (token);
}

static int	add_match(t_token *token, char *name, int matched, t_token **last)
{
	t_token	*new;

	if (matched == 0)
	{
		free(token->str);
		token->str = ft_strdup(name);
		*last = token;
	}
	else
	{
		new = new_word_token(name);
		if (!new)
			return (-1);
		new->next = (*last)->next;
		(*last)->next = new;
		*last = new;
	}
	return (0);
}

static int	collect_matches(DIR *dir, char *pattern, t_token *token)
{
	struct dirent	*entry;
	t_token			*last;
	int				matched;

	matched = 0;
	last = token;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.' && match_wildcard(entry->d_name, pattern))
		{
			if (add_match(token, entry->d_name, matched, &last) == -1)
				break ;
			matched++;
		}
		entry = readdir(dir);
	}
	return (matched);
}

void	expand_wildcard_token(t_token *token)
{
	DIR		*dir;
	char	*pattern;

	pattern = ft_strdup(token->str);
	if (!pattern)
		return ;
	dir = opendir(".");
	if (!dir)
		return (free(pattern), (void)0);
	collect_matches(dir, pattern, token);
	closedir(dir);
	free(pattern);
}
