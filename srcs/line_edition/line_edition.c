/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edition.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 16:29:25 by cyfermie          #+#    #+#             */
/*   Updated: 2018/11/27 12:08:35 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // debug
FILE *tty_debug = NULL; // debug
#define FOOLOL // debug


#include <forty_two_sh.h>
#undef FOOLOL // debug

// debug function
static void		le_debug_infos(void)
{		return ;
	struct s_line	*le = access_le_main_datas();

	le_debug("%s", "--------------------------------------\n");
	le_debug("PID du 42sh = %d\n", (int)getpid());
	le_debug("key number = %llu\n", (unsigned long long)(le->key_no));
	le_debug("cmd = |%s|\n", le->cmd);
	le_debug("cmd size = %zu\n", le->cmd_size);
	le_debug("cmd len = %zu\n", le->cmd_len);
	le_debug("cursor index = %u - %c\n", \
	le->cursor_index, le->cmd[le->cursor_index]);
	le_debug("start pos = %u\n", le->start_pos);
	le_debug("cursor pos = %u\ncursor line = %u\n", \
	le->cursor_pos, le->cursor_line);
	le_debug("term nb lines = %u\n", le->term_nb_lines);
	le_debug("term line size = %zu\n", le->term_line_size);
	le_debug("nb lines written = %u\n", le->nb_lines_written);
	le_debug("nb char on last line = %u\n", \
	le->nb_char_on_last_line);
	le_debug("clipboard = |%s|\n", le->clipboard);
	le_debug("clipboard size = %zu\nclipboard len = %zu\n", \
	le->clipboard_size, le->clipboard_len);
	le_debug("save tmp cmd = |%s|\n", le->save_tmp_cmd);
	le_debug("%s","--------------------------------------\n");
}

static enum e_read_key		get_user_input(char key[LE_KEY_BUFFER_SIZE])
{
	ssize_t read_ret;

	errno = 0;
	read_ret = read(STDIN_FILENO, key, LE_KEY_BUFFER_SIZE - 1);
	if (errno == EINTR)
	{
		if (g_cmd_status.resize_happened == true
		&& access_le_main_datas()->le_state.prompt_type != LE_DEFAULT_PROMPT)
			return (INTR_BY_SIGWINCH);
		return (INTR_BY_SIGINT);
	}
	if (read_ret == -1)
	{
		//{ le_debug("READ EXIT FAILURE PID %i\n", (int) getpid()  ) }
		exit(EXIT_FAILURE);
	}
	return (ALL_IS_ALRIGHT);
}

static char	*read_key(char key[LE_KEY_BUFFER_SIZE], struct sigaction *le_sig)
{
	enum e_read_key		ret_user_input;

	ft_memset(key, '\0', LE_KEY_BUFFER_SIZE);
	ret_user_input = get_user_input(key);
	if (ret_user_input == INTR_BY_SIGINT)
	{
		le_sig->sa_handler = SIG_DFL;
		sigaction(SIGWINCH, le_sig, NULL);
		set_term_attr(LE_SET_OLD);
		return (NULL);
	}
	else if (ret_user_input == INTR_BY_SIGWINCH)
	{
		set_term_attr(LE_SET_OLD);
		return (RESIZE_IN_PROGRESS);
	}
	return (key);
}

t_kno	get_key_number(const char *key)
{
	t_kno	key_no;
	t_kno	i;

	key_no = 0;
	i = 0;
	while (i < LE_KEY_BUFFER_SIZE)
	{
		key_no += ((t_kno)key[i]) << i;
		++i;
	}
	return (key_no);
}

static struct s_line	*prepare_line_edition(int prompt_type, \
											  struct sigaction *le_sig)
{
	struct s_line		*le;

	le = access_le_main_datas();
	init_line_edition_attributes(le, prompt_type);
	set_term_attr(LE_SET_NEW);
	g_cmd_status.cmd_running = false;
	g_cmd_status.sigint_happened = false;

	if (g_cmd_status.resize_happened == true)
		handle_window_resize(le);

	sigfillset(&(le_sig->sa_mask));
	le_sig->sa_flags = 0;
	le_sig->sa_handler = &(handle_sigwinch);
	sigaction(SIGWINCH, le_sig, NULL);
	return (le);
}

char			*line_edition(int prompt_type)
{
	char					*final_line;
	static struct s_line	*le;
	struct sigaction		le_sig;
	char					*ret_read_key;

	le = prepare_line_edition(prompt_type, &le_sig);
le_debug_infos(); // debug
	while (le->key_no != le->le_state.le_char_delim && "cest ta merge la jjaniec")
	{
		ret_read_key = read_key(le->key_buffer, &le_sig);
		if (ret_read_key == NULL || ret_read_key == RESIZE_IN_PROGRESS)
			return (ret_read_key);
		le->key_no = get_key_number(le->key_buffer);
		process_key(le);

		// check si un des 2 flags est actif pour retourner la bonne valeur
		if (g_cmd_status.resize_happened == true || g_cmd_status.sigint_happened == true)
		{
			set_term_attr(LE_SET_OLD);

			le_sig.sa_handler = SIG_DFL;
			sigaction(SIGWINCH, &le_sig, NULL);
		}

		if (g_cmd_status.resize_happened == true)
			return (RESIZE_IN_PROGRESS);
		if (g_cmd_status.sigint_happened == true)
			return (NULL);

le_debug_infos(); // debug
		if (true && le->key_no == le->le_state.le_char_delim) //
		{
			tputs(le->tcaps->_do, 1, &write_one_char);
			tputs(le->tcaps->cr, 1, &write_one_char);
			tputs(le->tcaps->cd, 1, &write_one_char);
			tputs(le->tcaps->up, 1, &write_one_char);
		}
	}
	set_term_attr(LE_SET_OLD);
	actionk_move_cursor_end(le);
	reset_history_on_first_elem(le);
	final_line = ft_xstrdup(le->cmd);
	free(le->cmd);
	le->cmd = NULL;
	le_sig.sa_handler = SIG_DFL;
	sigaction(SIGWINCH, &le_sig, NULL);
	return (final_line);
}
