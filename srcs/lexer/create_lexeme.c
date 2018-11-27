/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_lexeme.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 18:48:09 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/14 18:39:42 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Creates a lexeme struct with type and data
*/

t_lexeme		*create_lexeme(int type[2], char *data, \
					int pos, char **lexeme_begin_end_ptrs)
{
	t_lexeme	*e;

	e = NULL;
	if (data && *data)
	{
		e = ft_xmalloc(sizeof(t_lexeme));
		e->type = type[0];
		e->data = data;
		e->type_details = type[1];
		e->lexeme_begin_ptr = lexeme_begin_end_ptrs[0];
		e->lexeme_end_ptr = lexeme_begin_end_ptrs[1];
		e->pos = pos;
		e->next = NULL;
		log_info("Created elem w/ data |%s| - type: %d - type_details: %d - begin |%5s| - end |%5s|", data, e->type, e->type_details, e->lexeme_begin_ptr, e->lexeme_end_ptr);
		if (e->type == T_CTRL_OPT)
			env_assigns_status(*"Reset env_assign_status variable", 0);
	}
	else
	{
		log_warn("Skipping creation of empty data element");
		free(data);
	}
	return (e);
}
