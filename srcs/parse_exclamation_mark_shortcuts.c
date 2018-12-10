/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_exclamation_mark_shortcuts.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/06 19:18:12 by cyfermie          #+#    #+#             */
/*   Updated: 2018/12/10 21:50:13 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

static const char	*get_last_cmd(struct s_history *his)
{
	while (his->cmd != NULL)
		his = his->next;
	if (his->prev != NULL && his->prev->prev != NULL)
		return (his->prev->prev->cmd);
	return (NULL);
}

static const char	*get_cmd_at_index_n(struct s_history *his,
											unsigned int index)
{
	unsigned int	i;

	if (index == 0)
		return (NULL);
	while (his->prev != NULL)
		his = his->prev;
	i = 1;
	while (i < index)
	{
		his = his->next;
		++i;
		if (his == NULL)
			return (NULL);
	}
	return (his->cmd);
}

static const char	*get_cmd_at_index_n_reverse(struct s_history *his,
												unsigned int index)
{
	unsigned int	i;

	if (index == 0)
		return (NULL);
	while (his->next != NULL)
		his = his->next;
	if (his->prev == NULL)
		return (NULL);
	his = his->prev;
	i = 0;
	while (i < index)
	{
		his = his->prev;
		++i;
		if (his == NULL)
			return (NULL);
	}
	return (his->cmd);
}

static const char	*get_last_cmd_beggining_with_str(struct s_history *his,
														const char *str)
{
	size_t	str_len;

	str_len = ft_strlen(str);
	while (his->next != NULL)
		his = his->next;
	if (his->prev == NULL)
		return (NULL);
	his = his->prev;
	if (his->prev == NULL)
		return (NULL);
	his = his->prev;
	while (his != NULL)
	{
		if (ft_strncmp(str, his->cmd, str_len) == 0)
			return (his->cmd);
		his = his->prev;
	}
	return (NULL);
}

const char			*parse_exclamation_mark_shortcuts(const char *excla)
{
	const char		*ret;
	struct s_line	*le;

	ret = NULL;
	le = access_le_main_datas();
	if (excla == NULL || excla[0] != '!' || le->history == NULL
	|| excla[1] == '\0')
		return (NULL);
	if (ft_strequ(excla, "!!") || ft_strequ(excla, "!-1"))
		ret = get_last_cmd(le->history);
	else if (ft_str_is_positive_numeric(excla + 1))
		ret = get_cmd_at_index_n(le->history, ft_atoi(excla + 1));
	else if (excla[1] == '-' && ft_str_is_positive_numeric(excla + 2))
		ret = get_cmd_at_index_n_reverse(le->history, ft_atoi(excla + 2));
	else
		ret = get_last_cmd_beggining_with_str(le->history, excla + 1);
	return (ret);
}
