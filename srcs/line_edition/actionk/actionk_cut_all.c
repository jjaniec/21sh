/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actionk_cut_all.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/02 18:19:51 by cyfermie          #+#    #+#             */
/*   Updated: 2018/11/27 19:38:23 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
**	The command line is entirely copied into the shell clipboard, and
**	entirely cleared. If the command line is empty, it does nothing and
**	the clipboard remains untouched.
*/

void	actionk_cut_all(struct s_line *le)
{
	if (le->cmd[0] == '\0')
		return ;
	actionk_copy_all(le);
	actionk_delete_current_input(le);
}
