/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_history.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 19:08:07 by cyfermie          #+#    #+#             */
/*   Updated: 2018/11/11 16:27:35 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
**	Without option : print the history list with an index.
**	--clear : empty the history.
**	--save : save the history into a file.
**	history n : print the last 'n' elements.
**	-d n : deletes the element with the index 'n'.
*/

void	builtin_history(char **argv, t_environ *env, t_exec *exe)
{
	(void)env;
	exe->ret = 0;
	if (ft_count_elem_2d_array(argv + 1) == 0)
		blthis_print_history_with_indexes();
	else if (ft_count_elem_2d_array(argv + 1) == 1)
	{
		if (ft_strequ(argv[1], "--clear"))
			blthis_clear_history(access_le_main_datas());
		else if (ft_strequ(argv[1], "--save"))
			exe->ret = blthis_save_history_in_file(access_le_main_datas()->history) \
			== true ? (0) : (1);
		else if (ft_str_is_positive_numeric(argv[1]) == true)
			blthis_print_n_last_elem(access_le_main_datas(), ft_atoi(argv[1]));
		else
			ft_putstr_fd(BUILTIN_HISTORY_USAGE, STDERR_FILENO);
	}
	else if (ft_count_elem_2d_array(argv + 1) == 2)
	{
		if (ft_strequ(argv[1], "-d") && ft_str_is_positive_numeric(argv[2]))
			blthis_delete_element_number_n(ft_atoi(argv[2]));
		else
			ft_putstr_fd(BUILTIN_HISTORY_USAGE, STDERR_FILENO);
	}
	else
		ft_putstr_fd(BUILTIN_HISTORY_USAGE, STDERR_FILENO);
}
