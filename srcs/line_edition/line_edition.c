/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edition.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 16:29:25 by cyfermie          #+#    #+#             */
/*   Updated: 2018/07/19 16:21:53 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> // debug
FILE *tty_debug = NULL; // debug
#define FOOLOL // debug

#include "../../includes/line_edition.h"
#undef FOOLOL // debug

static void		read_key(char key[LE_KEY_SIZE])
{
	ssize_t read_ret;

	read_ret = read(STDIN_FILENO, key, LE_KEY_SIZE - 1);
	if (read_ret == -1)
	{
		write(STDERR_FILENO, "\nError while reading on stdin\n", 30);
		perror("read() - perror() report");
	}

	// if (read_ret == 0) ?  ctrl + d causes EOF ? think about that later ... 
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

char			*line_edition(void)
{
	char			*final_line;
	char			key[LE_KEY_SIZE];
	struct s_line	le;
	t_kno			key_no;

	set_term_attr(LE_SET_NEW);
	init_line_edition_attributes(&le);

	while ("cest ta mere la jjaniec")
	{
		ft_memset(key, '\0', LE_KEY_SIZE);
		read_key(key);
		//for (int i = 0 ; key[i] ; ++i) printf("pp = %d||\n", key[i]);
		key_no = get_key_number(key);

		if (key_no >= 32 && key_no >= 126)
			fprintf(tty_debug, "key = %" PRIu64 "\n" , key_no); // debug

		process_key(key_no, &le);

fprintf(tty_debug, "--------------------------------------\n");
		fprintf(tty_debug, "current cursor pos = %u\ncurrent cursor line = %u\n",
		le.current_cursor_pos, le.current_cursor_line);
		fprintf(tty_debug, "line index = %u\n", le.line_index);
		fprintf(tty_debug, "cursor index for line = %u\n", le.cursor_index_for_line);
		fprintf(tty_debug, "nb li currently writing = %u\n", le.nb_li_currently_writing);
		fprintf(tty_debug, "nb_car_written_on_last_current_line = %u\n",
		le.nb_car_written_on_last_current_line);
fprintf(tty_debug, "--------------------------------------\n");


		if (key_no == '\n')
		{
			set_term_attr(LE_SET_OLD);
			break ;
			// need more things to do in the future when line is finished
		} 

	}

	if ((final_line = ft_strdup(le.line)) == NULL)
		le_exit("Memory allocation failed\n", "malloc");
	return (final_line);
}


////////////////////////////////////////////////////////////// for tests
/*
void	prompt(void)
{
	printf("SERGE   $> ");
	fflush(stdout);
}

void	prepare_test(void)
{
	tty_debug = fopen(TTY_DEBUG, "w");
	if (tty_debug == NULL)
	{
		perror("perror report - fopen() failed in prepare_test()");
		exit(123);
	}
}

int	 main(void)
{
	prepare_test();
	prompt();
	
	char * s = line_edition();
	printf("\ninput = %s", s);
	free(s);
	fclose(tty_debug);

	return 0;
}
*/