/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upd_env_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/26 18:08:19 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/03 18:17:17 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

char	*upd_env_var(t_environ *self, char *name, char *new_value)
{
	t_env_entry		*var_ptr;
	char			*var_val_ptr;
	int				var_name_len;

	if (!(var_ptr = self->get_var(self, name)))
		return (NULL);
	var_val_ptr = var_ptr->entry;
	var_name_len = ft_strlen(name);
	ft_strncpy(var_ptr->entry + var_name_len + 1, new_value, \
		MAX_ENV_ENTRY_LEN - (var_name_len + 1));
	self->last_used_elem = var_ptr;
	return (var_ptr->entry);
}
