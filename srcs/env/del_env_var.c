/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 20:26:17 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/03 18:16:52 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Update pointers of all elements after the one to free
*/

static void			update_environ_ptr_tab(t_env_entry *entry_to_del_ptr)
{
	char		**environ_ptr;
	t_env_entry	*entry_to_replace_ptr_to_env;

	environ_ptr = entry_to_del_ptr->ptr_to_pos_in_environ_tab;
	entry_to_replace_ptr_to_env = entry_to_del_ptr;
	while (*environ_ptr && entry_to_replace_ptr_to_env)
	{
		*environ_ptr = *(environ_ptr + 1);
		if (entry_to_replace_ptr_to_env)
		{
			entry_to_replace_ptr_to_env->ptr_to_pos_in_environ_tab -= 1;
			entry_to_replace_ptr_to_env = entry_to_replace_ptr_to_env->next;
		}
		environ_ptr++;
	}
}

/*
** Update pointers of all elements after $entry_to_del_ptr w/ $prev_entry
** when the element to delete isn't the first one of the linked list
*/

static void			remove_env_entry(t_environ *self, \
						t_env_entry *entry_to_del_ptr, t_env_entry *prev_entry)
{
	self->last_used_elem = NULL;
	if (prev_entry)
	{
		prev_entry->next = prev_entry->next->next;
		if (prev_entry->next)
			prev_entry->next->prev = prev_entry;
	}
	else
		self->env_entries_list = self->env_entries_list->next;
	if (self->last_entry_ptr == entry_to_del_ptr)
		self->last_entry_ptr = entry_to_del_ptr->prev;
	update_environ_ptr_tab(entry_to_del_ptr);
	free(entry_to_del_ptr);
	self->entry_count -= 1;
}

/*
** Delete environnement variable by searching through all variables for
** corresponding name, updating elements pointers after the
** one to free, and free'ing it
*/

int					del_env_var(struct s_environ *self, char *varname)
{
	t_env_entry	*cur_env_var_struct;

	if (!(cur_env_var_struct = self->get_var(self, varname)))
	{
		log_error("%s environnement variable not found !", varname);
		return (1);
	}
	remove_env_entry(self, cur_env_var_struct, cur_env_var_struct->prev);
	log_debug("Removed %s environnement variable", varname);
	return (0);
}
