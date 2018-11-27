/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forty_two_sh.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 16:15:27 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/21 12:52:19 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FORTY_TWO_SH_H
# define FORTY_TWO_SH_H

# include <stddef.h>
# include <stdlib.h>
# include <stdint.h>
# include <inttypes.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <curses.h>
# include <term.h>
# include <time.h>
# include <termios.h>
# include <stdbool.h>
# include <dirent.h>

# include <errno.h>

# ifdef __linux__
	// Some linux specific tasks here
#  define _OS_ "Linux"
# endif
# ifdef __APPLE__
#  	// Some macos specific tasks here
#  define _OS_ "Darwin"
# endif
# ifndef _OS_
#  define _OS_ "?"
# endif

# define SH_NAME		"42sh"
# define IFS			"\t\n "
# define MALLOC_ERROR 	(EXIT_FAILURE)
# define OPT_NOT_FOUND_ERROR 1
# define ENABLE_JOB_CONTROL false // Not done yet

# ifndef VERBOSE_MODE
#  define VERBOSE_MODE 0
# endif

# define MAX_OPT_NAMES	2
# define CHAR_OPT_INDEX_SIZE (126)

# define MAX_ENV_ENTRIES	255
# define MAX_ENV_ENTRY_LEN	2048

# include <ft_printf.h>
# include "struct.h"
# include "lexer.h"
# include "log.h"
# include "ast.h"
# include "line_edition.h"
# include "exec.h"
# include "syntax_highlighting.h"
# include "signals.h"
# include "script.h"
# include "autocomplete.h"
# include "builtins.h"
# include "hash_table.h"
# include "prompt_details.h"

# define HISTORY_FILE_PATH "$HOME/.42sh_history"
# define ALIASES_FILE_PATH "$HOME/.42sh_aliases"

# define ERR_NO_ENTRY		"no such file or directory: "
# define ERR_ISDIR			"is a directory\n"
# define ERR_NORIGHTS		"permission denied: "
# define ERR_CMD_NOT_FOUND	"command not found\n"
# define ERR_INVALID_ALIAS_NAME "alias: invalid alias name\n"
# define ERR_AMBIGUOUS_REDIR	"ambiguous redirect\n"
# define ERR_BAD_FILEDESC		"Bad file descriptor\n"
# define ERR_ENOTDIR			"not a directory\n"
# define ERR_EACCESS			"permission denied\n"

extern t_option		g_sh_opts[];
extern const int	g_cant_begin_with[];
extern const int	g_token_bypass[];
extern const int	g_tokens[];
extern const int	g_next_tokens[][4];

extern void			(* const g_node_placer[])(t_ast *, t_ast *);
extern const char	*g_prompts[10];

extern t_job		*g_jobs;

# define SH_USAGE \
	SH_NAME" [-hcGv] [-c \"command\"]"

# define NOT_A_TTY_STDINOUT_ERR \
	SH_NAME": Input/output redirections are not supported, exiting\n"

typedef int	t_acocat;

int			get_next_line(const int fd, char **line);

int			prompt_show(const char *prompt);
char		*get_valid_input(t_lexeme **lexemes, int sub_prompts);

int			subpp_string(char **s);
t_lexeme	*subp_lexeme(t_lexeme *lex, int need_sub_prompt);
int			subp_heredoc(t_lexeme *lex, char *eof_word);

void		init_option_list(t_option **opt_tab, ...);

char		**parse_options(int *ac, char **av, \
				t_option *opt_list, t_option **char_opt_index);

void		format_help(char *usage_str, t_option *opts);

t_option	*get_opt_elem(t_option *opt_list, char *opt_str);

bool		is_option_activated(char *opt_str, \
				t_option *opt_list, t_option **char_opt_index);

char		*add_env_var(t_environ *self, char *name, char *entry_value);

int			del_env_var(struct s_environ *self, char *varname);

t_env_entry	*get_env_var(t_environ *self, char *varname);

char	*upd_env_var(t_environ *this, char *name, char *new_value);

t_environ	*init_environ(char **env, t_environ *env_struct);

void	free_env_entries(t_environ *env_struct, t_env_entry *env_entries);

void	free_all_shell_datas(void);

void		init_environ_struct_ptrs(t_environ *env_struct);

char	autoc_check_path(char *dirname);

bool		check_backup_file(const char *file_path);

void	load_history_file(struct s_line *le);
void	load_aliases_file(struct s_alias *alias);

char	*get_parsed_aliases_file_path(void);

char	*get_parsed_history_file_path(void);

char	*ft_strjoin_path(char *path1, char *path2);

int		is_identifier_invalid(char *str, char *assign_ptr);

t_shell_vars	*get_shell_vars(void);

t_lexeme	*handle_exclamation_mark_in_lexer(t_lexeme *lex);

const char      *parse_exclamation_mark_shortcuts(const char *excla);

struct s_alias	*access_alias_datas(void);
void		aliases_replace(t_lexeme **lex);
struct s_alias		*is_an_alias(const char *data, \
					const struct s_alias *iterator);

void		log_close(int fd);

char		**ft_dup_2d_array(char **arr);

char		**handle_env_assigns(t_ast *node, t_exec *exe, t_environ **env_used);

#endif
