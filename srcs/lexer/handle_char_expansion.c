/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_char_expansion.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/19 16:38:44 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/14 19:43:29 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Return environnement variable value of expansion beginning at $ptr
*/

static char		*get_env_var_value(char *ptr, t_shell_vars *vars, int *expansion_name_len)
{
	char	*exp_end_ptr;
	char	exp_end_ptr_char;
	char	*var_value;

	if (!(ptr && *ptr))
		return (NULL);
	var_value = NULL;
	exp_end_ptr = get_expansion_end(ptr + 1);
	if (exp_end_ptr == ptr + sizeof(char) && *exp_end_ptr == '$')
		exp_end_ptr += sizeof(char);
	if (exp_end_ptr == ptr + sizeof(char))
		return (NULL);
	exp_end_ptr_char = *(exp_end_ptr);
	(*exp_end_ptr) = '\0';
	*expansion_name_len = ft_strlen(ptr + 1);
	if (vars->env->get_var(vars->env, ptr + 1))
		var_value = vars->env->last_used_elem->val_begin_ptr;
	else if (vars->internals->get_var(vars->internals, ptr + 1))
		var_value = vars->internals->last_used_elem->val_begin_ptr;
	else if (vars->locals->get_var(vars->locals, ptr + 1))
		var_value = vars->locals->last_used_elem->val_begin_ptr;
	log_trace("Got env var value of |%s| for %s env variable expansion", var_value, ptr + 1);
	*(exp_end_ptr) = exp_end_ptr_char;
	return (var_value);
}

/*
** Concat data of l->new_data and expansion variable value
** in the new string that will substitute l->new_data
*/

static void		concat_expansion_data(t_lexeme_clean_data *l, char *expansion_value)
{
	size_t	expansion_value_len;
	char	*new_data_str;
	int		write_ptr_offset;

	expansion_value_len = ft_strlen(expansion_value);
	l->clean_data_size += expansion_value_len;
	new_data_str = ft_xmalloc(l->clean_data_size);
	ft_strcpy(new_data_str, l->clean_data);
	write_ptr_offset = (l->clean_data_write_ptr - l->clean_data) * sizeof(char);
	ft_strcpy(new_data_str + write_ptr_offset, expansion_value);
	l->clean_data_write_ptr = \
		write_ptr_offset + new_data_str + expansion_value_len;
	free(l->clean_data);
	l->clean_data = new_data_str;
}

/*
** Remakes l->new_data string w/ expansed environnement variable
** and replace new_data_write_ptr pointer to use the new malloced string
** w/ substitute_data()
*/

void			handle_dollar_expansion(t_lexeme_clean_data *l, t_shell_vars *vars)
{
	char	*env_var_value;
	int		expansion_name_len;

	expansion_name_len = 0;
	env_var_value = get_env_var_value(*(l->raw_lexeme_read_ptr), \
		vars, &expansion_name_len);
	log_debug("Env var value: %s", env_var_value);
	if (env_var_value && *env_var_value)
		concat_expansion_data(l, env_var_value);
	else if (ft_strchr(IFS"'\""EXPANSIONS_END_CHARS, *(*(l->raw_lexeme_read_ptr) + sizeof(char))))
	{
		*(l->clean_data_write_ptr++) = '$';
		*(l->clean_data_write_ptr) = '\0';
		*(l->raw_lexeme_read_ptr) += sizeof(char);
		return ;
	}
	(*(l->raw_lexeme_read_ptr)) += (expansion_name_len + 1) * sizeof(char);
}

void			handle_tild_expansion(t_lexeme_clean_data *l, t_shell_vars *vars)
{
	char	*home_path;

	home_path = NULL;
	if (vars->env->get_var(vars->env, "HOME"))
		home_path = vars->env->last_used_elem->val_begin_ptr;
	log_trace("Got env var value of |%s| for HOME env variable expansion", home_path);
	if (home_path && *home_path)
		concat_expansion_data(l, home_path);
	else
		free(home_path);
	(*(l->raw_lexeme_read_ptr)) += sizeof(char);
}
