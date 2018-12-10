/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_shell_datas.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/27 19:31:39 by jjaniec           #+#    #+#             */
/*   Updated: 2018/12/10 21:49:56 by jjaniec          ###   ########.fr       */
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
	if (vars->hashtable)
		free_hashtable(vars->hashtable);
	free_env_entries(vars->env, vars->env->env_entries_list);
	free_env_entries(vars->locals, vars->locals->env_entries_list);
	free_env_entries(vars->internals, vars->internals->env_entries_list);
	vars = NULL;
}

/*
**	Just read the function's name you idiot
*/

static void		free_aliases_list(void)
{
	struct s_alias	*alias;
	struct s_alias	*del;

	alias = access_alias_datas();
	free(alias->key);
	free(alias->value);
	alias = alias->next;
	while (alias != NULL)
	{
		del = alias;
		alias = alias->next;
		free(del->key);
		free(del->value);
		free(del);
	}
}

/*
** Free all shell datas before exiting
*/

void			free_all_shell_datas(void)
{
	t_ast	**ast_ptr;

	free_environ();
	if ((ast_ptr = access_ast_data()))
		ast_free(*ast_ptr);
	le_free_datas();
	free_aliases_list();
	if (g_cmd_status.interactive_mode == true)
		free(get_parsed_aliases_file_path());
}
