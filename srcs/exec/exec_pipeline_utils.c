/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 18:20:25 by gechavia          #+#    #+#             */
/*   Updated: 2026/03/14 18:13:16 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_pipeline	*new_pipeline(t_command *cmd)
{
	t_pipeline	*new;

	if (!cmd)
		return (NULL);
	new = malloc(sizeof(t_pipeline));
	if (!new)
		return (NULL);
	new->cmd = cmd;
	new->next = NULL;
	return (new);
}

static void	pipeline_add_back(t_pipeline **lst, t_pipeline *new)
{
	t_pipeline	*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_pipeline	*extract_cmd(t_node *node)
{
	t_pipeline	*list;
	t_pipeline	*left_list;
	t_pipeline	*right_list;

	if (!node)
		return (NULL);
	if (node->type == NODE_COMMAND)
		return (new_pipeline(node->command));
	if (node->type != NODE_PIPE)
		return (NULL);
	left_list = extract_cmd(node->left);
	right_list = extract_cmd(node->right);
	if (!left_list)
		return (right_list);
	list = left_list;
	if (right_list)
		pipeline_add_back(&list, right_list);
	return (list);
}

void	wait_all(pid_t g_signal)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = waitpid(-1, &status, 0);
	while ((pid) > 0)
	{
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			g_exit_status = 130;
		else if (pid == g_signal)
			handle_status(status);
		pid = waitpid(-1, &status, 0);
	}
	signal(SIGINT, handler_signal);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			g_exit_status = 130;
		else if (sig == SIGQUIT)
		{
			write(2, "Quit (core dumped)\n", 19);
			g_exit_status = 131;
		}
	}
}
