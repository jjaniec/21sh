/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_termcaps_strings.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 20:00:25 by cyfermie          #+#    #+#             */
/*   Updated: 2018/09/19 17:08:29 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
**	Initialize some termcaps that will be used for the shell internal behavior.
*/

struct s_le_termcaps	*init_termcaps_strings(void)
{
	static struct s_le_termcaps tc;

	if ((tc.nd = tgetstr("nd", NULL)) == NULL)
		le_exit("Failed to initialize \"nd\" termcap\n", "tgetstr", errno);
	if ((tc.le = tgetstr("le", NULL)) == NULL)
		le_exit("Failed to initialize \"le\" termcap\n", "tgetstr", errno);
	if ((tc._do = tgetstr("do", NULL)) == NULL)
		le_exit("Failed to initialize \"do\" termcap\n", "tgetstr", errno);
	if ((tc.up = tgetstr("up", NULL)) == NULL)
		le_exit("Failed to initialize \"up\" termcap\n", "tgetstr", errno);
	if ((tc.dc = tgetstr("dc", NULL)) == NULL)
		le_exit("Failed to initialize \"dc\" termcap\n", "tgetstr", errno);
	if ((tc.cl = tgetstr("cl", NULL)) == NULL)
		le_exit("Failed to initialize \"cl\" termcap\n", "tgetstr", errno);
	if ((tc.md = tgetstr("md", NULL)) == NULL)
		le_exit("Failed to initialize \"md\" termcap\n", "tgetstr", errno);
	if ((tc.me = tgetstr("me", NULL)) == NULL)
		le_exit("Failed to initialize \"me\" termcap\n", "tgetstr", errno);
	if ((tc.cr = tgetstr("cr", NULL)) == NULL)
		le_exit("Failed to initialize \"me\" termcap\n", "tgetstr", errno);
	if ((tc.cd = tgetstr("cd", NULL)) == NULL)
		le_exit("Failed to initialize \"me\" termcap\n", "tgetstr", errno);
	if ((tc.dl = tgetstr("dl", NULL)) == NULL)
		le_exit("Failed to initialize \"me\" termcap\n", "tgetstr", errno);
	if ((tc.al = tgetstr("al", NULL)) == NULL)
		le_exit("Failed to initialize \"me\" termcap\n", "tgetstr", errno);

	return (&tc);
}
