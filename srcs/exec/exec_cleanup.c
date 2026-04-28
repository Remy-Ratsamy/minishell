/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cleanup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 15:37:06 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 19:46:11 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_all_heredoc_fds(t_token *tokens)
{
	t_token	*all_tokens;

	all_tokens = tokens;
	while (all_tokens)
	{
		if (all_tokens->heredoc_fd != -1)
		{
			close(all_tokens->heredoc_fd);
			all_tokens->heredoc_fd = -1;
		}
		all_tokens = all_tokens->next;
	}
}

static void	cleanup_pipeline(t_cleanup *cleanup)
{
	t_pipeline	*tmp;

	while (cleanup->head_pipeline)
	{
		tmp = cleanup->head_pipeline->next;
		free(cleanup->head_pipeline);
		cleanup->head_pipeline = tmp;
	}
	cleanup->pipeline = NULL;
	cleanup->head_pipeline = NULL;
}

void	cleanup_and_exit(t_cleanup *cleanup, int status)
{
	if (cleanup->line)
		free(cleanup->line);
	if (cleanup->pipeline)
		cleanup_pipeline(cleanup);
	if (cleanup->tokens)
	{
		if (cleanup->tokens->heredoc_fd != -1)
			close_all_heredoc_fds(cleanup->tokens);
		free_tokens(cleanup->tokens);
	}
	if (cleanup->ast)
		free_ast(cleanup->ast);
	free_envp(cleanup->envp);
	rl_clear_history();
	exit(status);
}
