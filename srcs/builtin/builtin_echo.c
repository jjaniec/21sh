/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 17:33:25 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/01 18:59:16 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** https://www.unix.com/man-page/posix/1posix/echo
*/

void		builtin_echo(char **argv, t_environ *env, t_exec *exe)
{
	(void)exe;
	(void)env;
	argv++;
	while (*argv)
	{
		ft_putstr(*argv);
		if (argv[1])
			ft_putchar(' ');
		argv++;
	}
	ft_putchar('\n');
	exit(0);
}
