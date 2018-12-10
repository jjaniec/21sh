/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aliases_replace.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/27 13:00:45 by sbrucker          #+#    #+#             */
/*   Updated: 2018/12/10 21:49:38 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

struct s_alias		*is_an_alias(const char *data,
					const struct s_alias *iterator)
{
	while (iterator)
	{
		if (iterator->value && ft_strequ(data, iterator->key))
			return ((struct s_alias *)iterator);
		iterator = iterator->next;
	}
	return (NULL);
}

static void			remplace(t_lexeme **lex, t_lexeme *last, t_lexeme **save, \
					char *value)
{
	t_lexeme		*new;
	t_lexeme		*next;

	lexer(value, &new, NULL);
	next = lex[0]->next;
	free(lex[0]->data);
	free(*lex);
	*lex = NULL;
	if (!last)
		*save = new;
	else
		last->next = new;
	if (!new)
	{
		if (!last)
			*save = next;
		else
			last->next = next;
		*lex = next;
		return ;
	}
	while (new && new->next)
		new = new->next;
	new->next = next;
	*lex = new;
}

static void			init_vars(t_lexeme **last, t_lexeme **save, int *first_word,
																t_lexeme *lex)
{
	*last = NULL;
	*save = lex;
	*first_word = 0;
}

static t_lexeme		*need_replace(t_lexeme *lex, const struct s_alias *alias)
{
	struct s_alias	*to_remplace;
	t_lexeme		*last;
	t_lexeme		*save;
	t_lexeme		*next;
	int				first_word;

	init_vars(&last, &save, &first_word, lex);
	while (lex)
	{
		while (first_word && lex && lex->next && lex->type == T_WORD)
		{
			last = lex;
			lex = lex->next;
		}
		first_word = (lex->type == T_WORD && first_word == 0) ? 1 : 0;
		next = lex->next;
		to_remplace = is_an_alias(lex->data, alias);
		if (to_remplace)
			remplace(&lex, last, &save, to_remplace->value);
		if (lex == next && (!(first_word = 0)))
			continue ;
		last = lex;
		lex = lex->next;
	}
	return (save);
}

void				aliases_replace(t_lexeme **lex)
{
	const struct s_alias *alias;

	alias = access_alias_datas();
	*lex = need_replace(*lex, alias);
}
