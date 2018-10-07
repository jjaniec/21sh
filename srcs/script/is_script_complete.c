/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_script_complete.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/05 11:21:11 by sebastien         #+#    #+#             */
/*   Updated: 2018/10/01 11:57:30 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

static t_lexeme	*next_token_then(t_lexeme *lex)
{
	if (lex->type_details == TK_SCRIPT_ELIF)
		return (is_script_complete(lex->next, lex->next, TK_SCRIPT_THEN));
	if (lex->type_details == TK_SCRIPT_ELSE)
		return (is_script_complete(lex->next, lex->next, TK_SCRIPT_FI));
	if (lex->type_details == TK_SCRIPT_FI)
		return (lex->next);
	return (NULL);
}

static t_lexeme	*return_error(t_lexeme *lex)
{
	log_error("There is an error here: %s", lex->data);
	return (NULL);
}

static t_lexeme	*end_of_loop(t_lexeme *lex, t_lexeme *tmp, int next_token)
{
	if (lex && lex->type_details == next_token && next_token != 0)
	{
		if (next_token == TK_SCRIPT_ELIF)
			return (is_script_complete(lex->next, tmp, TK_SCRIPT_CONDITION_IF));
		if (next_token == TK_SCRIPT_THEN)
			return (is_script_complete(lex->next, tmp, TK_SCRIPT_ELIF));
		if (next_token == TK_SCRIPT_ELSE)
			return (is_script_complete(lex->next, tmp, TK_SCRIPT_FI));
		if (next_token == TK_SCRIPT_DO)
			return (is_script_complete(lex->next, tmp, TK_SCRIPT_DONE));
		if (next_token == TK_SCRIPT_CONDITION_IF)
			return (is_script_complete(lex->next, tmp, TK_SCRIPT_THEN));
		if (next_token == TK_SCRIPT_CONDITION_WHILE)
			return (is_script_complete(lex->next, tmp, TK_SCRIPT_DO));
		if (next_token == TK_SCRIPT_FI)
			return (lex);
		if (next_token == TK_SCRIPT_DONE)
			return (lex);
	}
	else if (next_token != 0)
		return (NULL);
	return (tmp);
}

static t_lexeme	*is_start_of_new_script(t_lexeme *lex)
{
	if (lex->type_details == TK_SCRIPT_IF)
		return (is_script_complete(lex->next, lex->next, \
		TK_SCRIPT_CONDITION_IF));
	else if (lex->type_details == TK_SCRIPT_WHILE)
		return (is_script_complete(lex->next, lex->next, \
		TK_SCRIPT_CONDITION_WHILE));
	else
		return (lex);
}

/*
** Check if a script got all of his tokens needed.
*/

t_lexeme		*is_script_complete(t_lexeme *lex, t_lexeme *tmp, \
				int next_token)
{
	if (lex && !good_start(lex) && (next_token >= TK_SCRIPT_CONDITION_IF || \
	next_token == TK_SCRIPT_ELIF || next_token == TK_SCRIPT_DONE))
		return (NULL);
	while (lex && \
	((lex->type_details != next_token && next_token != 0) || next_token == 0))
	{
		if (lex->type_details == TK_SCRIPT_IF \
		|| lex->type_details == TK_SCRIPT_WHILE)
		{
			tmp = is_start_of_new_script(lex);
			if (!tmp)
				return (return_error(lex));
			lex = tmp;
		}
		else if (next_token == TK_SCRIPT_ELIF && \
	(lex->type_details == TK_SCRIPT_ELSE || lex->type_details == TK_SCRIPT_ELIF\
	|| lex->type_details == TK_SCRIPT_FI))
			return (next_token_then(lex));
		else if (lex->type_details > TK_SCRIPT && \
	((lex->type_details != next_token && next_token != 0) || next_token == 0))
			return (return_error(lex));
		lex = lex->next;
	}
	return (end_of_loop(lex, tmp, next_token));
}
