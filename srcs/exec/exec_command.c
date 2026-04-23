/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:29:02 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/20 14:32:47 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	char	*path;
	int		err;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	apply_redirections(cmd->redirections, cleanup);
	path = get_path(cmd->argv[0], *envp);
	if (!path)
	{
		err = 127;
		if (ft_strchr(cmd->argv[0], '/') && access(cmd->argv[0], F_OK) == 0)
			err = 126;
		print_command_error(cmd->argv[0], err);
		cleanup_and_exit(cleanup, err);
	}
	execve(path, cmd->argv, *envp);
	if (errno == ENOEXEC)
		retry_as_shell_script(path, cmd->argv, *envp);
	(print_command_error(cmd->argv[0], 126),
		cleanup_and_exit(cleanup, 126));
}

static void	setup_parent_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static void	restore_parent_signals(void)
{
	signal(SIGINT, handler_signal);
	signal(SIGQUIT, SIG_IGN);
}

static int	fork_and_wait(t_command *cmd, char ***envp,
		t_cleanup *cleanup)
{
	pid_t	pid;
	int		status;

	setup_parent_signals();
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 1);
	if (pid == 0)
		exec_child(cmd, envp, cleanup);
	waitpid(pid, &status, 0);
	restore_parent_signals();
	return (get_exit_code(status));
}

int	exec_command(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	int	saved_stdout;
	int	ret;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (is_parent_builtin(cmd->argv[0]))
		return (apply_redirections(cmd->redirections, cleanup),
			execute_builtin_parent(cmd, envp, cleanup));
	if (is_simple_builtin(cmd->argv[0]))
	{
		saved_stdout = dup(STDOUT_FILENO);
		apply_redirections(cmd->redirections, cleanup);
		ret = execute_builtin_simple(cmd, envp);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
		return (ret);
	}
	return (fork_and_wait(cmd, envp, cleanup));
}
