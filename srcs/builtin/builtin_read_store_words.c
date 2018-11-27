/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_read_store_words.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 18:59:25 by cyfermie          #+#    #+#             */
/*   Updated: 2018/11/21 13:00:04 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

static void	coco_bichon_lox(t_shell_vars *vars, char **split, unsigned int i)
{
	ft_strncat(vars->locals->last_used_elem->val_begin_ptr, " ", \
	MAX_ENV_ENTRY_LEN - \
	ft_strlen(vars->locals->last_used_elem->entry));
	ft_strncat(vars->locals->last_used_elem->val_begin_ptr, split[i], \
	MAX_ENV_ENTRY_LEN - \
	ft_strlen(vars->locals->last_used_elem->entry));
}

static void	aux_delices_de_janice(struct s_bltread *options, t_shell_vars *vars)
{
	unsigned int	i;

	i = 0;
	if (!options->words_vars || !(*(options->words_vars)))
		vars->locals->del_var(vars->locals, "REPLY");
	else
	{
		while (options->words_vars[i])
			vars->locals->del_var(vars->locals, options->words_vars[i++]);
	}
}

static void	sebateau(char **split, struct s_bltread *options, char *var_name,
														t_shell_vars *vars)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	aux_delices_de_janice(options, vars);
	while (split[i])
	{
		if (options->words_vars && options->words_vars[j])
			var_name = options->words_vars[j];
		if (!(vars->locals->get_var(vars->locals, var_name)))
			vars->locals->add_var(vars->locals, var_name, split[i]);
		else
		{
			if (ft_strequ(var_name, "REPLY") || !(options->words_vars[j + 1]))
				coco_bichon_lox(vars, split, i);
			else
				ft_strncpy(vars->locals->last_used_elem->val_begin_ptr, \
				split[i], MAX_ENV_ENTRY_LEN);
		}
		i++;
		if (options->words_vars && options->words_vars[j] \
		&& options->words_vars[j + 1])
			j++;
	}
}

static char	**split_buffer_with_ifs(unsigned char *buffer)
{
	char	**split;

	split = ft_strsplit_bystr((const char *)buffer, LE_IFS);
	if (split == NULL)
		exit(MALLOC_ERROR);
	return (split);
}

void		bltread_store_words_in_shell_variables(unsigned char *buffer,
									struct s_bltread *options)
{
	t_shell_vars	*vars;
	char			*var_name;
	char			**split;

	split = split_buffer_with_ifs(buffer);
	var_name = "REPLY";
	vars = get_shell_vars();
	sebateau(split, options, var_name, vars);
	ft_free_2d_array(split);
}
