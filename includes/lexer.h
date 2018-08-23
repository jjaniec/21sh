/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 15:14:05 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/18 15:37:06 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <twenty_one_sh.h>

# define UNMATCHED_QUOTE_ERR	-1

/*
** Backslashes rules for handle_backslash_escape()
*/

# define NOT_IN_QUOTES		0
# define IN_SQUOTES			1
# define IN_DQUOTES			2

/*
** Lexeme types and TOKENS
*/

/*
** ex: ENV_VAR=VAL command
*/

# define T_ENV_ASSIGN		1

/*
** Default type and
** default token for type_details
*/

# define T_WORD				2

# define TK_DEFAULT			0

/*
** Control operators:
** &
** &&
** // (
** // )
** ;
** ;; // only used for case statements, not mandatory
** '\n'
** |
** ||
*/

# define T_CTRL_OPT			3

# define TK_AND				31
# define TK_DAND			32
# define TK_SEMICOLON		33
# define TK_NEWLINE			34
# define TK_PIPE			35
# define TK_OR				36

/*
** Redirect operators:
** <
** > or >| http://pubs.opengroup.org/
**			onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_07_02
** >>
** << or <<-
** <& http://pubs.opengroup.org/
**			onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_07_05
** >&
** <> http://pubs.opengroup.org/
**			onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_07_07
*/

/*
** IO redirects operators lexing particularities:
** when input fd is not specified, data of the lexeme element will only
** be the token specifier, example:
**   with "ls >&1", data of the TK_GREATAND operator will be ">&"
** else if an input fd is specified and no IFS separator are between '>'/'<'
** and the fd number, the data of our operator will contain
** the input fd number in the data string,
** example:
**   with "ls 42>&1", data of the TK_GREATAND operator will be "42>&"
**   while with "ls 42 >&1", data of the TK_GREATAND operator will be ">&"
**     as "42" will be an argument of ls
** if input fd is a string not only composed of numbers, it will be lexed
** as a T_WORD
** and the data of our operator will not be changed, example:
**   with "echo ls>&2",
**     our linked list will contain
**     (T_WORD)echo -> (T_WORD)ls -> (T_REDIR_OPT)>& -> (T_WORD)2
**     and the fd '1' (default) will be redirected to fd '2'
*/

# define T_REDIR_OPT		4

# define TK_LESS			40
# define TK_GREAT			41
# define TK_CLOBBER			42
# define TK_DGREAT			43
# define TK_DLESS			44
# define TK_DLESSDASH		45
# define TK_TLESS			46
# define TK_LESSAND			47
# define TK_GREATAND		48
# define TK_LESSGREAT		49

t_lexeme				*lexer(char *exp);

int						is_separator(char c);

int						is_operator(char c);

t_lexeme				*create_lexeme(size_t type, char *data, \
							size_t type_details);

size_t					get_lexeme_type(char *s, int *pos, \
							char **data, size_t *type_details);

size_t					lexeme_type_word(char *s, int *pos, char **data);

void					clean_word_lexeme(char **data);

char					*has_matching_quote(char *s, int pos);

int						env_assigns_status(int mode_set, int new_value);

void					handle_backslash_escape(char *s, int *pos,
						int in_quote_type);

void					free_lexemes(t_lexeme *ll);

#endif
