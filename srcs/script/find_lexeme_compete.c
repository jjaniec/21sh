/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_lexeme_compete.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/20 19:15:02 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/01 11:57:14 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Find the end_lexeme for all end_token[]
*/

static t_lexeme		**loop_find_tokens(const int end_token[], \
					int size_end_token, t_lexeme *lex)
{
	unsigned int	i;
	t_lexeme		**end;

	i = 0;
	end = (t_lexeme **)ft_memalloc(sizeof(t_lexeme *) * (size_end_token + 1));
	if (!end)
		exit(MALLOC_ERROR);
	while (end_token[i])
	{
		end[i] = find_end_lexeme_solo(lex, end_token[i]);
		i++;
	}
	return (end);
}

/*
** Calculating the path length for all tokens.
*/

static unsigned int	*loop_paths(const int end_token[], \
					int size_end_token, t_lexeme *lex, t_lexeme **end)
{
	unsigned int	i;
	t_lexeme		*save;
	unsigned int	*paths;

	paths = (unsigned int *)ft_memalloc(sizeof(unsigned int) \
			* (size_end_token + 1));
	if (!paths)
		exit(MALLOC_ERROR);
	i = 0;
	while (end_token[i])
	{
		save = lex;
		while (end[i] && save && save != end[i])
		{
			paths[i]++;
			save = save->next;
		}
		i++;
	}
	return (paths);
}

/*
** Choose the closest path.
*/

static unsigned int	loop_closest(const int end_token[], unsigned int *paths)
{
	unsigned int	i;
	unsigned int	closest;

	i = 0;
	closest = -1;
	while (end_token[i])
	{
		if (paths[i] != 1 && closest > paths[i])
			closest = paths[i];
		i++;
	}
	return (closest);
}

/*
** Find and return the closest' sub_ast' closing' lexeme corresponding to \
** t_lexeme *lex from all token proposed in end_token[]
*/

t_lexeme			*find_lexeme_compete(t_lexeme *lex, \
					const int end_token[])
{
	int			size_end_token;
	t_lexeme		**end;
	unsigned int	*paths;
	unsigned int	closest;

	size_end_token = 0;
	while (end_token[size_end_token])
		size_end_token++;
	end = loop_find_tokens(end_token, size_end_token, lex);
	paths = loop_paths(end_token, size_end_token, lex, end);
	closest = loop_closest(end_token, paths);
	while (lex && closest--)
		lex = lex->next;
	free(end);
	free(paths);
	return (lex);
}
