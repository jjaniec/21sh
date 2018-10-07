/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 15:57:17 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/05 16:31:18 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Update optimization pointers
*/

static void	upd_self_ptrs(t_environ *self, t_env_entry *new_entry_struct)
{
	self->last_used_elem = new_entry_struct;
	self->environ[self->entry_count] = new_entry_struct->entry;
	new_entry_struct->ptr_to_pos_in_environ_tab = \
		&(self->environ[self->entry_count]);
	self->environ[self->entry_count] = new_entry_struct->entry;
	new_entry_struct->prev = self->last_entry_ptr;
	if (self->last_entry_ptr)
		self->last_entry_ptr->next = new_entry_struct;
	self->last_entry_ptr = new_entry_struct;
	new_entry_struct->next = NULL;
	if (!(self->env_entries_list))
		self->env_entries_list = new_entry_struct;
}

/*
** Add a new environnement variable in the linked list
** and append it in our environnement ptr tab,
** it will not go through all the elements to append it to the end
** but instead use the self->last_entry_ptr pointer,
** if it's used to update the value of a variable,
** it will instead create a duplicate entry as it allows better performances,
** use self->upd_var instead
*/

char		*add_env_var(t_environ *self, char *name, char *entry_value)
{
	int				i;
	char			*assign_char_ptr;
	t_env_entry		*new_entry_struct;
	int				entry_name_len;
	int				entry_value_len;

	if (self->entry_count == MAX_ENV_ENTRIES || !name)
		return (NULL);
	new_entry_struct = malloc(sizeof(t_env_entry));
	new_entry_struct->next = NULL;
	new_entry_struct->ptr_to_pos_in_environ_tab = NULL;
	i = 0;
	if ((assign_char_ptr = ft_strchr(name, '=')) && assign_char_ptr != name)
	{
		ft_strncpy(new_entry_struct->entry, name, MAX_ENV_ENTRY_LEN);
		new_entry_struct->val_begin_ptr = \
			(assign_char_ptr - name + sizeof(char)) + new_entry_struct->entry;
	}
	else if (entry_value && *entry_value)
	{
		entry_name_len = ft_strlen(name);
		entry_value_len = ft_strlen(entry_value);
		ft_strncpy(new_entry_struct->entry, name, MAX_ENV_ENTRY_LEN);
		new_entry_struct->entry[entry_name_len] = '=';
		new_entry_struct->val_begin_ptr = \
			new_entry_struct->entry + sizeof(char) * (entry_name_len + 1);
		ft_strncpy(new_entry_struct->entry + entry_name_len + 1, entry_value, \
			MAX_ENV_ENTRY_LEN - (entry_name_len + 1));
	}
	new_entry_struct->entry[MAX_ENV_ENTRY_LEN] = '\0';
	upd_self_ptrs(self, new_entry_struct);
	log_debug("New environnement var created ->entry: |%s| - ->entry_count: %d / ptr pos in env tab: %s - ->env[entry_count]: |%s| - ->val_begin_ptr |%s|", \
		new_entry_struct->entry, self->entry_count - 1, *(new_entry_struct->ptr_to_pos_in_environ_tab), self->environ[self->entry_count], new_entry_struct->val_begin_ptr);
	self->entry_count += 1;
	return (new_entry_struct->entry);
}
