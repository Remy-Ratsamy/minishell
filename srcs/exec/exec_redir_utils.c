/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:05:16 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/20 15:05:46 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_redir_error(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
}

void	apply_in(t_redir *redir, t_cleanup *cleanup)
{
	int	fd_in;

	fd_in = open(redir->file, O_RDONLY);
	if (fd_in == -1)
	{
		print_redir_error(redir->file);
		cleanup_and_exit(cleanup, 1);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		cleanup_and_exit(cleanup, 1);
	}
	close(fd_in);
}

void	apply_out(t_redir *redir, t_cleanup *cleanup)
{
	int	fd_out;

	fd_out = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd_out == -1)
	{
		print_redir_error(redir->file);
		cleanup_and_exit(cleanup, 1);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		cleanup_and_exit(cleanup, 1);
	}
	close(fd_out);
}

void	apply_append(t_redir *redir, t_cleanup *cleanup)
{
	int	fd_apnd;

	fd_apnd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd_apnd == -1)
	{
		print_redir_error(redir->file);
		cleanup_and_exit(cleanup, 1);
	}
	if (dup2(fd_apnd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		cleanup_and_exit(cleanup, 1);
	}
	close(fd_apnd);
}

void	apply_heredoc(t_redir *redir, t_cleanup *cleanup)
{
	int	pipe_fd[2];

	if (!redir->heredoc_content)
		cleanup_and_exit(cleanup, 1);
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		cleanup_and_exit(cleanup, 1);
	}
	if (write(pipe_fd[1], redir->heredoc_content,
			ft_strlen(redir->heredoc_content)) == -1)
	{
		perror("write");
		cleanup_and_exit(cleanup, 1);
	}
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		cleanup_and_exit(cleanup, 1);
	}
	close(pipe_fd[0]);
	free(redir->heredoc_content);
	redir->heredoc_content = NULL;
}
