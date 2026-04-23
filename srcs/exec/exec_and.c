/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 15:07:59 by gechavia          #+#    #+#             */
/*   Updated: 2025/12/22 17:53:46 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_and(t_node *node, char ***envp, t_cleanup *cleanup)
{
	int	status;

	status = 0;
	if (node->left->type == NODE_COMMAND)
		status = exec_command(node->left->command, envp, cleanup);
	else if (node->left->type != NODE_COMMAND)
		status = exec_ast(node->left, envp, cleanup);
	if (status == 0)
	{
		if (node->right->type == NODE_COMMAND && status == 0)
			status = exec_command(node->right->command, envp, cleanup);
		else
			status = exec_ast(node->right, envp, cleanup);
	}
	return (status);
}
