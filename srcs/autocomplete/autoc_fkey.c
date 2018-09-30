/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoc_fkey.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 16:26:37 by cgaspart          #+#    #+#             */
/*   Updated: 2018/09/25 16:05:02 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

static int		tab_key(char buffer[3], t_autoc *autoc)
{
	(void)buffer;
	autoc->menu_selected++;
	if (!autoc->items[autoc->menu_selected])
		autoc->menu_selected = 0;
	autoc->menu_line = 0;
	return (0);
}

static int		return_key(char buffer[3], t_autoc *autoc)
{
	if (autoc->menu_selected >= 0)
	{
		autoc_push_in_line(autoc->le, autoc->items[autoc->menu_selected],
		autoc->search);
		if (buffer[0] == 32)
			insert_and_print_character_into_cmdline(autoc->le, (t_kno)' ');
	}
	return (1);
}

static int		arrow_key(char buffer[3], t_autoc *autoc)
{
	int	arrow_mask[3];

	arrow_mask[0] = 0;
	if (autoc->menu_selected < 0)
		arrow_mask[0] = 1;
	arrow_mask[1] = (int)buffer[2] - 64;
	arrow_mask[2] = 0;
	if (autoc->nbr_line > 1)
		arrow_mask[2] = 1;
	return (autoc_arrow_process(arrow_mask, autoc));
}

static int		other_key(char buffer[3], t_autoc *autoc)
{
	process_key((uint64_t)buffer[0], autoc->le);
	return (1);
}

void			init_key_func(t_autoc *autoc)
{
	int		i;

	i = -1;
	while (++i < 128)
		autoc->key_function[i] = &other_key;
	autoc->key_function[27] = &arrow_key;
	autoc->key_function[32] = &return_key;
	autoc->key_function[10] = &return_key;
	autoc->key_function[LE_TAB] = &tab_key;
}
