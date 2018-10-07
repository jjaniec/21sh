/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_lexeme_colorized.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 07:13:38 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/02 11:35:02 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

extern t_option		g_sh_opts[];

/*
** Returns 1 if file in lexeme_data was found by searching directories in
** $PATH or if path in lexeme_data exists,
** and store stat of file in elem_stats,
** otherwise return 0
*/

static int		elem_path_found(struct stat *elem_stats, \
					char *lexeme_data, t_environ *env, int item_nb)
{
	char		*path_entry;
	char		*tmp;
	int			r;

	r = 0;
	if (ft_strchr(lexeme_data, '/') || item_nb > 0)
	{
		if (lstat(lexeme_data, elem_stats) != -1)
			return (1);
		return (0);
	}
	else
	{
		path_entry = NULL;
		if (env->get_var(env, "PATH"))
			path_entry = env->last_used_elem->val_begin_ptr;
		if (path_entry)
		{
			if ((tmp = isin_path(path_entry, lexeme_data)))
			{
				lstat(tmp, elem_stats);
				r = 1;
			}
			else if (!r && is_builtin(lexeme_data, NULL))
				r = 1;
			ft_strdel(&tmp);
		}
	}
	return (r);
}

/*
** Print color associated w/ stats of program argument
** (directory / file / not found)
*/

static void		print_prog_name_arg_col(struct stat *elem_stats, \
					int elem_found, int item_nb)
{
	if (elem_found)
	{
		if (item_nb == 0)
			ft_putstr(COL_PROG_NAME_FOUND);
		else
		{
			if (S_ISDIR(elem_stats->st_mode))
				ft_putstr(COL_PROG_ARG_DIR);
			else
				ft_putstr(COL_PROG_ARG_FILE);
		}
	}
	else
	{
		if (item_nb == 0)
			ft_putstr(COL_PROG_NAME_NOT_FOUND);
		else
			ft_putstr(COL_PROG_ARG_NOT_FOUND);
	}
}

/*
** Prints color associated w/ type of current element
*/

static void		put_lexeme_color(t_lexeme *lexeme, char *lexeme_begin, \
					t_environ *env)
{
	struct stat		elem_stats;
	static int		item_nb = -1;

	item_nb++;
	if (lexeme->type == T_CTRL_OPT)
	{
		ft_putstr(COL_OPERATORS);
		item_nb = -1;
	}
	else if (lexeme->type == T_REDIR_OPT)
		ft_putstr(COL_REDIRS);
	else if (lexeme->type == T_WORD)
	{
		if (*(char *)(lexeme->data) == '-')
			ft_putstr(COL_PROG_OPT);
		else if (*(lexeme_begin) == '$')
			ft_putstr(COL_ENV_VAR_EXPANSION);
		else if (*(lexeme_begin) == '\'' || *(lexeme_begin) == '"')
			ft_putstr(COL_QUOTED_ARG);
		else
			print_prog_name_arg_col(&elem_stats, \
				elem_path_found(&elem_stats, lexeme->data, env, item_nb), item_nb);
	}
	if (!lexeme->next)
		item_nb = -1;
}

/*
** Print color associated w/ current lexeme w/ put_lexeme_color()
** and reset colors to COL_DEFAULT
*/

void 			print_to_line_edition(const char *s, int nb)
{
	struct s_line *le;

	le = access_le_main_datas();
	while (nb > 0)
	{
		print_key_at_end(le, *s++);
		--nb;
	}
}


void			print_lexeme_colorized(char *lexeme_begin, char *lexeme_end, \
					char *input_ptr, t_lexeme *lexeme, t_environ *env)
{
	put_lexeme_color(lexeme, lexeme_begin, env);
	if (is_option_activated("c", g_sh_opts, NULL))
		write(1, input_ptr, (lexeme_end - input_ptr));
	else
		print_to_line_edition(input_ptr, (lexeme_end - input_ptr));
	ft_putstr(COL_DEFAULT);
}
