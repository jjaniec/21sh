/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   order_tab_ascii.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/31 15:18:13 by cgaspart          #+#    #+#             */
/*   Updated: 2018/08/31 15:18:15 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

char			**order_tab_ascii(char **tabl)
{
	int		i;
	char	*buff;

	i = 0;
	while (tabl[i])
	{
		if (tabl[i + 1] && ft_strcmp(tabl[i], tabl[i + 1]) > 0)
		{
			buff = tabl[i];
			tabl[i] = tabl[i + 1];
			tabl[i + 1] = buff;
			i = 0;
		}
		else
			i++;
	}
	return (tabl);
}
