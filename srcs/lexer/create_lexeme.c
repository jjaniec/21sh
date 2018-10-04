/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_lexeme.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:48:09 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/01 10:26:45 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Creates a lexeme struct with type and data
*/

t_lexeme		*create_lexeme(size_t type[2], char *data, \
					int pos, char **lexeme_begin_end_ptrs)
{
	t_lexeme	*e;

	e = NULL;
	if (data && *data)
	{
		if (!(e = malloc(sizeof(t_lexeme))))
			exit(MALLOC_ERROR);
		e->type = type[0];
		e->data = data;
		e->type_details = type[1];
		e->lexeme_begin_ptr = lexeme_begin_end_ptrs[0];
		e->lexeme_end_ptr = lexeme_begin_end_ptrs[1];
		e->pos = pos;
		e->next = NULL;
		log_info("Created elem w/ data |%s| - type : %zu - begin |%5s| - end |%5s|", data, type, e->lexeme_begin_ptr, e->lexeme_end_ptr);
	}
	else
	{
		log_warn("Skipping creation of empty data element");
		free(data);
	}
	return (e);
}
