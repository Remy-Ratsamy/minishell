/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: reratsam <reratsam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 17:50:03 by gechavia          #+#    #+#             */
/*   Updated: 2026/04/28 19:18:01 by reratsam         ###   ########.fr       */
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
void					apply_redirections(t_redir *redir, t_cleanup *cleanup);
void					print_redir_error(char *file);
void					apply_in(t_redir *redir, t_cleanup *cleanup);
void					apply_out(t_redir *redir, t_cleanup *cleanup);
void					apply_append(t_redir *redir, t_cleanup *cleanup);
void					apply_heredoc(t_redir *redir, t_cleanup *cleanup);
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
void					heredoc_sigint(int sig);

#endif