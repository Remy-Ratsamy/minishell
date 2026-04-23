/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:58:04 by gechavia          #+#    #+#             */
/*   Updated: 2026/03/13 15:20:30 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	apply_redirections(t_redir *redir, t_cleanup *cleanup)
{
	t_redir	*current;

	current = redir;
	while (current)
	{
		if (current->type == REDIR_IN)
			apply_in(current, cleanup);
		else if (current->type == REDIR_OUT)
			apply_out(current, cleanup);
		else if (current->type == REDIR_APPEND)
			apply_append(current, cleanup);
		else if (current->type == REDIR_HEREDOC)
			apply_heredoc(current, cleanup);
		current = current->next;
	}
}
