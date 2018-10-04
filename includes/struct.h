/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 10:31:07 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/01 10:24:58 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include <forty_two_sh.h>

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
	int					pos;
	struct s_lexeme		*next;
}						t_lexeme;

/*
** Struct used by clean_word_lexeme()
** to clean a lexeme string of quotes / backslashes,
** and apply expansions.
** raw_lexeme_data: raw lexeme data (before cleaning it)
** new_data: new lexeme data, free of unnecessary quotes/backslashes, w/ expansions
** new_data_size: size of new_data pointer, required for knowing size of
**   new data string size when realloc'ing to store an expansion data
*/

typedef struct			s_lexeme_clean_data
{
	char				*raw_lexeme_data;
	char				**raw_lexeme_read_ptr;
	char				*clean_data;
	char				*clean_data_write_ptr;
	size_t				clean_data_size;
}						t_lexeme_clean_data;

typedef struct			s_ast
{
	size_t			type;
	size_t			type_details;
	char			**data;
	struct s_ast	*sub_ast;
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

typedef t_option		*t_opt_list;

#endif
