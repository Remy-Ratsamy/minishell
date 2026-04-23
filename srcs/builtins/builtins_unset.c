/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/14 07:44:19 by gechavia          #+#    #+#             */
/*   Updated: 2026/02/19 13:18:39 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_var(char ***envp, char *name)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	if (!envp || !*envp || !name)
		return ;
	len = ft_strlen(name);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], name, len) == 0
			&& ((*envp)[i][len] == '='
				|| (*envp)[i][len] == '\0'))
		{
			free((*envp)[i]);
			j = i;
			while ((*envp)[j])
			{
				(*envp)[j] = (*envp)[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

int	builtin_unset(t_command *cmd, char ***env)
{
	int	i;
	int	ret;

	if (!cmd || !cmd->argv)
		return (1);
	i = 1;
	ret = 0;
	while (cmd->argv[i])
	{
		if (!is_valid_env(cmd->argv[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(cmd->argv[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			ret = 1;
		}
		else
			remove_var(env, cmd->argv[i]);
		i++;
	}
	return (ret);
}
