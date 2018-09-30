/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoc_arrow.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/24 17:56:57 by cgaspart          #+#    #+#             */
/*   Updated: 2018/09/24 17:56:58 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

static void		not_autoc_arrow(int arrow, t_autoc *autoc)
{
	if (arrow == 1)
		actionk_history_up(autoc->le);
	if (arrow == 2)
		actionk_history_down(autoc->le);
	if (arrow == 3)
		actionk_cursor_move_right(autoc->le);
	if (arrow == 4)
		actionk_cursor_move_left(autoc->le);
}

static void		columns_arrow(int arrow, t_autoc *autoc)
{
	if (arrow == 1)
		autoc->menu_selected--;
	if (arrow == 2)
		autoc->menu_selected++;
	if (arrow == 3)
	{
		autoc->menu_selected += autoc->nbr_line;
		if (autoc->menu_selected >= autoc->nbr_items)
			autoc->menu_selected -= autoc->nbr_items;
	}
	if (arrow == 4)
	{
		autoc->menu_selected -= autoc->nbr_line;
		if (autoc->menu_selected == -autoc->nbr_line)
			autoc->menu_selected = autoc->nbr_items - 1;
		if (autoc->menu_selected < 0)
			autoc->menu_selected += autoc->nbr_items;
	}
}

static void		one_line_arrow(int arrow, t_autoc *autoc)
{
	if (arrow == 3)
		autoc->menu_selected++;
	if (arrow == 4)
		autoc->menu_selected--;
}

int				autoc_arrow_process(int arrow_mask[3], t_autoc *autoc)
{
	if (arrow_mask[0])
	{
		not_autoc_arrow(arrow_mask[1], autoc);
		return (1);
	}
	else if (arrow_mask[2])
		columns_arrow(arrow_mask[1], autoc);
	else
		one_line_arrow(arrow_mask[1], autoc);
	if (autoc->menu_selected >= autoc->nbr_items)
		autoc->menu_selected = 0;
	if (autoc->menu_selected < 0)
		autoc->menu_selected = autoc->nbr_items - 1;
	autoc->menu_line = 0;
	return (0);
}
