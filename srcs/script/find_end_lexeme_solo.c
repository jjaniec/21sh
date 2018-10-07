/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_end_lexeme_solo.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 19:29:38 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/01 11:56:20 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Go to the end of an other script.
*/

static t_lexeme	*to_end(t_lexeme *lex)
{
	int			end_token;
	t_lexeme	*save_lex;

	if (lex->type_details == TK_SCRIPT_IF)
		end_token = TK_SCRIPT_FI;
	else if (lex->type_details == TK_SCRIPT_WHILE)
		end_token = TK_SCRIPT_DONE;
	else
		return (lex);
	while (lex && lex->type_details != end_token)
	{
		lex = lex->next;
		save_lex = lex;
		lex = to_end(lex);
		if (save_lex != lex)
			lex = lex->next;
	}
	return (lex);
}

/*
** Fin the next lexeme corresponding to *start with token next_token.
*/

t_lexeme		*find_end_lexeme_solo(t_lexeme *start, const int end_token)
{
	t_lexeme	*lex;

	start = start->next;
	lex = start;
	while (lex && lex->type_details != end_token)
	{
		lex = to_end(lex);
		lex = lex->next;
	}
	if (lex)
		return (lex);
	return (start);
}
