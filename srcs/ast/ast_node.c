/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 11:00:55 by gechavia          #+#    #+#             */
/*   Updated: 2026/02/19 12:50:22 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*create_node(t_node_type type, t_command *command)
{
	t_node	*new_node;

	new_node = malloc(sizeof(t_node));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->command = command;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

int	count_nodes(t_node *node)
{
	if (node == NULL)
		return (0);
	if (node->type == NODE_COMMAND)
		return (1);
	else
		return (1 + count_nodes(node->left) + count_nodes(node->right));
}
