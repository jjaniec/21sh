/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ_struct_ptrs.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 17:59:37 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/03 18:17:09 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Init pointers and counters of a t_environ struct
*/

void		init_environ_struct_ptrs(t_environ *env_struct)
{
	if (env_struct)
	{
		env_struct->environ[0] = NULL;
		env_struct->environ[MAX_ENV_ENTRIES] = NULL;
		env_struct->last_used_elem = NULL;
		env_struct->last_entry_ptr = NULL;
		env_struct->entry_count = 0;
		env_struct->env_entries_list = NULL;
		env_struct->get_var = get_env_var;
		env_struct->add_var = add_env_var;
		env_struct->upt_var = upd_env_var;
		env_struct->del_var = del_env_var;
	}
	else
		log_error("Trying to init NULL environ struct");
}
