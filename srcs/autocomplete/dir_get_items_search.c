/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir_get_items_search.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 13:38:10 by cgaspart          #+#    #+#             */
/*   Updated: 2018/09/25 16:03:39 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

static int		get_matching(char **items, char *search)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (items[i])
	{
		if (ft_strnstr(items[i], search, ft_strlen(search)))
			count++;
		i++;
	}
	return (count);
}

static char		**get_search_result(char **items, char *search, int match_res)
{
	char	**res;
	int		j;
	int		i;

	i = 0;
	j = 0;
	res = (char**)malloc(sizeof(char*) * match_res + 1);
	while (items[i])
	{
		if (ft_strnstr(items[i], search, ft_strlen(search)))
		{
			res[j] = ft_strdup(items[i]);
			j++;
		}
		i++;
	}
	res[j] = NULL;
	return (res);
}

char			**dir_get_items_search(char *in, t_autoc *autoc)
{
	char	**res;
	char	**items;
	int		match_result;

	res = NULL;
	items = dir_get_items(in);
	match_result = get_matching(items, autoc->search);
	if (match_result)
	{
		res = get_search_result(items, autoc->search, match_result);
		autoc_free_items(items);
		if (match_result == 1)
			autoc_push_in_line(autoc->le, res[0], autoc->search);
		else
			return (res);
	}
	autoc_free_items(res);
	return (NULL);
}
