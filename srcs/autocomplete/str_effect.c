/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_effect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/28 19:33:08 by cgaspart          #+#    #+#             */
/*   Updated: 2018/08/28 19:33:10 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

void	ft_ul_video(char *str)
{
	tputs(tgetstr("us", NULL), 1, &write_one_char);
	tputs(tgetstr("mr", NULL), 1, &write_one_char);
	ft_putstr(str);
	tputs(tgetstr("me", NULL), 1, &write_one_char);
	tputs(tgetstr("ue", NULL), 1, &write_one_char);
}

void	ft_ul(char *str)
{
	tputs(tgetstr("us", NULL), 1, &write_one_char);
	ft_putstr(str);
	tputs(tgetstr("ue", NULL), 1, &write_one_char);
}

void	ft_video(char *str)
{
	tputs(tgetstr("mr", NULL), 1, &write_one_char);
	ft_putstr(str);
	tputs(tgetstr("me", NULL), 1, &write_one_char);
}
