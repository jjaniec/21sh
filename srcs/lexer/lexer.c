/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/12 15:19:12 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/01 10:25:36 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Skip $IFS Separators and returns get_lexeme_type
** if something other than separators is found
*/

static size_t		get_lexeme(char *str, int *pos, \
						char **evaluated_str, size_t *type_details)
{
	size_t	r;

	r = 0;
	if (str[*pos])
		r = get_lexeme_type(str, pos, evaluated_str, type_details);
	return (r);
}

/*
** Add created lexeme to the end of our linked list
** or init linked list with created elem if $ll == NULL
*/

static t_lexeme		*add_lexeme_to_list(t_lexeme *e, \
						t_lexeme **ll, t_lexeme **last_elem)
{
	if (!(*ll))
	{
		*ll = e;
		*last_elem = e;
	}
	else if (*last_elem)
	{
		(*last_elem)->next = e;
		*last_elem = (*last_elem)->next;
	}
	return (e);
}

/*
** Returns created lexeme or NULL if no lexeme are found
*/

static int			make_next_lexeme(char *line, int *pos, \
						t_lexeme **lexemes, t_lexeme **cur_lexeme)
{
	size_t		type[2];
	char		*data;
	char		*lexeme_begin_end_ptrs[2];
	t_lexeme	*e;

	type[1] = TK_DEFAULT;
	while (line[*pos] && line[*pos] != '\\' && line[*pos] != '\n' && \
			is_separator(line[*pos]))
		*pos += 1;
	if (line[*pos])
	{
		lexeme_begin_end_ptrs[0] = &(line[*pos]);
		type[0] = get_lexeme(line, pos, &data, &(type[1]));
		if ((int)type[0] == UNMATCHED_QUOTE_ERR)
			return (UNMATCHED_QUOTE_ERR);
		lexeme_begin_end_ptrs[1] = &(line[*pos]);
		if ((e = create_lexeme(type, data, *pos, lexeme_begin_end_ptrs)))
			add_lexeme_to_list(e, lexemes, cur_lexeme);
		return (1);
	}
	return (0);
}

/*
** Returns a linked list of parsed 'lexemes' depending on shell syntax
** line: input received from user
*/

int					lexer(char *line, t_lexeme **lexemes, \
						char **unmatched_quote_err_ptr)
{
	t_lexeme	*cur_elem;
	int			i;
	int			r;

	if (unmatched_quote_err_ptr)
		*unmatched_quote_err_ptr = NULL;
	*lexemes = NULL;
	i = 0;
	r = 0;
	if (line)
		while (line[i])
		{
			r = make_next_lexeme(line, &i, lexemes, &cur_elem);
			if (r == UNMATCHED_QUOTE_ERR)
			{
				if (unmatched_quote_err_ptr)
				{
					*unmatched_quote_err_ptr = &(line[i]);
					log_fatal("Unmatched_quote_err_ptr: |%s|", *unmatched_quote_err_ptr);
				}
				break ;
			}
			else if (r == 0)
				break ;
		}
	env_assigns_status(*"resets env_assigns_passed value to 0", 0);
	script_lexemes(*lexemes);
	return (r);
}
