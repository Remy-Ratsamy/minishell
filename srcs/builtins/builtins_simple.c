/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_simple.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 11:24:28 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 19:43:22 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_command *cmd)
{
	char	cwd[PATH_MAX];

	(void)cmd;
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

static int	print_echo_arg(char *arg)
{
	if (write(STDOUT_FILENO, arg, ft_strlen(arg)) == -1)
		return (-1);
	return (0);
}

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->argv[1] && ft_strcmp(cmd->argv[1], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->argv[i])
	{
		if (print_echo_arg(cmd->argv[i]) == -1)
			return (1);
		if (cmd->argv[i + 1])
			if (write(STDOUT_FILENO, " ", 1) == -1)
				return (1);
		i++;
	}
	if (newline)
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (1);
	return (0);
}

int	builtin_env(t_command *cmd, char **envp)
{
	int	i;

	if (cmd->argv[1])
	{
		ft_putendl_fd("minishell: env: too many arguments", 2);
		return (1);
	}
	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
