# Bug Fixes

## 1. `srcs/exec/exec_pipeline.c` — Builtin error fell through to `execve`

**Problem:** `execute_builtin_simple` returns `1` both for "not a builtin" and for a builtin error (e.g. `echo` writes nothing and returns 1). The old code only exited the child if the return was `0`, so a failing builtin would fall through and try to `execve("echo", ...)` as an external command.

**Before:**
```c
if (execute_builtin_simple(cmd, envp) == 0)
    cleanup_and_exit(cleanup, 0);
```

**After:**
```c
if (is_simple_builtin(cmd->argv[0]))
    cleanup_and_exit(cleanup, execute_builtin_simple(cmd, envp));
```

---

## 2. `srcs/exec/exec_redir_utils.c` — `exit(1)` in parent process + hardcoded errno message

**Problem (a):** `print_redir_error` called `exit(1)` directly. Because `apply_redirections` is called **in the parent process** for simple builtins (`echo`, `pwd`, `env`), a redirection failure (e.g. `echo hi > /no/such/dir`) would terminate the entire shell instead of just returning an error.  The callers already call `cleanup_and_exit` right after, so the `exit(1)` was also redundant in child processes.

**Problem (b):** The error message was hardcoded as `"No such file or directory"` regardless of the actual `errno`, so `echo hi > /root/file` would print the wrong error (`No such file or directory` instead of `Permission denied`).

**Before:**
```c
ft_putstr_fd("No such file or directory\n", 2);
exit(1);
```

**After:**
```c
ft_putstr_fd(strerror(errno), 2);
ft_putstr_fd("\n", 2);
```

---

## 3. `srcs/exec/exec_command.c` — `stdin` not restored after simple builtin redirection

**Problem:** When running a simple builtin (echo/pwd/env) with a `<` redirection in the parent process, only `stdout` was saved and restored. `stdin` was permanently changed for the entire shell, breaking all subsequent commands that read from stdin.

**Before:**
```c
int saved_stdout;
int ret;
...
saved_stdout = dup(STDOUT_FILENO);
apply_redirections(cmd->redirections, cleanup);
ret = execute_builtin_simple(cmd, envp);
dup2(saved_stdout, STDOUT_FILENO);
close(saved_stdout);
```

**After:**
```c
int saved_stdout;
int saved_stdin;
int ret;
...
saved_stdout = dup(STDOUT_FILENO);
saved_stdin = dup(STDIN_FILENO);
apply_redirections(cmd->redirections, cleanup);
ret = execute_builtin_simple(cmd, envp);
dup2(saved_stdout, STDOUT_FILENO);
dup2(saved_stdin, STDIN_FILENO);
close(saved_stdout);
close(saved_stdin);
```

---

## 4. `srcs/exec/exec.c` — Dead code (unreachable null check)

**Problem:** A second `!node` check appeared directly after a `node == NULL` check. The second branch could never be reached and returned a different value (`0` vs `-1`), which was confusing.

**Before:**
```c
if (node == NULL)
    return (-1);
if (!node)
    return (0);
```

**After:**
```c
if (node == NULL)
    return (-1);
```

---

## 5. `srcs/builtins/builtins_utils.c` — Memory leak in `env_add` on allocation failure

**Problem (a):** On `malloc` failure, `free(new_envp)` was called on a `NULL` pointer — harmless but pointless.

**Problem (b):** After the `ft_strdup` loop fills `new_envp[0..count-1]`, if `ft_strjoin3` then fails, only `new_envp` itself was freed — all the `ft_strdup`'d strings inside it were leaked.

**Before:**
```c
if (!new_envp)
    return (free(new_envp), 1);
// ...
if (!new_var)
    return (free(new_envp), 1);
```

**After:**
```c
if (!new_envp)
    return (1);
// ...
if (!new_var)
{
    while (i-- > 0)
        free(new_envp[i]);
    return (free(new_envp), 1);
}
```

---

## 6. `srcs/heredoc/heredoc.c` — Typo in syntax error message

**Problem:** The error message printed when a heredoc had no delimiter was missing `near` and a space before the quote. Evaluators and testers often compare shell output exactly against bash.

**Before:**
```
minishell: syntax error unexpected token'newline'
```

**After:**
```
minishell: syntax error near unexpected token 'newline'
```
