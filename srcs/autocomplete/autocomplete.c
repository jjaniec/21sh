/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/28 19:23:25 by cgaspart          #+#    #+#             */
/*   Updated: 2018/08/28 19:23:28 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

static int			check_dir(struct s_line *le)
{
	int	i;

	i = le->cursor_index - 1;		
	if (ft_strchr(le->cmd, ' '))
	{
		while (i)
		{
			if (le->cmd[i] > 40)
				return (1);
			i--;
		}
	}
	return (0);
}

static t_autoc		*autoc_setup(struct s_line *le)
{
	t_autoc	*autoc;

	autoc = (t_autoc*)malloc(sizeof(t_autoc));
	if (autoc == NULL)
		return (NULL);
	autoc->le = le;
	autoc->search = NULL;
	autoc->menu_selected = -1;
	autoc->menu_line = 0;
	autoc->nbr_line = 0;
	autoc->nbr_items_in_line = 0;
	autoc->max_item_len = 0;
	autoc->nbr_items = 0;
	if (le->cursor_index > 1 && check_dir(le))
		autoc->items = autoc_dir(autoc);
	else
		autoc->items = NULL;
	return (autoc);
}

void				autocomplete(struct s_line *le)
{
	t_autoc	*autoc;

	autoc = autoc_setup(le);
	if (autoc && autoc->items)
		autoc_menu(autoc, le);
	autoc_mem_free(autoc);
}
