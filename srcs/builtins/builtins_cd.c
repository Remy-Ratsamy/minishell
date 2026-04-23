/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:45:57 by gechavia          #+#    #+#             */
/*   Updated: 2026/02/19 13:18:11 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_env_forcd(char ***envp, char *name, char *value)
{
	int		i;
	int		len;
	char	*new_var;

	len = ft_strlen(name);
	i = 0;
	while ((*envp)[i])
	{
		if (!ft_strncmp((*envp)[i], name, len) && (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			new_var = ft_strjoin3(name, "=", value);
			if (!new_var)
				return (1);
			(*envp)[i] = new_var;
			return (0);
		}
		i++;
	}
	return (env_add(envp, name, value));
}

static int	set_pwd_oldpwd(char ***envp, char *oldcwd)
{
	char	newcwd[PATH_MAX];

	if (!getcwd(newcwd, sizeof(newcwd)))
		return (perror("getcwd"), 1);
	if (update_env_forcd(envp, "OLDPWD", oldcwd))
		return (1);
	if (update_env_forcd(envp, "PWD", newcwd))
		return (1);
	return (0);
}

static char	*get_dash_path(char **envp)
{
	char	*oldpwd;
	char	*path;

	oldpwd = ft_getenv("OLDPWD", envp);
	if (!oldpwd)
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		return (NULL);
	}
	printf("%s\n", oldpwd);
	path = ft_strdup(oldpwd);
	return (path);
}

static char	*get_cd_path(t_command *cmd, char **envp, int *should_free)
{
	char	*path;
	char	*home;

	*should_free = 0;
	if (!cmd->argv[1])
	{
		home = ft_getenv("HOME", envp);
		if (!home)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), NULL);
		path = ft_strdup(home);
		if (!path)
			return (NULL);
		*should_free = 1;
	}
	else if (!ft_strcmp(cmd->argv[1], "-"))
	{
		path = get_dash_path(envp);
		if (!path)
			return (NULL);
		*should_free = 1;
	}
	else
		path = cmd->argv[1];
	return (path);
}

int	builtin_cd(t_command *cmd, char ***envp)
{
	char	oldcwd[PATH_MAX];
	char	*path;
	int		should_free;

	if (cmd->argv[1] && cmd->argv[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 2);
	if (!getcwd(oldcwd, sizeof(oldcwd)))
		return (perror("getcwd"), 1);
	path = get_cd_path(cmd, *envp, &should_free);
	if (!path)
		return (1);
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		if (should_free)
			free(path);
		return (1);
	}
	if (should_free)
		free(path);
	return (set_pwd_oldpwd(envp, oldcwd));
}
