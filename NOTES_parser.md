# Parser — study notes

Built up in order with Remy, 2026-04-23.

## 0. What the parser receives and produces

**Input:** a linked list of `t_token` (from the lexer) plus the length (`int`) of that list.

Each `t_token` (see `includes/minishell.h`) carries:

- `type` — one of `TOKEN_WORD`, `TOKEN_PIPE`, `TOKEN_REDIR_IN/OUT/APPEND/HEREDOC`, `TOKEN_OR`, `TOKEN_AND`, `TOKEN_LPAREN`, `TOKEN_RPAREN`, `TOKEN_ERROR`
- `quote` — `NO_QUOTE`, `SINGLE_QUOTE`, `DOUBLE_QUOTE`
- `str` — the raw text (e.g. `"echo"`, `"hello"`, `">"`)
- `heredoc_content`, `heredoc_fd` — filled in if this was a heredoc
- `next` — linked-list pointer

**Output:** an AST (`t_node *`) where every node is either:

- a leaf: `NODE_COMMAND` → holds a `t_command *` (an `argv[]` + a linked list of `t_redir`)
- an internal: `NODE_PIPE`, `NODE_AND`, `NODE_OR` → has `left` and `right` subtrees

The AST captures bash operator precedence. Lowest precedence becomes the *root* of the tree so that exec evaluates it last. Bash precedence (low → high):

1. `||` and `&&` (left-associative, equal precedence) — LOW_PRIO
2. `|`                                               — MEDIUM_PRIO
3. command with its redirections                    — leaf

Parens `( ... )` force grouping regardless of precedence.

Example: `echo hi > out | grep a && ls` becomes:

```
          AND
         /   \
      PIPE    ls
     /    \
  echo hi>out  grep a
```

`&&` is the lowest-precedence operator in this input → becomes the root. Then on the left side, `|` splits `echo hi > out` and `grep a`. On the right side, `ls` is a plain command.

---

## 1. `parser.c` — entry point `parse()`

Only one function. It is a **recursive descent parser** using operator-precedence splitting.

**Algorithm:**

1. If slice is empty → syntax error.
2. Scan for the **last** `||`/`&&` at paren-depth 0 (lowest precedence). Found? split + recurse.
3. Else scan for the last `|` at paren-depth 0. Found? split + recurse.
4. Else if the slice is fully wrapped in `( ... )` → strip parens + recurse.
5. Else if it starts with `(` but isn't wrapped → unmatched paren error.
6. Else → build a `NODE_COMMAND` leaf via `create_command_node`.

**Why "last" operator, not first?**
Because `&&`/`||` are left-associative. In `a && b && c`, the *rightmost* `&&` must be at the root so the leftmost one lives deeper and is evaluated first.

**Why lowest-precedence first?**
The operator farthest from the leaves = highest up in the tree. Lowest precedence = evaluated last = root of the subtree.

**Paren-depth tracking:**
`find_operator` counts `(` and `)` to a `count` variable. It only accepts an operator when `count == 0`, i.e. not inside nested parens. This is how `(a || b) | c` correctly picks `|` as the root, not `||`.

---

