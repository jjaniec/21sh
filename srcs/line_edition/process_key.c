/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_key.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 16:28:40 by cyfermie          #+#    #+#             */
/*   Updated: 2018/10/22 16:00:38 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
**	Return 'true' if 'key' corresponds to a printable value,
**	otherwise returns 'false'.
*/

static bool	key_is_printable(t_kno key)
{
	if (key >= 32 && key <= 126)
		return (true);
	return (false);
}

/*
**	This function handles the behavior of the 'key' entered by the user.
**	The 'key' can be printable, so it will be added to the command line,
**	the 'key' can also be '\n' meaning the user typed the 'enter' key to finish
**	his command line, and the 'key' might corresponds to an action (like moving
**	the cursor, delete a character, etc ...)
*/

void			process_key(struct s_line *le)
{
	if (key_is_printable(le->key_no) == true)
	{
		if (le->le_state.opt_colosyn == true)
			colosyn_add_char(le, le->key_no);
		else if (le->le_state.opt_colosyn == false)
		{
			if (cursor_is_at_end_of_cmd(le) == true)
				print_key_at_end(le, le->key_no);
			else
				insert_and_print_character_into_cmdline(le, le->key_no);
		}
	}
	else if (le->key_no == le->le_state.le_char_delim)
	{
		check_cmd_storage(le, 1);
		le->cmd[le->cmd_len] = le->key_no;
	}
	else
		action_key(le->key_no, le);
}
