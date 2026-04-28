/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 10:54:13 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 19:46:16 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric_exit(const char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	get_exit_value(const char *str)
{
	long	val;

	val = ft_atol(str);
	return ((unsigned char)val);
}

int	builtin_exit(t_command *cmd, t_cleanup *cleanup)
{
	int	exit_code;

	ft_putendl_fd("exit", 1);
	rl_clear_history();
	if (!cmd->argv[1])
		cleanup_and_exit(cleanup, g_exit_status);
	if (!is_numeric_exit(cmd->argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->argv[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		cleanup_and_exit(cleanup, 2);
	}
	if (cmd->argv[2])
		return (ft_putendl_fd("minishell: exit: too many arguments", 2), 1);
	exit_code = get_exit_value(cmd->argv[1]);
	g_exit_status = exit_code;
	cleanup_and_exit(cleanup, exit_code);
	return (g_exit_status);
}
