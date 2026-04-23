/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:11:15 by gechavia          #+#    #+#             */
/*   Updated: 2026/03/13 15:38:02 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_command_error(char *cmd, int error_type)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": ", 2);
	if (error_type == 127)
		ft_putstr_fd("command not found\n", 2);
	else if (error_type == 126)
		ft_putstr_fd("Permission denied\n", 2);
}

int	get_exit_code(int status)
{
	if (WIFEXITED(status))
		return (g_exit_status = WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (g_exit_status = (128 + WTERMSIG(status)));
	return (-1);
}

void	setup_signals(void)
{
	signal(SIGINT, handler_signal);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	setup_child_pipe(int prev_fd, int *pipefd, int has_next)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (has_next && pipefd)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
}

void	retry_as_shell_script(char *path, char **argv, char **envp)
{
	char	**new_argv;
	int		argc;
	int		i;

	argc = 0;
	while (argv[argc])
		argc++;
	new_argv = malloc(sizeof(char *) * (argc + 2));
	if (!new_argv)
		return ;
	new_argv[0] = "/bin/sh";
	new_argv[1] = path;
	i = 1;
	while (argv[i])
	{
		new_argv[i + 1] = argv[i];
		i++;
	}
	new_argv[argc + 1] = NULL;
	execve("/bin/sh", new_argv, envp);
	free(new_argv);
}
