# Fixes for bugs #1, #4, #5 (norminette-compliant)

These three bugs travel together because #4/#5 require `apply_redirections`
to *signal* failure instead of `exit()`-ing, and #1 needs that same
return-value plumbing once heredoc content is delivered through
`redir->heredoc_content` (the field the parser and `apply_heredoc` already
expect, but which was never populated).

## Files modified (8)

1. `includes/minishell.h`
2. `srcs/exec/exec_redir_utils.c`
3. `srcs/exec/exec_redir.c`
4. `srcs/exec/exec_command.c`
5. `srcs/exec/exec_pipeline.c`
6. `srcs/builtins/builtins.c`
7. `srcs/heredoc/heredoc.c`
8. `srcs/heredoc/handle_single_heredoc.c`

No new `.c` files — **`Makefile` is unchanged**. Function-per-file caps
after the changes (limit 5):

| file | before | after |
|---|---|---|
| `heredoc.c` | 2 | 3 |
| `builtins.c` | 4 | 5 |
| `exec_redir.c` | 1 | 1 |
| `exec_redir_utils.c` | 5 | 5 |
| `exec_command.c` | 5 | 5 |
| `exec_pipeline.c` | 4 | 4 |
| `handle_single_heredoc.c` | 5 | 5 |

All function bodies stay ≤ 25 lines; `run_builtin_with_redir` uses 4
arguments (the limit).

---

## Each modified file, in full

Drop these in verbatim — every block below is the complete new file.

### 1. `includes/minishell.h`

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:50:03 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 00:00:00 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <libft.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>

extern int				g_exit_status;

typedef enum e_quote_type
{
	NO_QUOTE,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
}						t_quote_type;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_ERROR,
}						t_token_type;

typedef struct s_token
{
	t_token_type		type;
	t_quote_type		quote;
	char				*str;
	char				*heredoc_content;
	int					heredoc_fd;
	struct s_token		*next;
}						t_token;

typedef enum e_redir_type
{
	REDIR_NONE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
}						t_redir_type;

typedef struct s_redir
{
	t_redir_type		type;
	char				*file;
	char				*heredoc_content;
	int					heredoc_fd;
	struct s_redir		*next;
}						t_redir;
typedef struct s_command
{
	char				**argv;
	t_redir				*redirections;
}						t_command;
typedef enum e_node_type
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
}						t_node_type;

typedef struct s_node
{
	t_node_type			type;
	t_command			*command;
	struct s_node		*left;
	struct s_node		*right;
}						t_node;
typedef enum e_operator_prio
{
	LOW_PRIO,
	MEDIUM_PRIO,
}						t_operator_prio;

typedef struct s_operator_info
{
	int					position;
	t_token_type		type;
}						t_operator_info;
typedef struct s_pipeline
{
	t_command			*cmd;
	struct s_pipeline	*next;
}						t_pipeline;
typedef struct s_cleanup
{
	char				*line;
	t_token				*tokens;
	t_pipeline			*pipeline;
	t_pipeline			*head_pipeline;
	t_node				*ast;
	int					last_status;
	char				**envp;
}						t_cleanup;

typedef struct s_expand_tokens
{
	char				*str;
	char				*result;
	char				**envp;
}						t_expand_tokens;

t_token					*lexer(char *line);
t_token					*create_token(t_token_type type, t_quote_type quote,
							char *str, int len);
void					token_add_back(t_token **lst, t_token *new);
void					free_tokens(t_token *tokens);
int						handle_operator(char *line, int *i, t_token **tokens);
int						handle_word(char *line, int *i, t_token **tokens);
int						handle_quotes(char *line, int *i, t_token **tokens);
void					skip_spaces(char *line, int *i);
t_token_type			get_operator_type(char *str);
t_quote_type			get_quote_type(char *line, int *i, char *quote_char);
int						process_heredoc(t_token *tokens, t_cleanup *cleanup);
void					expander(t_token *tokens, char **envp);
char					*expand(t_expand_tokens *tokens);
int						get_var_len(char *str, int *i, char **envp);
int						expanded_len(char *str, char **envp);
int						copy_var_value(t_expand_tokens *tokens, int *i, int k);
char					*copy_expanded_str(t_expand_tokens *tokens);
char					*ft_getenv(char *name, char **envp);
int						is_valid_var_char(char c);
t_node					*create_node(t_node_type type, t_command *command);
int						count_nodes(t_node *node);
void					free_ast(t_node *node);
void					free_redirections(t_redir *redirections);
t_node					*parse(t_token *tokens, int length);
t_command				*parse_command(t_token *tokens, int length);
char					**create_argv(t_token *tokens, int count, int length);
t_redir					*parse_redirections(t_token *tokens, int length);
void					redir_add_back(t_redir **lst, t_redir *new);
void					print_unexpected_token(void);
int						print_parser_redir_error(t_token *token);

