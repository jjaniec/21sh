/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_read_things.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 20:29:21 by cyfermie          #+#    #+#             */
/*   Updated: 2018/11/19 15:41:19 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

void		bltread_prepare_reading_line(struct termios *t,
		unsigned char **buffer, struct s_bltread *options)
{
	*buffer = ft_xmalloc(BLTREAD_MAX_CH + 1);
	ft_memset(*buffer, '\0', BLTREAD_MAX_CH + 1);
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, t) == -1 && tcgetattr(STDOUT_FILENO, t) == -1)
		{
			ft_putstr_fd("Error while getting terminal olo attributes\n", 2);
			exit(EXIT_FAILURE);
		}
		t->c_lflag &= ~(ICANON);
		if (options->opt_s == true)
			t->c_lflag &= ~(ECHO);
		if (tcsetattr(STDIN_FILENO, TCSANOW, t) == -1 \
		&& tcsetattr(STDOUT_FILENO, TCSANOW, t) == -1)
		{
			ft_putstr_fd("Error while setting terminal olo2 attributes\n", 2);
			exit(EXIT_FAILURE);
		}
		if (options->opt_p == true)
			write(STDOUT_FILENO, "> ", 2);
	}
}

static int	bltread_check_options_six(int foo, struct s_bltread *options, char **args,
													unsigned int *i)
{
	if (foo == 1)
	{
		options->opt_d = true;
		if (args[*i + 1] != NULL && ft_strlen(args[*i + 1]) == 1U)
			options->delim_opt_d = args[*i + 1][0];
		else
			return (0);
		*i = *i + 2;
		if (args[*i] == NULL)
			return (1);
		*i = *i - 1;
		return (2);
	}
	options->opt_n = true;
	if (bltread_check_options_four(1, args, *i) == true)
		options->nb_opt_nN = ft_atoi(args[*i + 1]);
	else
		return (0);
	*i += 2;
	if (args[*i] == NULL)
		return (1);
	*i -= 1;
	return (2);
}

int			bltread_check_options_two(struct s_bltread *options, char **args, unsigned int *i,
														bool *fou)
{
	int janice;

	janice = bltread_check_options_five(2, options, args, i);
	if (janice == 2)
		*fou = true;
	else
		return (janice == 0 ? false : true);
	return (2);
}

int			bltread_check_options_three(struct s_bltread *options, char **args, unsigned int *i,
																bool *fou)
{
	int janice;

	janice = bltread_check_options_six(2, options, args, i);
	if (janice == 2)
		*fou = true;
	else
		return (janice == 0 ? false : true);
	return (2);
}

int			bltread_check_options_one(struct s_bltread *options, char **args, unsigned int *i,
																bool *fou)
{
	int janice;

	janice = bltread_check_options_six(1, options, args, i);
	if (janice == 2)
		*fou = true;
	else
		return (janice == 0 ? false : true);
	return (2);
}
