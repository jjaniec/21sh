/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 15:14:05 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/01 10:27:40 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <forty_two_sh.h>

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
# define TK_SUBAST			1

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

/*
** Expansions specifiers needed by handle_quotes_expansions()
*/

# define EXPANSIONS_SPECIFIERS	"$~"

int						lexer(char *line, t_lexeme **lexemes, \
							char **unmatched_quote_err_ptr);

int						is_separator(char c);

int						is_operator(char c);

t_lexeme				*create_lexeme(size_t type[2], char *data, \
							int pos, char **lexeme_begin_end_ptrs);

size_t					get_lexeme_type(char *s, int *pos, \
							char **data, size_t *type_details);

size_t					lexeme_type_word(char *s, int *pos, char **data);

void					handle_quotes_expansions(char **data);

char					*has_matching_quote(char *s, char quote);

int						env_assigns_status(int mode_set, int new_value);

int						handle_escape_offset(char *ptr, int in_quote_type);

void					free_lexemes(t_lexeme *ll);

void					handle_dollar_expansion(t_lexeme_clean_data *l, char **env);

void					handle_tild_expansion(t_lexeme_clean_data *l, char **env);

char					*get_expansion_end(char *str);

#endif