t_operator_info			find_operator(t_token *tokens, int length,
							t_operator_prio prio);
int						has_wrapping_parentheses(t_token *tokens, int length);
int						is_target_operator(t_token_type type,
							t_operator_prio prio);
t_node					*handle_operator_parser(t_token *tokens, int length,
							t_operator_info operator_info);
t_node					*create_command_node(t_token *tokens, int length);
void					print_syntax_error(char *token);
int						ft_tokens_size(t_token *lst);
int						count_tokens_word(t_token *token, int length);
t_redir_type			token_to_redir_type(t_token_type type);
t_token					*advance_token(t_token *token, int position);
int						exec_ast(t_node *node, char ***envp,
							t_cleanup *cleanup);
int						exec_pipeline(t_node *node, char ***envp,
							t_cleanup *cleanup);
int						exec_command(t_command *cmd, char ***envp,
							t_cleanup *cleanup);
int						exec_or(t_node *node, char ***envp, t_cleanup *cleanup);
int						exec_and(t_node *node, char ***envp,
							t_cleanup *cleanup);
char					*get_path(char *cmd, char **envp);
char					*find_path_in_env(char **envp);
char					*try_path(char **paths, char *cmd);
int						apply_redirections(t_redir *redir, t_cleanup *cleanup);
void					print_redir_error(char *file);
int						apply_in(t_redir *redir, t_cleanup *cleanup);
int						apply_out(t_redir *redir, t_cleanup *cleanup);
int						apply_append(t_redir *redir, t_cleanup *cleanup);
int						apply_heredoc(t_redir *redir, t_cleanup *cleanup);
char					*read_pipe_content(int fd);
int						run_builtin_with_redir(t_command *cmd, char ***envp,
							t_cleanup *cleanup, int is_parent);
void					print_command_error(char *cmd, int error_type);
int						get_exit_code(int status);
int						builtin_cd(t_command *cmd, char ***envp);
char					*ft_strjoin3(char *a, char *b, char *c);
int						is_parent_builtin(const char *cmd);
int						execute_builtin_simple(t_command *cmd, char ***envp);
int						builtin_echo(t_command *cmd);
int						builtin_pwd(t_command *cmd);
int						builtin_env(t_command *cmd, char **envp);
int						builtin_export(t_command *cmd, char ***env);
int						builtin_unset(t_command *cmd, char ***env);
int						builtin_exit(t_command *cmd,
							t_cleanup *cleanup);
int						is_numeric_exit(const char *str);
int						get_exit_value(const char *str);
void					cleanup_and_exit(t_cleanup *cleanup, int status);
int						is_valid_env(const char *name);
char					*ft_strndup(const char *s, int maxlen);
int						handle_export_arg(char *arg, char ***env);
int						execute_builtin_parent(t_command *cmd, char ***envp,
							t_cleanup *cleanup);
void					sort_env(char **env);
int						update_env_forcd(char ***envp, char *name, char *value);
int						env_add(char ***envp, char *name, char *value);
int						update_or_add_env(char ***envp, char *name,
							char *value);
t_pipeline				*extract_cmd(t_node *node);
char					*remove_quote(char *str);
void					handler_signal(int sig);
void					handle_status(int status);
void					wait_all(pid_t g_signal);
int						is_redirection(t_token *token);
char					**copy_envp(char **envp);
void					free_envp(char **envp);
void					free_pipeline(t_pipeline *p);
int						is_simple_builtin(const char *cmd);
void					setup_signals(void);
void					setup_child_pipe(int prev_fd, int *pipefd,
							int has_next);
