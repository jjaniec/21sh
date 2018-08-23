/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/09 16:41:20 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/09 17:10:37 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Check if function is a builtin
*/

int		is_builtin(char *cmd, \
			void (**builtin_fun_ptr)(char **, char **, t_exec *))
{
	if (ft_strequ(cmd, "echo"))
		*builtin_fun_ptr = &builtin_echo;
	else if (ft_strequ(cmd, "cd"))
		*builtin_fun_ptr = &builtin_cd;
	else if (ft_strequ(cmd, "setenv"))
		*builtin_fun_ptr = &builtin_setenv;
	else if (ft_strequ(cmd, "unsetenv"))
		*builtin_fun_ptr = &builtin_setenv;
	else if (ft_strequ(cmd, "env"))
		*builtin_fun_ptr = &builtin_env;
	else if (ft_strequ(cmd, "exit"))
		*builtin_fun_ptr = &builtin_exit;
	else if (ft_strequ(cmd, "return"))
		*builtin_fun_ptr = &builtin_return;
	return ((*builtin_fun_ptr) ? (1) : (0));
}
