/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_line_edition_attributes.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 19:44:09 by cyfermie          #+#    #+#             */
/*   Updated: 2018/10/22 16:02:02 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
**	Initialize the termcaps library for the terminal.
*/

static void		init_termcaps(void)
{
	const char	*term;

	if ((term = getenv("TERM")) == NULL)
		term = "xterm-256color";
	if (tgetent(NULL, term) != 1)
	{
		ft_putstr_fd("Error while getting terminal attributes\n", 2);
		exit(EXIT_FAILURE);
	}
}

/*
**	Initialize some datas for the main datas structure, but only one time.
**	This function is only called on the first initialization of the line edition 
**	feature.
*/

static void			init_once(struct s_line *le)
{
	init_termcaps();

	le->tcaps = init_termcaps_strings();
	le->history = ft_xmalloc(sizeof(struct s_history));
	le->history->prev = NULL;
	le->history->next = NULL;
	le->history->cmd = NULL;
	le->save_tmp_cmd = NULL;
	le->clipboard = ft_xmalloc(sizeof(char) * LE_DEFAULT_LINE_SIZE);
	ft_memset(le->clipboard, '\0', LE_DEFAULT_LINE_SIZE);
	le->clipboard_size = LE_DEFAULT_LINE_SIZE;
	le->clipboard_len = 0;
	le->le_state.opt_colosyn = true;
	load_history_file(le);
	le->le_state.le_char_delim = LE_CHAR_DELIM_DEFAULT;
}

/*
**	Initialize some datas for the main datas structure to their default values.
*/

void    			init_line_edition_attributes(struct s_line *le, int prompt_type)
{
	static bool already_init = false;

	if (already_init == false)
	{
		init_once(le);
		already_init = true;
	}
	le->key_no = 0;
	le->cmd = ft_xmalloc(sizeof(char) * LE_DEFAULT_LINE_SIZE);
	le->cmd_size = LE_DEFAULT_LINE_SIZE;
	ft_memset(le->cmd, '\0', le->cmd_size);
	le->cmd_len = 0;
	le->cursor_index = 0;
    le->start_pos = prompt_show(g_prompts[-prompt_type]);
    le->cursor_pos = le->start_pos;
	le->cursor_line = 0;
	le->term_nb_lines = get_terminal_nb_lines();
	le->term_line_size = get_terminal_nb_col();
	le->nb_lines_written = 1;
	le->nb_char_on_last_line = 0;
	le->le_state.le_is_init = true;
	le->le_state.prompt_type = prompt_type;
}
