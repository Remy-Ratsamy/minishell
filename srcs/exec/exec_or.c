/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_or.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 17:37:02 by gechavia          #+#    #+#             */
/*   Updated: 2025/12/22 17:54:13 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_or(t_node *node, char ***envp, t_cleanup *cleanup)
{
	int	status;

	status = 0;
	if (node->left->type == NODE_COMMAND)
		status = exec_command(node->left->command, envp, cleanup);
	else if (node->left->type != NODE_COMMAND)
		status = exec_ast(node->left, envp, cleanup);
	if (status != 0)
	{
		if (node->right->type == NODE_COMMAND)
			status = exec_command(node->right->command, envp, cleanup);
		else
			status = exec_ast(node->right, envp, cleanup);
	}
	return (status);
}
