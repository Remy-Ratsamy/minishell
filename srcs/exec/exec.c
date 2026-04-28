/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/22 15:50:17 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/20 15:05:46 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_ast(t_node *node, char ***envp, t_cleanup *cleanup)
{
	int	status;

	status = 0;
	if (node == NULL)
		return (-1);
	if (node->type == NODE_COMMAND)
		status = exec_command(node->command, envp, cleanup);
	else if (node->type == NODE_AND)
		status = exec_and(node, envp, cleanup);
	else if (node->type == NODE_OR)
		status = exec_or(node, envp, cleanup);
	else if (node->type == NODE_PIPE)
		status = exec_pipeline(node, envp, cleanup);
	return (status);
}
