/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 17:33:25 by sbrucker          #+#    #+#             */
/*   Updated: 2018/11/19 18:05:33 by jjaniec          ###   ########.fr       */
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
		if (write(STDOUT_FILENO, *argv, ft_strlen(*argv)) == -1)
		{
			if (errno == EBADF)
				ft_putstr_fd(SH_NAME": write error: "ERR_BAD_FILEDESC, 2);
			exe->ret = 1;
			return ;
		}
		if (argv[1])
			ft_putchar(' ');
		argv++;
	}
	ft_putchar('\n');
	exe->ret = 0;
}
