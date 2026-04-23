*This project has been created as part of the 42 curriculum by gechavia.*

# Minishell

## Description

Minishell is a project from the 42 curriculum. The goal is to implement a functional Unix shell from scratch in C, capable of parsing and executing commands, handling redirections, pipes, environment variables, signals, and built-in commands — all while respecting the 42 norm and managing memory without leaks.

The shell follows a modular pipeline: **lexer** → **heredoc** → **expander** → **parser** → **executor**. Each stage transforms the user's input into an executable form, built around an Abstract Syntax Tree (AST) to handle operator precedence correctly.

## Instructions

### Requirements

- GCC or CC compiler
- GNU Make
- libreadline (`libreadline-dev` on Debian/Ubuntu)

### Compilation

```bash
make
```

### Run

```bash
./minishell
```

### Clean

```bash
make clean    # Remove object files
make fclean   # Remove object files and binary
make re       # Full recompile
```

### Valgrind (memory leak check)

```bash
valgrind --leak-check=full --show-leak-kinds=all \
  --track-origins=yes --suppressions=readline.supp ./minishell
```

## Features

- Interactive prompt with command history
- Single quotes: prevents all interpretation of metacharacters
- Double quotes: prevents interpretation except for `$`
- Environment variable expansion (`$VAR`, `$?`)
- Redirections: `<`, `>`, `>>`, `<<` (heredoc)
- Pipes (`|`) with correct chaining
- Logical operators: `&&`, `||` with proper precedence via AST
- Wildcard expansion (`*`) in the current directory
- Signal handling: `ctrl-C` (new prompt), `ctrl-D` (exit), `ctrl-\` (ignored)
- Built-in commands:
  - `echo` (with `-n` option)
  - `cd` (relative and absolute paths, `-`, `HOME`)
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit` (with optional exit code)

## Project Architecture

Each subdirectory under `srcs/` corresponds to one stage of the pipeline:

| Directory | Role |
|---|---|
| `srcs/lexer/` | Tokenizes raw input into a linked list of tokens |
| `srcs/heredoc/` | Reads heredoc content interactively before expansion |
| `srcs/expander/` | Expands environment variables and wildcards |
| `srcs/parser/` | Builds an AST from the token list |
| `srcs/ast/` | AST node creation and utilities |
| `srcs/exec/` | Walks the AST and executes commands |
| `srcs/builtins/` | Built-in command implementations |
| `srcs/` (root) | Entry point and shell loop |
| `libft/` | Custom C standard library |

## Resources

### References

- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — used as the reference implementation for all behavior
- [The Open Group Base Specifications — Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf)
- `man 3 readline`, `man 2 fork`, `man 2 execve`, `man 2 pipe`, `man 2 waitpid`, `man 7 signal`

### AI Usage

- Updating the README to match the required format
