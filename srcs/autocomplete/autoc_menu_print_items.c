/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoc_print_items.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/16 10:45:08 by cgaspart          #+#    #+#             */
/*   Updated: 2018/09/25 18:11:23 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

static int		get_print_infos(t_autoc *autoc)
{
	int i;
	int words;
	int longest;

	longest = 0;
	i = 0;
	while (autoc->items[i])
	{
		if (longest < (int)ft_strlen(autoc->items[i]))
			longest = ft_strlen(autoc->items[i]);
		i++;
	}
	autoc->max_item_len = longest;
	words = (int)autoc->win.ws_col / (longest + 1);
	if (!words)
		return (1);
	i = 0;
	while (autoc->items[i])
		i++;
	autoc->nbr_items = i;
	autoc->nbr_line = (i / words) + ((i % words) ? 1 : 0);
	if (!autoc->nbr_line || autoc->nbr_line >= (int)autoc->win.ws_row)
		return (1);
	autoc->nbr_items_in_line = i / autoc->nbr_line;
	return (0);
}

static void		cursor_back(t_autoc *autoc, struct s_line *le)
{
	unsigned int	original_pos_index;
	unsigned int	tmp_cursor;
	unsigned int	tmp_line;

	original_pos_index = le->cursor_index + le->start_pos;
	tmp_cursor = 0;
	tmp_line = (unsigned int)autoc->menu_line;
	tputs(autoc->le->tcaps->cr, 1, &write_one_char);
	while (tmp_cursor != original_pos_index)
	{
		tputs(autoc->le->tcaps->nd, 1, &write_one_char);
		tmp_cursor++;
	}
	while (tmp_line--)
		tputs(autoc->le->tcaps->up, 1, &write_one_char);
}

static void		menu_new_line(t_autoc *autoc)
{
	tputs(autoc->le->tcaps->_do, 1, &write_one_char);
	tputs(autoc->le->tcaps->cr, 1, &write_one_char);
	tputs(autoc->le->tcaps->cd, 1, &write_one_char);
	autoc->menu_line++;
}

static void		print_col_items(int i, t_autoc *autoc)
{
	int count;

	count = 0;
	while (autoc->items[i] && count <= autoc->nbr_items_in_line)
	{
		if (ft_strchr(autoc->items[i], '/'))
			ft_putstr(COL_PROG_ARG_DIR);
		(autoc->menu_selected == i) ? (ft_video(autoc->items[i])) :
		(ft_putstr(autoc->items[i]));
		autoc_menu_print_spaces(autoc->max_item_len + 1,
			ft_strlen(autoc->items[i]), autoc->le);
		ft_putstr(COL_DEFAULT);
		count++;
		i = i + autoc->nbr_line;
		if (i > autoc->nbr_items)
			break ;
	}
}

void			autoc_menu_print_items(t_autoc *autoc, struct s_line *le)
{
	int line;

	line = 0;
	ioctl(2, TIOCGWINSZ, &autoc->win);
	if (get_print_infos(autoc))
	{
		tputs(autoc->le->tcaps->cl, 1, &write_one_char);
		prompt_show(g_prompts[-(autoc->le->le_state.prompt_type)]);
		ft_putstr(autoc->le->cmd);
		cursor_back(autoc, le);
		return ;
	}
	menu_new_line(autoc);
	while (line < autoc->nbr_line)
	{
		print_col_items(line, autoc);
		menu_new_line(autoc);
		line++;
	}
	cursor_back(autoc, le);
}
