/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_applyredir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 09:08:43 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/20 14:26:59 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	*init_redir_node(t_token *current)
{
	t_redir	*redir;

	if (!current || !current->next || current->next->type != TOKEN_WORD)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = token_to_redir_type(current->type);
	if (current->next->str)
		redir->file = remove_quote(current->next->str);
	else
		redir->file = ft_strdup("");
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	redir->heredoc_fd = -1;
	redir->heredoc_content = NULL;
	return (redir);
}

static int	apply_heredoc_content(t_redir *redir, t_token *current)
{
	if (!redir || !current)
		return (0);
	if (current->type == TOKEN_REDIR_HEREDOC)
	{
		if (current->heredoc_content)
			redir->heredoc_content = ft_strdup(current->heredoc_content);
		else
			redir->heredoc_content = ft_strdup("");
		if (!redir->heredoc_content)
			return (0);
		if (current->heredoc_fd != -1)
		{
			close(current->heredoc_fd);
			current->heredoc_fd = -1;
		}
	}
	return (1);
}

static t_redir	*create_redir_node(t_token *current)
{
	t_redir	*redir;

	if (!current || !current->next || current->next->type != TOKEN_WORD)
	{
		if (current && current->next)
			print_parser_redir_error(current->next);
		else
			print_unexpected_token();
		return (NULL);
	}
	redir = init_redir_node(current);
	if (!redir)
		return (NULL);
	redir->type = REDIR_HEREDOC;
	free(redir->file);
	redir->file = ft_strdup(current->next->str);
	if (!redir->file)
		return (free(redir), NULL);
	redir->heredoc_fd = current->heredoc_fd;
	if (!apply_heredoc_content(redir, current))
		return (free(redir->file), free(redir), NULL);
	return (redir);
}

t_redir	*parse_redirections(t_token *tokens, int length)
{
	t_token	*current;
	t_redir	*redir;
	t_redir	*head;

	current = tokens;
	head = NULL;
	while (current && length--)
	{
		if (is_redirection(current))
		{
			if (current->type == TOKEN_REDIR_HEREDOC)
				redir = create_redir_node(current);
			else
				redir = init_redir_node(current);
			if (!redir)
			{
				free_redirections(head);
				return (NULL);
			}
			redir_add_back(&head, redir);
			current = current->next;
		}
		current = current->next;
	}
	return (head);
}
