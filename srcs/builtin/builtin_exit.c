/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 17:21:10 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/01 11:22:27 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
**  https://www.unix.com/man-page/posix/1posix/exit
*/

/*
** Check if a string is only composed of numbers
*/

static int	str_is_num(char *str)
{
	if (!(*str))
		return (0);
	while (*str && ft_isdigit(*str))
		str++;
	if (!(*str))
		return (1);
	return (0);
}

/*
** Exit shell and free malloced data,
** if an argument is specified and this argument is numerical,
** return argument value as exit value,
** if the passed argument is non numerical, exit w/ 256,
** otherwise exit with 0 by default
*/

inline void	builtin_exit(char **argv,t_environ *env, t_exec *exe)
{
	int		exit_val;
	int		first_arg_is_num;

	(void)env;
	exit_val = 0;
	first_arg_is_num = 0;
	if (argv && argv[1])
	{
		if ((first_arg_is_num = str_is_num(argv[1])))
			exit_val = ft_atoi(argv[1]);
		if (!first_arg_is_num)
		{
			ft_putstr_fd(SH_NAME": exit: numeric argument required\n", 2);
			exit_val = 255;
		}
		else if (argv[2])
		{
			ft_putstr_fd(SH_NAME": exit: too many arguments\n", 2);
			exe->ret = 1;
			return ;
		}
	}
	else
		exit_val = exe->ret;
	free_all_shell_data();
	exit(exit_val);
}
