/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:28:24 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/20 15:05:46 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*read_heredoc_line(char *delimiter, char *buffer)
{
	char	*line;
	char	*line_with_newline;
	char	*joined_buffer;

	line = readline("> ");
	if (!line)
		return (free(buffer), NULL);
	if (ft_strcmp(line, delimiter) == 0)
		return (free(line), buffer);
	line_with_newline = ft_strjoin(line, "\n");
	if (!line_with_newline)
		return (free(buffer), free(line), NULL);
	joined_buffer = ft_strjoin(buffer, line_with_newline);
	if (!joined_buffer)
		return (free(buffer), free(line_with_newline), free(line), NULL);
	free(buffer);
	free(line_with_newline);
	free(line);
	return (joined_buffer);
}

static char	*read_heredoc_content(char *delimiter)
{
	char	*buffer;
	char	*new_buffer;

	buffer = ft_strdup("");
	if (!buffer)
		return (NULL);
	while (1)
	{
		new_buffer = read_heredoc_line(delimiter, buffer);
		if (!new_buffer)
			return (NULL);
		if (new_buffer == buffer)
			break ;
		buffer = new_buffer;
	}
	return (buffer);
}

static void	heredoc_child(char *delimiter, int write_fd, t_cleanup *cleanup)
{
	char	*content;

	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	content = read_heredoc_content(delimiter);
	if (!content || g_exit_status == 130)
	{
		close(write_fd);
		rl_clear_history();
		cleanup_and_exit(cleanup, 130);
	}
	if (write(write_fd, content, ft_strlen(content)) == -1)
		perror("minishell: write");
	free(content);
	close(write_fd);
	rl_clear_history();
	cleanup_and_exit(cleanup, 0);
}

static int	fork_heredoc_child(t_token *token, int *pipefd, t_cleanup *cleanup)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	if (pid == 0)
	{
		close(pipefd[0]);
		heredoc_child(token->next->str, pipefd[1], cleanup);
	}
	return (pid);
}

int	handle_single_heredoc(t_token *token, t_cleanup *cleanup)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	token->heredoc_fd = -1;
	token->heredoc_content = NULL;
	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), -1);
	signal(SIGINT, SIG_IGN);
	pid = fork_heredoc_child(token, pipefd, cleanup);
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), -1);
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, handler_signal);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		return (close(pipefd[0]), (g_exit_status = 130), -1);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	if (g_exit_status == 130)
		return (close(pipefd[0]), -1);
	token->heredoc_fd = pipefd[0];
	return (0);
}