void					retry_as_shell_script(char *path, char **argv,
							char **envp);
void					expand_wildcard_token(t_token *token);
int						handle_single_heredoc(t_token *token,
							t_cleanup *cleanup);
void					init_cleanup(t_cleanup *cleanup);
void					cleanup_iteration(t_cleanup *cleanup);
void	heredoc_sigint(int sig);

#endif
```

### 2. `srcs/exec/exec_redir_utils.c`

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:05:16 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 00:00:00 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_redir_error(char *file)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd("No such file or directory\n", 2);
}

int	apply_in(t_redir *redir, t_cleanup *cleanup)
{
	int	fd_in;

	(void)cleanup;
	fd_in = open(redir->file, O_RDONLY);
	if (fd_in == -1)
		return (print_redir_error(redir->file), -1);
	if (dup2(fd_in, STDIN_FILENO) == -1)
		return (perror("minishell: dup2"), close(fd_in), -1);
	close(fd_in);
	return (0);
}

int	apply_out(t_redir *redir, t_cleanup *cleanup)
{
	int	fd_out;

	(void)cleanup;
	fd_out = open(redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd_out == -1)
		return (print_redir_error(redir->file), -1);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		return (perror("minishell: dup2"), close(fd_out), -1);
	close(fd_out);
	return (0);
}

int	apply_append(t_redir *redir, t_cleanup *cleanup)
{
	int	fd_apnd;

	(void)cleanup;
	fd_apnd = open(redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd_apnd == -1)
		return (print_redir_error(redir->file), -1);
	if (dup2(fd_apnd, STDOUT_FILENO) == -1)
		return (perror("minishell: dup2"), close(fd_apnd), -1);
	close(fd_apnd);
	return (0);
}

int	apply_heredoc(t_redir *redir, t_cleanup *cleanup)
{
	int	pipe_fd[2];

	(void)cleanup;
	if (!redir->heredoc_content)
		return (-1);
	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	if (write(pipe_fd[1], redir->heredoc_content,
			ft_strlen(redir->heredoc_content)) == -1)
		return (perror("write"), close(pipe_fd[0]), close(pipe_fd[1]), -1);
	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		return (perror("dup2"), close(pipe_fd[0]), -1);
	close(pipe_fd[0]);
	free(redir->heredoc_content);
	redir->heredoc_content = NULL;
	return (0);
}
```

