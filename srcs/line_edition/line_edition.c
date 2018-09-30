/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edition.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 16:29:25 by cyfermie          #+#    #+#             */
/*   Updated: 2018/09/28 17:12:32 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // debug
FILE *tty_debug = NULL; // debug
#define FOOLOL // debug


#include <twenty_one_sh.h>
#undef FOOLOL // debug

// debug function
static void		le_debug_infos(void)
{return ;
	struct s_line	*le = access_le_main_datas();

	le_debug("%s", "--------------------------------------\n");
	le_debug("PID du 42sh = %d\n", (int)getpid());
	le_debug("key number = %llu\n", (unsigned long long)(le->key_no));
	le_debug("cmd = |%s|\n", le->cmd);
	le_debug("cmd size = %zu\n", le->cmd_size);
	le_debug("cmd len = %u\n", le->cmd_len);
	le_debug("cursor index = %u - %c\n", \
	le->cursor_index, le->cmd[le->cursor_index]);
	le_debug("start pos = %u\n", le->start_pos);
	le_debug("cursor pos = %u\ncursor line = %u\n", \
	le->cursor_pos, le->cursor_line);
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


static void		read_key(char key[LE_KEY_SIZE])
{
	ssize_t read_ret;

	read_ret = read(STDIN_FILENO, key, LE_KEY_SIZE - 1);
	if (read_ret == -1)
	{
		write(STDERR_FILENO, "\nError while reading on stdin\n", 30);
		perror("read() - perror() report");
	}

	// check les differentes erreurs de read(), une coupure a cause d'un signal ...
}

static t_kno	get_key_number(const char *key)
{
	t_kno	key_no;
	t_kno	i;

	key_no = 0;
	i = 0;
	while (i < LE_KEY_SIZE)
	{
		key_no += ((t_kno)key[i]) << i;
		++i;
	}
	return (key_no);
}

char			*line_edition(int prompt_type)
{
	char					*final_line;
	char					key[LE_KEY_SIZE];
	static struct s_line	*le;
	t_kno					key_no;

	le = access_le_main_datas();
	init_line_edition_attributes(le, prompt_type);
	set_term_attr(LE_SET_NEW);

	le_debug_infos(); // debug
	while ("cest ta merge la jjaniec")
	{
		ft_memset(key, '\0', LE_KEY_SIZE);
		read_key(key);

		//for (int i = 0 ; key[i] ; ++i) printf("pp = %d||\n", key[i]);
		key_no = get_key_number(key);

		//if (key_no >= 32 && key_no >= 126)
			//fprintf(tty_debug, "key = %" PRIu64 "\n" , key_no); // debug

		process_key(key_no, le);
		le_debug_infos(); // debug

		if (key_no == '\n')
		{
			tputs(le->tcaps->_do, 1, &write_one_char);
			tputs(le->tcaps->cr, 1, &write_one_char);
			tputs(le->tcaps->cd, 1, &write_one_char);
			tputs(le->tcaps->up, 1, &write_one_char);
			set_term_attr(LE_SET_OLD);
			break ;
		}
	}

	actionk_move_cursor_end(le);
	reset_history_on_first_elem(le);

	if ((final_line = ft_strdup(le->cmd)) == NULL)
		le_exit("Memory allocation failed\n", "malloc", errno);
	free(le->cmd);
	le->cmd = NULL;
	return (final_line);
}


////////////////////////////////////////////////////////////// for tests
/*
void	prompt(void)
{
	printf("PROMPT  $> ");
	fflush(stdout);
}
*/

/*
void	prepare_test(void)
{
	tty_debug = fopen(TTY_DEBUG, "w");
	if (tty_debug == NULL)
	{
		perror("perror report - fopen() failed in prepare_test()");
		exit(123);
	}
}
*/

/*
int	 main(void)
{
	char	*input;

	prepare_test();

	while ("\x1b\x5b\x48\x1b\x5b\x32\x4a\x00\x66\x75\x63\x6b")
	{
		prompt();

		input = line_edition();
		//printf("\ninput = |%s|\n", s);

		if ( strcmp(input, "q\n") == 0 )
			break ;

		//printf("\nINPUT = |%s|", input);

		free(input);

#define TERPRI putchar('\n')
		if (TERPRI, TERPRI, TERPRI)
			;
	}

	fclose(tty_debug);

	return
	!!!!!!!!!!!!!
	"patate + licorne = patatorne"
	+ !!!
	TERPRI
	;;;
}*/
