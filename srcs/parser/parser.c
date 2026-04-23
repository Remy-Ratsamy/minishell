/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 10:47:17 by gechavia          #+#    #+#             */
/*   Updated: 2026/02/19 12:45:39 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse(t_token *tokens, int length)
{
	t_operator_info	operator_info;

	if (tokens == NULL || length == 0)
		return (ft_putstr_fd("minishell: syntax error near "
				"unexpected token `newline'\n", 2), NULL);
	if (!tokens)
		return (NULL);
	operator_info = find_operator(tokens, length, LOW_PRIO);
	if (operator_info.position != -1)
		return (handle_operator_parser(tokens, length, operator_info));
	operator_info = find_operator(tokens, length, MEDIUM_PRIO);
	if (operator_info.position != -1)
		return (handle_operator_parser(tokens, length, operator_info));
	if (has_wrapping_parentheses(tokens, length))
		return (parse(tokens->next, length - 2));
	if (!has_wrapping_parentheses(tokens, length)
		&& tokens->type == TOKEN_LPAREN)
		return (ft_putstr_fd
			("minishell: syntax error near unexpected end of file\n", 2),
			NULL);
	return (create_command_node(tokens, length));
}
