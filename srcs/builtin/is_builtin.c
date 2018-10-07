/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/09 16:41:20 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/01 11:23:17 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Check if function is a builtin
*/

int		is_builtin(char *cmd, \
			void (**builtin_fun_ptr)(char **, t_environ *, t_exec *))
{
	void		*builtin_ret_ptr;

	builtin_ret_ptr = NULL;
	if (ft_strequ(cmd, "echo"))
		builtin_ret_ptr = &builtin_echo;
	else if (ft_strequ(cmd, "cd"))
		builtin_ret_ptr = &builtin_cd;
	else if (ft_strequ(cmd, "setenv"))
		builtin_ret_ptr = &builtin_setenv;
	else if (ft_strequ(cmd, "unsetenv"))
		builtin_ret_ptr = &builtin_unsetenv;
	else if (ft_strequ(cmd, "env"))
		builtin_ret_ptr = &builtin_env;
	else if (ft_strequ(cmd, "exit"))
		builtin_ret_ptr = &builtin_exit;
	else if (ft_strequ(cmd, "return"))
		builtin_ret_ptr = &builtin_return;
	else if (ft_strequ(cmd, "history"))
		builtin_ret_ptr = &builtin_history;
	else if (ft_strequ(cmd, "syntax"))
		builtin_ret_ptr = &builtin_toggle_syntax_highlighting;
	else if (ft_strequ(cmd, "[") || ft_strequ(cmd, "test"))
		builtin_ret_ptr = &builtin_test;
	if (builtin_fun_ptr)
		*builtin_fun_ptr = builtin_ret_ptr;
	return ((builtin_ret_ptr) ? (1) : (0));
}