### 3. `srcs/exec/exec_redir.c`

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:58:04 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 00:00:00 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_redirections(t_redir *redir, t_cleanup *cleanup)
{
	t_redir	*current;
	int		ret;

	current = redir;
	while (current)
	{
		ret = 0;
		if (current->type == REDIR_IN)
			ret = apply_in(current, cleanup);
		else if (current->type == REDIR_OUT)
			ret = apply_out(current, cleanup);
		else if (current->type == REDIR_APPEND)
			ret = apply_append(current, cleanup);
		else if (current->type == REDIR_HEREDOC)
			ret = apply_heredoc(current, cleanup);
		if (ret == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}
```

### 4. `srcs/exec/exec_command.c`

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 17:29:02 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 00:00:00 by reratsam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	char	*path;
	int		err;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirections(cmd->redirections, cleanup) == -1)
		cleanup_and_exit(cleanup, 1);
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
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	if (is_parent_builtin(cmd->argv[0]))
		return (run_builtin_with_redir(cmd, envp, cleanup, 1));
	if (is_simple_builtin(cmd->argv[0]))
		return (run_builtin_with_redir(cmd, envp, cleanup, 0));
	return (fork_and_wait(cmd, envp, cleanup));
}
```

### 5. `srcs/exec/exec_pipeline.c`

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:57:07 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 00:00:00 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_pipeline_cmd(t_command *cmd, t_cleanup *cleanup,
		char ***envp)
{
	char	*path;
	char	**env;

	if (!cmd)
		return ;
	if (apply_redirections(cmd->redirections, cleanup) == -1)
		cleanup_and_exit(cleanup, 1);
	if (execute_builtin_simple(cmd, envp) == 0)
		cleanup_and_exit(cleanup, 0);
	env = *envp;
	path = get_path(cmd->argv[0], env);
	if (!path)
	{
		print_command_error(cmd->argv[0], 127);
		cleanup_and_exit(cleanup, 127);
	}
	execve(path, cmd->argv, env);
	if (errno == ENOEXEC)
		retry_as_shell_script(path, cmd->argv, env);
	print_command_error(cmd->argv[0], 126);
	cleanup_and_exit(cleanup, 126);
}

static pid_t	fork_pipeline_child(t_cleanup *cleanup, int prev_fd,
		int *pipefd, char ***envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		setup_child_pipe(prev_fd, pipefd, cleanup->pipeline->next != NULL);
		execute_pipeline_cmd(cleanup->pipeline->cmd, cleanup, envp);
	}
	if (cleanup->pipeline->next && pipefd)
		close(pipefd[1]);
	if (prev_fd != -1)
		close(prev_fd);
	return (pid);
}

static void	run_pipeline_loop(t_cleanup *cleanup, int *prev_fd, char ***envp)
{
	int		pipefd[2];
	pid_t	pid;

	*prev_fd = -1;
	while (cleanup->pipeline)
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
		if (cleanup->pipeline->next && pipe(pipefd) == -1)
			(perror("pipe"), cleanup_and_exit(cleanup, 1));
		pid = fork_pipeline_child(cleanup, *prev_fd, pipefd, envp);
		if (pid == -1)
			cleanup_and_exit(cleanup, 1);
		if (cleanup->pipeline->next)
			*prev_fd = pipefd[0];
		else
			*prev_fd = -1;
		cleanup->pipeline = cleanup->pipeline->next;
	}
	wait_all(pid);
}

int	exec_pipeline(t_node *node, char ***envp, t_cleanup *cleanup)
{
	int	prev_fd;

	cleanup->pipeline = extract_cmd(node);
	cleanup->head_pipeline = cleanup->pipeline;
	run_pipeline_loop(cleanup, &prev_fd, envp);
	if (prev_fd != -1)
		close(prev_fd);
	free_pipeline(cleanup->head_pipeline);
	cleanup->pipeline = NULL;
	return (g_exit_status);
}
```

### 6. `srcs/builtins/builtins.c`

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 15:19:13 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 00:00:00 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_parent_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin_simple(t_command *cmd, char ***envp)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (builtin_echo(cmd));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd(cmd));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (builtin_env(cmd, *envp));
	return (1);
}

int	execute_builtin_parent(t_command *cmd, char ***envp, t_cleanup *cleanup)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (-1);
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd, envp));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd, envp));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd, envp));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd, cleanup));
	return (-1);
}

int	is_simple_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	return (0);
}

int	run_builtin_with_redir(t_command *cmd, char ***envp,
		t_cleanup *cleanup, int is_parent)
{
	int	saved[3];
	int	ret;

	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	saved[2] = dup(STDERR_FILENO);
	if (apply_redirections(cmd->redirections, cleanup) == -1)
		ret = 1;
	else if (is_parent)
		ret = execute_builtin_parent(cmd, envp, cleanup);
	else
		ret = execute_builtin_simple(cmd, envp);
	dup2(saved[0], STDIN_FILENO);
	dup2(saved[1], STDOUT_FILENO);
	dup2(saved[2], STDERR_FILENO);
	close(saved[0]);
	close(saved[1]);
	close(saved[2]);
	return (ret);
}
```

> Note: `builtin_exit` calls `cleanup_and_exit`, which `exit()`s before the
> restore lines run. That's fine — the process is going away. The saved fds
> are leaked-on-exit, the same outcome as before.

