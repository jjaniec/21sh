/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_shell_data.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/27 19:31:39 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/03 18:17:33 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Free all environnement data
*/

static void		free_environ(void)
{
	t_shell_vars	*vars;

	if (!(vars = get_shell_vars()))
		return ;
	free_env_entries(vars->env, vars->env->env_entries_list);
	free_env_entries(vars->locals, vars->locals->env_entries_list);
	free_env_entries(vars->internals, vars->internals->env_entries_list);
	//free(env_struct);
	vars = NULL;
}

/*
** Free all shell data before exiting
*/

void			free_all_shell_data(void)
{
	free_environ();
	ast_free(*(access_ast_data()));
}
