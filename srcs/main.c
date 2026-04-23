/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:49:44 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/20 15:05:46 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit_status = 0;

static int	init_shell(char **envp, char ***my_envp, t_cleanup *cleanup)
{
	init_cleanup(cleanup);
	*my_envp = copy_envp(envp);
	if (!*my_envp)
		return (1);
	setup_signals();
	return (0);
}

static int	handle_empty_line(char *line)
{
	if (line == NULL)
	{
		printf("exit\n");
		clear_history();
		return (1);
	}
	if (line[0] == '\0')
	{
		free(line);
		return (1);
	}
	return (0);
}

static int	process_input(char *line, char ***my_envp, t_cleanup *cleanup)
{
	t_token	*token;
	t_node	*node;

	token = NULL;
	node = NULL;
	token = lexer(line);
	if (!token)
		return (-1);
	cleanup->line = line;
	cleanup->tokens = token;
	cleanup->envp = *my_envp;
	if (process_heredoc(token, cleanup) == -1)
		return (free_tokens(token), -1);
	expander(token, *my_envp);
	node = parse(token, ft_tokens_size(token));
	if (!node)
		return (free_tokens(token), -1);
	cleanup->ast = node;
	g_exit_status = exec_ast(node, my_envp, cleanup);
	free_tokens(cleanup->tokens);
	cleanup->tokens = NULL;
	free_ast(cleanup->ast);
	cleanup->ast = NULL;
	return (0);
}

static void	shell_loop(char ***my_envp, t_cleanup *cleanup)
{
	char	*line;

	signal(SIGINT, handler_signal);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("minishell> ");
		if (handle_empty_line(line))
		{
			if (!line)
				break ;
			continue ;
		}
		add_history(line);
		process_input(line, my_envp, cleanup);
		free(line);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_cleanup	cleanup;
	char		**my_envp;

	(void)argc;
	(void)argv;
	if (init_shell(envp, &my_envp, &cleanup))
		return (1);
	shell_loop(&my_envp, &cleanup);
	free_envp(my_envp);
	return (0);
}
