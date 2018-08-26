/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 10:31:07 by sbrucker          #+#    #+#             */
/*   Updated: 2018/08/26 17:01:03 by sebastien        ###   ########.fr       */
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
** next: ptr to next lexeme
*/

typedef struct			s_lexeme
{
	size_t				type;
	size_t				type_details;
	void				*data;
	struct s_lexeme		*next;
}						t_lexeme;

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

#endif
