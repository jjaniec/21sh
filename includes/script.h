/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebastien <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 21:01:05 by sebastien         #+#    #+#             */
/*   Updated: 2018/10/01 12:34:01 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCRIPT_H
# define SCRIPT_H
# include <forty_two_sh.h>

# define T_SCRIPT_LOGICAL 5

# define TK_SCRIPT 50
# define TK_SCRIPT_IF 51
# define TK_SCRIPT_ELIF 52
# define TK_SCRIPT_ELSE 53
# define TK_SCRIPT_WHILE 54


# define T_SCRIPT_CONTAINER 6

# define TK_SCRIPT_THEN 60
# define TK_SCRIPT_FI 61
# define TK_SCRIPT_DO 62
# define TK_SCRIPT_DONE 63


# define T_SCRIPT_CONDITION 7

# define TK_SCRIPT_CONDITION_IF 70
# define TK_SCRIPT_CONDITION_WHILE 71

/*
** SCRIPT LEXER
*/

void		script_lexemes(t_lexeme *lexemes);
t_lexeme	*is_keyword(t_lexeme *lex, int last_lex_lvl);
//t_lexeme	*is_keyword(t_lexeme *lex);


/*
** SCRIPT VALIDATION
*/

int			script_check_parsing(t_lexeme *lex1, t_lexeme *lex2);
t_lexeme	*is_script_complete(t_lexeme *lex, t_lexeme *tmp, size_t next_token);
int			good_start(t_lexeme *lex);


/*
** SCRIPT AST
*/

t_ast		*script_create_node(t_lexeme *lex);
t_lexeme	*script_put_node_ast(t_lexeme *lex, t_ast *root);
int			put_node(t_lexeme **lex, t_ast **root, t_ast *new, \
			void(* const node_placer)(t_ast *, t_ast *));
t_lexeme	*need_subast(t_lexeme *lex, t_ast **root, t_ast *new, \
			void (* const node_placer)(t_ast *, t_ast *));

t_lexeme	*find_end_lexeme(t_lexeme *start, const size_t end_token[]);
t_lexeme	*find_lexeme_compete(t_lexeme *lex, const size_t end_token[]);
t_lexeme	*find_end_lexeme_solo(t_lexeme *start, const size_t end_token);


/*
** SCRIPT EXEC
*/

void		script_in_exec(t_ast *node, t_exec *exe);
t_exec		*exec_script(t_ast *node, t_exec *exe);

#endif
