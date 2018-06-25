/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 15:14:05 by jjaniec           #+#    #+#             */
/*   Updated: 2018/06/25 14:45:19 by jjaniec          ###   ########.fr       */
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

# define T_ENV_ASSIGN		1

/*
** ex: ENV_VAR=VAL command
*/

# define T_WORD				2

# define T_CTRL_OPT			3

# define TK_AND				31
# define TK_DAND			32
# define TK_SEMICOLON		33
# define TK_NEWLINE			34
# define TK_PIPE			35
# define TK_OR				36

/* Control operators:
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

# define T_REDIR_OPT		4

# define TK_LESS			41
# define TK_GREAT			42
# define TK_CLOBBER			43
# define TK_DGREAT			44
# define TK_DLESS			45
# define TK_DLESSDASH		46
# define TK_LESSAND			47
# define TK_GREATAND		48
# define TK_LESSGREAT		49

/*
** Redirect operators:
** <
** > or >| http://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_07_02
** >>
** << or <<-
** <& http://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_07_05
** >&
** <> http://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html#tag_02_07_07
*/

# define TK_DEFAULT			0

/*
** Default token for type_details if not applied (word lexemes)
*/

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
	char				*data;
	struct s_lexeme		*next;
}						t_lexeme;

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

void					handle_backslash_escape(char *s, int *pos, int in_quote_type);

#endif
