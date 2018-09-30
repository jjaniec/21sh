/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoc_menu_print_spaces.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 15:38:21 by cgaspart          #+#    #+#             */
/*   Updated: 2018/09/16 15:38:23 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

void	autoc_menu_print_spaces(int longest, int len, struct s_line *le)
{
	if (len != 0)
	{
		while (len >= 1 && longest > 0)
		{
			len--;
			longest--;
		}
	}
	else
		longest--;
	while (longest)
	{
		tputs(le->tcaps->nd, 1, &write_one_char);
		longest--;
	}
	tputs(le->tcaps->nd, 1, &write_one_char);
}
