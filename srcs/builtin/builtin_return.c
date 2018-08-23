/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_return.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/09 16:38:37 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/09 17:12:29 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

void	builtin_return(char **argv, char **envp, t_exec *exe)
{
	(void)envp;

	exe->ret = ft_atoi(argv[1]);
}
