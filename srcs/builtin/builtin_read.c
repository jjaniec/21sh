/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/22 13:18:30 by cyfermie          #+#    #+#             */
/*   Updated: 2018/11/19 15:41:12 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
void	kk(struct s_bltread *options)
{
	le_debug("d = %s\nn = %s\nN = %s\np = %s\ns = %s\n",
	options->opt_d == true ? "true" : "false",
	options->opt_n == true ? "true" : "false",
	options->opt_N == true ? "true" : "false",
	options->opt_p == true ? "true" : "false",
	options->opt_s == true ? "true" : "false"      );

	if (options->opt_d == true)
	{
		{ le_debug("delim opt d = '%c'\n", options->delim_opt_d) }
	}
	if (options->opt_n == true || options->opt_N == true)
	{
		{ le_debug("nb opt n N = %u\n", options->nb_opt_nN) }
	}

	if (options->words_vars == NULL)
	{
		{ le_debug("%s\n", "PAS DE VARS") }
		return ;
	}

	{ le_debug("%s\n", "VARS = ") }
	for (int i = 0 ; options->words_vars[i] != NULL ; ++i)
	{
		{ le_debug("%s\n", options->words_vars[i]) }
	}

}
*/

static int		read_loop(unsigned char *buffer, struct s_bltread *options)
{
	unsigned int	nb_ch;

	nb_ch = 0;
	while ("read")
	{
		if (read(STDIN_FILENO, buffer + nb_ch, 1) == -1)
		{
			if (errno == EINTR)
				return (1);
			else
				exit(EXIT_FAILURE);
		}
		nb_ch += 1;
		if (options->opt_N == false
		&& ((options->opt_d == 1 && buffer[nb_ch - 1] == options->delim_opt_d)
		|| (options->opt_d == false && buffer[nb_ch - 1] == '\n')))
		{
			buffer[nb_ch - 1] = '\0';
			break ;
		}
		else if (options->opt_N == true || options->opt_n == true)
			if (nb_ch == options->nb_opt_nN)
				break ;
	}
	return (0);
}

unsigned char	*read_line(struct s_bltread *options, t_exec *exe)
{
	struct termios	t;
	unsigned char	*buffer;
	int				ret;

	bltread_prepare_reading_line(&t, &buffer, options);
	ret = read_loop(buffer, options);
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	{
		t.c_lflag |= (ICANON | ECHO);
		if (tcsetattr(STDIN_FILENO, TCSANOW, &t) == -1 \
		&& tcsetattr(STDOUT_FILENO, TCSANOW, &t) == -1)
		{
			ft_putstr_fd("Error while setting terminal attributes\n", 2);
			exit(EXIT_FAILURE);
		}
	}
	if (ret == 1)
	{
		exe->ret = 1;
		g_cmd_status.builtin_running = false;
		write(STDOUT_FILENO, "\n", sizeof(char));
		free(options->words_vars);
		options->words_vars = NULL;
		free(buffer);
		buffer = NULL;
		return (NULL);
	}
	return (buffer);
}

static bool		check_order_args(char **args)
{
	unsigned int	i;

	i = 0;
	while (args[i] != NULL && (ft_strequ(args[i], "-n") == true
	|| ft_strequ(args[i], "-N") == true
	|| ft_strequ(args[i], "-p") == true
	|| ft_strequ(args[i], "-s") == true
	|| ft_strequ(args[i], "-d") == true))
	{
		++i;
		if (args[i] == NULL)
			return (true);
		if (ft_strequ(args[i - 1], "-n") == true
		|| ft_strequ(args[i - 1], "-N") == true
		|| ft_strequ(args[i - 1], "-d") == true)
			++i;
	}
	if (args[i] == NULL)
		return (true);
	if (ft_strequ(args[i], "--") == false)
		return (false);
	return (true);
}

static bool		prepare_blt_read(char **argv, struct s_bltread *options,
														t_exec *exe)
{
	if (bltread_get_activated_options(argv + 1, options, false, 0) == false
	|| (options->opt_n == true && options->opt_N == true)
	|| check_order_args(argv + 1) == false)
	{
		ft_putstr_fd(BUILTIN_READ_USAGE, STDERR_FILENO);
		exe->ret = 1;
		g_cmd_status.builtin_running = false;
		free(options->words_vars);
		return (false);
	}
	options->opt_n = true;
	if (options->opt_N == true)
		options->opt_d = false;
	if (options->nb_opt_nN > BLTREAD_MAX_CH)
		options->nb_opt_nN = BLTREAD_MAX_CH;
	if (options->nb_opt_nN == 0)
	{
		exe->ret = 1;
		g_cmd_status.builtin_running = false;
		free(options->words_vars);
		return (false);
	}
	return (true);
}

/*
**	builtin read :
**	wait for the user's input, then split the input into words using IFS,
**	the words are stored in the shell variables given as arguments.
**	If there is more words than variables to store them, the last variable will
**	have all the remaining words.
**	If there is no variable given as argument, the REPLY variable is used.
**
**	option -d x : change the delimiter ('\n' by default) to x.
**	option -n x : read stops if receiving the delimiter character or if it
**	has read x characters. (not compatible with option -N).
**	option -N x : ignore the delimiter character, read stops if it has read
**	x character or encountering EOF. (not compatible with option -n).
**	option -p : a prompt is displayed before reading.
**	option -s : the input is not displayed.
**	issou
*/


void			builtin_read(char **argv, t_environ *env, t_exec *exe)
{
	struct s_bltread	options;
	unsigned char		*buffer;

	(void)(env);
	g_cmd_status.builtin_running = true;
	exe->ret = 0;
	if (prepare_blt_read(argv, &options, exe) == false)
		return ;
//kk( & options );
	buffer = read_line(&options, exe);
	if (buffer != NULL)
		bltread_store_words_in_shell_variables(buffer, &options);
	g_cmd_status.builtin_running = false;
	free(options.words_vars);
	free(buffer);
}