### 7. `srcs/heredoc/heredoc.c`

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gechavia <gechavia@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/31 14:54:14 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 00:00:00 by gechavia         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_exit_status = 130;
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	process_heredoc(t_token *tokens, t_cleanup *cleanup)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_REDIR_HEREDOC)
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				if (current->next)
					fprintf(stderr,
						"minishell: syntax error near unexpected token '%s'\n",
						current->next->str);
				else
					fprintf(stderr,
						"minishell: syntax error unexpected token'newline'\n");
				return (-1);
			}
			if (handle_single_heredoc(current, cleanup) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

char	*read_pipe_content(int fd)
{
	char	*result;
	char	*new;
	char	buf[1024];
	int		n;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	n = read(fd, buf, sizeof(buf) - 1);
	while (n > 0)
	{
		buf[n] = '\0';
		new = ft_strjoin(result, buf);
		free(result);
		if (!new)
			return (NULL);
		result = new;
		n = read(fd, buf, sizeof(buf) - 1);
	}
	return (result);
}
```

### 8. `srcs/heredoc/handle_single_heredoc.c`

The four static helpers (`read_heredoc_line`, `read_heredoc_content`,
`heredoc_child`, `fork_heredoc_child`) are unchanged. Only
`handle_single_heredoc` is rewritten — it now drains the pipe into
`token->heredoc_content` instead of stashing the read-end fd.

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 12:28:24 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 00:00:00 by reratsam         ###   ########.fr       */
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
	token->heredoc_content = read_pipe_content(pipefd[0]);
	close(pipefd[0]);
	waitpid(pid, &status, 0);
	signal(SIGINT, handler_signal);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	if (g_exit_status == 130 || !token->heredoc_content)
		return (free(token->heredoc_content),
			(token->heredoc_content = NULL), -1);
	return (0);
}
```

---

## Why these three together

- **#1 (heredoc delivery):** the parser already copies
  `current->heredoc_content` into `redir->heredoc_content`, and
  `apply_heredoc` already pipes that string back into stdin. The only
  missing piece was actually *populating* `token->heredoc_content`.
  `handle_single_heredoc` now does that (by draining the pipe the child
  writes to). Bonus side effect: the existing `expand_heredoc_content`
  in the expander finally sees a non-NULL content, so `$VAR` inside an
  unquoted heredoc body is now expanded. Also, large heredocs (> 64 KB
  pipe buffer) no longer deadlock, because the parent drains while the
  child is still writing.

- **#4 (builtin redirection killing the shell):** `print_redir_error`
  no longer calls `exit`. The `apply_*` functions now return `-1` on
  failure, `apply_redirections` propagates that, and the builtin path
  treats it as "ret = 1, skip the builtin." Forked callers
  (`exec_child`, `execute_pipeline_cmd`) explicitly call
  `cleanup_and_exit(cleanup, 1)` on `-1`, preserving prior behavior in
  the child path.

- **#5 (only stdout was saved/restored):** `run_builtin_with_redir`
  saves and restores **stdin, stdout, and stderr** with `dup`/`dup2`
  around the builtin. This applies to both parent builtins (`cd`,
  `export`, `unset`, `exit`) and simple builtins (`echo`, `pwd`,
  `env`).

## Quick verification after applying

```sh
make re && ./minishell
```

```sh
# #1 — heredoc actually delivers the body
$ cat <<EOF
hello
world
EOF
hello
world

# bonus — $VAR expansion in unquoted heredoc bodies
$ X=42
$ cat <<EOF
x is $X
EOF
x is 42

# #4 — failed builtin redirection no longer kills the shell
$ echo hi < /no/such/file
minishell: /no/such/file: No such file or directory
$ echo still alive
still alive

# #5 — stdin/stderr restored after a redirected builtin
$ echo first < /etc/hostname     # stdin redirected just for this builtin
$ cat                            # stdin should be the terminal again
hello
hello
^D
```

## Out of scope (not fixed here)

- **Bug #2 (heredoc expansion):** partially addressed as a side effect —
  unquoted-delimiter heredocs now expand because `heredoc_content` is
  finally non-NULL. Properly *skipping* expansion when the delimiter is
  *quoted* still requires tracking that flag on the heredoc token, which
  this patch does not add.
- **Bug #3 (quoted heredoc delimiter hangs):** the lexer leaves the
  quote chars in `token->next->str`, so user input never matches the
  delimiter. `remove_quote` would have to be applied to the delimiter
  passed to `heredoc_child` (and ideally combined with a "do not expand"
  flag for #2). Out of scope for this patch.
