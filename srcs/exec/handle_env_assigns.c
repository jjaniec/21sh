/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env_assigns.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/10 15:02:19 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/11 15:36:32 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** When process is not forked, create a duplicate of current environnement
** to use with T_ENV_ASSIGNS to prevent modified
** env variables to stay in main environnement
*/

static t_environ	*create_new_tmp_env(t_environ *base_env, char ***tmp_env_assigns_begin)
{
	t_environ	*new_tmp_env;

	new_tmp_env = ft_xmemalloc(sizeof(t_environ));
	init_environ(base_env->environ, new_tmp_env);
	*tmp_env_assigns_begin = new_tmp_env->environ;
	return (new_tmp_env);
}

/*
** Decide which environnement should be used, a new temporary one, or the current
** one when in a fork
*/

static t_environ	*get_env_to_use(t_exec *exe, char ***tmp_env_assigns_begin)
{
	t_shell_vars	*vars;
	t_environ		*new_tmp_env;

	*tmp_env_assigns_begin = NULL;
	if (!(vars = get_shell_vars()))
		return (NULL);
	if (exe->prog_forked)
		*tmp_env_assigns_begin = \
			vars->env->last_entry_ptr->ptr_to_pos_in_environ_tab + 1;
	else
		return (create_new_tmp_env(vars->env, tmp_env_assigns_begin));
	return (vars->env);
}

/*
** Modify environnement returned by get_env_to_use()
** with each T_ENV_ASSIGN in our ast below our program node
*/

char		**handle_env_assigns(t_ast *node, t_exec *exe, \
				t_environ **env_used)
{
	char		*tmp;
	char		**tmp_env_assigns;

	(void)exe;
	if (!(node && node->left && node->left->type == T_ENV_ASSIGN))
		return (NULL);
	tmp_env_assigns = NULL;
	while (node->left)
		node = node->left;
	log_info("PID %zu: Handle env assigns of %s(t %d td %d)", getpid(), node->data[0], \
		node->type, node->type_details);
	*env_used = get_env_to_use(exe, &tmp_env_assigns);
	while (node->type == T_ENV_ASSIGN)
	{
		if (!((*env_used)->get_var(*env_used, node->data[0])))
			(*env_used)->add_var(*env_used, node->data[0], NULL);
		else
		{
			tmp = ft_strchr(node->data[0], '=');
			ft_strncpy((*env_used)->last_used_elem->val_begin_ptr, tmp + sizeof(char), \
				MAX_ENV_ENTRY_LEN - ((tmp - node->data[0]) * sizeof(char)));
		}
		node = node->parent;
	}
	return (tmp_env_assigns);
}
