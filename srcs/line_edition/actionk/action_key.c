/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_key.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 17:05:47 by cyfermie          #+#    #+#             */
/*   Updated: 2018/09/18 16:43:15 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
**	Initialization of an array containing the correspondences between
**	the action keys and their functions.
*/

static	struct s_action_key	*get_array_action_key(void)
{
	static struct s_action_key	actionk[LE_NB_KEYS] =
	{
		{LE_ARROW_RIGHT, &actionk_cursor_move_right},
		{LE_ARROW_LEFT, &actionk_cursor_move_left},
		{LE_CTRL_A, &actionk_move_cursor_start},
		{LE_HOME, &actionk_move_cursor_start},
		{LE_CTRL_E, &actionk_move_cursor_end},
		{LE_END, &actionk_move_cursor_end},
		{LE_ALT_RIGHT, &actionk_move_cursor_by_word_right},
		{LE_ALT_LEFT, &actionk_move_cursor_by_word_left},
		{LE_BACKSPACE, &actionk_delete_character},
		{LE_DELETE, &actionk_delete_character},
		{LE_CTRL_UP, &actionk_move_cursor_line_up},
		{LE_CTRL_DOWN, &actionk_move_cursor_line_down},
		{LE_CTRL_B, &actionk_copy_to_start},
		{LE_CTRL_F, &actionk_copy_to_end},
		{LE_CTRL_R, &actionk_copy_all},
		{LE_CTRL_P, &actionk_past_clipboard},
		{LE_CTRL_U, &actionk_delete_current_input},
		{LE_CTRL_DASH, &actionk_cut_all},
		{LE_CTRL_OPEN_SQUARE_BRACKET, &actionk_cut_to_start},
		{LE_CTRL_CLOSE_SQUARE_BRACKET, &actionk_cut_to_end},
		{LE_ARROW_UP, &actionk_history_up},
		{LE_ARROW_DOWN, &actionk_history_down},
		{LE_CTRL_D, &actionk_eof},
		{LE_CTRL_L, &actionk_clear_screen},
		{LE_TAB, &autocomplete}
	};

	return (actionk);
}

/*
**	Looking for the function corresponding to the key, and start it.
*/

void						action_key(t_kno key_no, struct s_line *le)
{
	struct s_action_key		*actionk;
	unsigned int			i;

	actionk = get_array_action_key();
	i = 0;
	while (i < LE_NB_KEYS)
	{
		if (actionk[i].key_no == key_no)
		{
			actionk[i].func_ptr(le);
			break ;
		}
		++i;
	}
}
