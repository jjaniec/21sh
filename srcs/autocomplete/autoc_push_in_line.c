/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoc_push_in_line.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/19 17:02:20 by cgaspart          #+#    #+#             */
/*   Updated: 2018/09/19 17:02:22 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

static char		*get_final_line(struct s_line *le, char *str, char *search)
{
	int i;

	i = 0;
	if (!search)
		return (ft_strjoin(le->cmd, str));
	while (str[i])
	{
		if (str[i] != search[i])
			return (ft_strjoin(le->cmd, &str[i]));
		i++;
	}
	return (NULL);
}

void			autoc_push_in_line(struct s_line *le, char *str, char *search)
{
	char	*final_line;

	final_line = get_final_line(le, str, search);
	if (final_line == NULL)
		return ;
	refresh_colosyn(le, final_line);
	free(final_line);
}
