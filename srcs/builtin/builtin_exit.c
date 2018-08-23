/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 17:21:10 by sbrucker          #+#    #+#             */
/*   Updated: 2018/08/09 17:11:40 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

inline void	builtin_exit(char **argv, char **envp, t_exec *exe)
{
	(void)argv;
	(void)envp;

	if (exe->tmp_envp)
		ft_free_argv(exe->tmp_envp);
	if (exe->envp)
		ft_free_argv(exe->envp);
	exit(0);
}
