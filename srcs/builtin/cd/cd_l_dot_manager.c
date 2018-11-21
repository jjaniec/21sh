/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_l_dot_manager.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <cgaspart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 11:57:23 by cgaspart          #+#    #+#             */
/*   Updated: 2018/11/21 22:25:53 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

void	dot_parser(t_cd *cd_i, char *av)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = cd_clean_last_slash(av);
	if (!ft_strchr(tmp, '/'))
	{
		if (cd_i->env->get_var(cd_i->env, "PWD"))
		{
			free(tmp);
			ft_putstr_fd("\n| Deeper with dot |", 2);
			tmp = cd_add_slash(cd_i->env->last_used_elem->val_begin_ptr);
			cd_i->cwd_link = ft_xstrjoin(tmp, av);
			if (!cd_change_dir(cd_i->env, av, cd_i->cwd))
				link_env_update(cd_i);
		}
	}
	free(tmp);
}

void	dot_manager(t_cd *cd_i, char *av)
{
	char *tmp;
	if (!ft_strcmp(av, "."))
		return ;
	if (!ft_strcmp(av, ".."))
	{
		if (cd_i->env->get_var(cd_i->env, "PWD"))
		{
			tmp = cd_clean_last_slash(cd_i->env->last_used_elem->val_begin_ptr);
			cd_i->cwd_link = cd_get_last_path(tmp);
			free(tmp);
			if (!cd_change_dir(cd_i->env, cd_i->cwd_link, cd_i->cwd))
				link_env_update(cd_i);
		}
	}
	else
		dot_parser(cd_i, av);
}
