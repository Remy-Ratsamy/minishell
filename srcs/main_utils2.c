/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 16:54:17 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/20 15:05:46 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	cleanup_iteration(t_cleanup *cleanup)
{
	if (cleanup->tokens)
		free_tokens(cleanup->tokens);
	if (cleanup->ast)
		free_ast(cleanup->ast);
	cleanup->tokens = NULL;
	cleanup->ast = NULL;
}
