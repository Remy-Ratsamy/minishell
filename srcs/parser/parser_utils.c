/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 16:12:01 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/20 15:05:46 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_operator_info	find_operator(t_token *tokens, int length, t_operator_prio prio)
{
	int				count;
	int				last_op_position;
	int				i;
	t_token_type	last_op_type;
	t_token			*current;

	last_op_position = -1;
	last_op_type = 0;
	count = 0;
	i = -1;
	current = tokens;
	while (current && ++i < length)
	{
		if (current->type == TOKEN_LPAREN)
			count++;
		else if (current->type == TOKEN_RPAREN)
			count--;
		if (count == 0 && is_target_operator(current->type, prio))
		{
			last_op_position = i;
			last_op_type = current->type;
		}
		current = current->next;
	}
	return ((t_operator_info){last_op_position, last_op_type});
}

int	has_wrapping_parentheses(t_token *tokens, int length)
{
	t_token	*current;
	int		i;
	int		count;

	if (tokens->type != TOKEN_LPAREN)
		return (0);
	current = tokens;
	i = 0;
	while (i++ < length - 1 && current)
		current = current->next;
	if (current->type != TOKEN_RPAREN)
		return (0);
	current = tokens;
	count = 0;
	i = 0;
	while (i < length - 1)
	{
		count += (current->type == TOKEN_LPAREN);
		count -= (current->type == TOKEN_RPAREN);
		if (count == 0 && i < length - 1)
			return (0);
		current = current->next;
		i++;
	}
	return (1);
}

int	is_target_operator(t_token_type type, t_operator_prio prio)
{
	if (prio == LOW_PRIO)
		return (type == TOKEN_OR || type == TOKEN_AND);
	else if (prio == MEDIUM_PRIO)
		return (type == TOKEN_PIPE);
	return (0);
}

void	print_syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putstr_fd("'\n", 2);
}

t_node	*handle_operator_parser(t_token *tokens, int length,
		t_operator_info operator_info)
{
	t_node	*node;

	if (operator_info.position == 0)
		return (print_syntax_error(tokens[0].str), NULL);
	if (operator_info.type == TOKEN_AND)
		node = create_node(NODE_AND, NULL);
	else if (operator_info.type == TOKEN_OR)
		node = create_node(NODE_OR, NULL);
	else
		node = create_node(NODE_PIPE, NULL);
	node->left = parse(tokens, operator_info.position);
	node->right = parse(advance_token(tokens, operator_info.position + 1),
			length - operator_info.position - 1);
	if (node->left == NULL || node->right == NULL)
		return (free_ast(node), NULL);
	return (node);
}
