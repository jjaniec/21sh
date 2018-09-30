/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoc_menu.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 16:22:54 by cgaspart          #+#    #+#             */
/*   Updated: 2018/09/01 16:22:58 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

int				autoc_menu(t_autoc *autoc, struct s_line *le)
{
	autoc_menu_print_items(autoc, le);
	init_key_func(autoc);
	while (autoc_key_reader(autoc))
		autoc_menu_print_items(autoc, le);
	return (1);
}
