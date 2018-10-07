/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_lexeme.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:48:09 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/01 11:27:33 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Creates a lexeme struct with type and data
*/

t_lexeme		*create_lexeme(int type, char *data, \
					int type_details, char **lexeme_begin_end_ptrs)
{
	t_lexeme	*e;

	e = NULL;
	if (data && *data)
	{
		if (!(e = malloc(sizeof(t_lexeme))))
			exit(MALLOC_ERROR);
		e->type = type;
		e->data = data;
		e->type_details = type_details;
		e->lexeme_begin_ptr = lexeme_begin_end_ptrs[0];
		e->lexeme_end_ptr = lexeme_begin_end_ptrs[1];
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
