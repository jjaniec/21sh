/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 10:31:07 by sbrucker          #+#    #+#             */
/*   Updated: 2018/09/25 21:14:27 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <twenty_one_sh.h>

/*
** Struct in linked list to store type of lexeme and data pointer
** type: type of lexeme (T_(WORD/CTRL_OPT/REDIR_OPT))
** type_details: token associated with data (TK_(LESS/GREAT/...))
** data: pointer to char * from input to data of element
** elem_(begin/end)_ptr: pointer to beginning and end of lexeme in input string
** next: ptr to next lexeme
*/

typedef struct			s_lexeme
{
	size_t				type;
	size_t				type_details;
	void				*data;
	char				*lexeme_begin_ptr;
	char				*lexeme_end_ptr;
	struct s_lexeme		*next;
}						t_lexeme;

typedef struct			s_ast
{
	size_t			type;
	size_t			type_details;
	char			**data;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*parent;
}						t_ast;

/*
** int	ret: the return value of the last command. Default: 0
** int	ready_for_exec: if set to one, next execve() will be bypassed Default: 0
** char	**envp: the environmental var. Default: arg of the main()
** char	**tmp_envp: environmental var if modified temporarily
**			(T_ASSIGN_ENVIRONEMENT for instance). Default: NULL
*/

typedef struct			s_exec
{
	int		ret;
	int		ready_for_exec;
	char	**envp;
	char	**tmp_envp;
}						t_exec;

/*
** Option typedef:
** an option should be composed of:
** the name of the option: ex: (-)"G" / (--)"--color"
** the description of the option: ex: "Enables colorized output"
** a bool storing its status, initialized with its default value
*/

typedef struct			s_option
{
	char	*opt_name[MAX_OPT_NAMES];
	char	*opt_desc;
	bool	opt_status;
}						t_option;

/*
** Each environnement entry will be stored in a struct with a
** pointer to it's value
** entry: Key/Value pair for environnement entry,
**   like "OLDPWD=something"
** val_begin_ptr: pointer to beginning of value in our key/value pair
**   example, for "OLDPWD=something", val_begin_ptr will point to
**   "something", it allows us not to look for the '=' char
**   everytime we the value of
**   the variable
** ptr_to_pos_in_environ_tab: pointer to t_environ->environ entry position.
**   for not having to search through all variables
**   when we want to delete the entry
*/

typedef struct		s_env_entry
{
	char				entry[MAX_ENV_ENTRY_LEN + 1];
	char				*val_begin_ptr;
	char				**ptr_to_pos_in_environ_tab;
	struct s_env_entry	*next;
}					t_env_entry;

/*
** Environnement struct
** environ: environnement to pass to programs
** last_used_elem: last environnement entry requested / added / updated,
**   set to NULL when calling (del_var) and en entry was deleted
*/

typedef struct		s_environ
{
	char			*environ[MAX_ENV_ENTRIES + 1];
	t_env_entry		*last_used_elem;
	t_env_entry		*last_entry_ptr;
	t_env_entry		*env_entries_list;
	int				entry_count;
	char			*(*add_var)(struct s_environ *, char *, char *);
	char			*(*upt_var)(struct s_environ *, char *, char *);
	int				(*del_var)(struct s_environ *, char *);
	char			*(*get_var)();
}					t_environ;

#endif
