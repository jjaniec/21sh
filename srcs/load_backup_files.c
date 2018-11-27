/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_backup_files.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/08 16:19:04 by cyfermie          #+#    #+#             */
/*   Updated: 2018/11/18 16:48:52 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

static void	do_the_copy(const char *separator_sign_pos, bool *first_elem, \
								struct s_alias **alias, const char *line)
{
	const char	*check_equal = line;
	while (*check_equal && is_separator(*check_equal))
		++check_equal;
	if (*check_equal == '=')
		return ;
	if (*first_elem == false)
	{
		(*alias)->next = ft_xmalloc(sizeof(struct s_alias));
		(*alias) = (*alias)->next;
		(*alias)->next = NULL;
	}
	(*alias)->key = ft_xmalloc(separator_sign_pos - line + 1);
	ft_memcpy((*alias)->key, line, separator_sign_pos - line);
	(*alias)->key[separator_sign_pos - line] = '\0';
	(*alias)->value = ft_xmalloc(ft_strlen(separator_sign_pos));
	ft_memcpy((*alias)->value, separator_sign_pos + 1, \
	ft_strlen(separator_sign_pos + 1));
	(*alias)->value[ft_strlen(separator_sign_pos + 1)] = '\0';
	*first_elem = false;
}

static void	copy_file_datas_in_aliases_list(struct s_alias *alias, int fd)
{
	int		ret;
	char	*line;
	char	*separator_sign_pos;
	bool	first_elem;

	first_elem = true;
	line = NULL;
	while ("there is still something to read")
	{
		ret = get_next_line(fd, &line);
		if (ret == -1)
		{
			ft_putstr_fd("42sh: error while loading .42sh_aliases\n", 2);
			exit(MALLOC_ERROR);
		}
		else if (ret == 0)
			break ;
		separator_sign_pos = ft_strchr(line, ' ');
		if (separator_sign_pos != NULL)
			do_the_copy(separator_sign_pos, &first_elem, &alias, line);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

/*
**	Load the aliases file into a linked list used by the shell.
*/

void		load_aliases_file(struct s_alias *alias)
{
	int			fd;
	const char	*alias_file_path = get_parsed_aliases_file_path();
	
	if (alias_file_path == NULL || check_backup_file(alias_file_path) == false
	|| (fd = open(alias_file_path, O_RDONLY)) == -1)
	{
		ft_putstr_fd("42sh: error while loading .42sh_aliases\n", 2);
		return ;
	}
	copy_file_datas_in_aliases_list(alias, fd);
	close(fd);
//	free((char *)alias_file_path);
//	alias_file_path = NULL;
}

/*
**	Load the history file into a linked list used by the shell.
*/

void		load_history_file(struct s_line *le)
{
	int			fd;
	int			ret;
	char		*line;
	const char	*his_file_path = get_parsed_history_file_path();

	if (his_file_path == NULL || check_backup_file(his_file_path) == false
	|| (fd = open(his_file_path, O_RDONLY)) == -1 || le->history == NULL)
	{
		ft_putstr_fd("42sh: error while loading .42sh_history\n", 2);
		return ;
	}
	line = NULL;
	while ("there is still something to read")
	{
		ret = get_next_line(fd, &line);
		if (ret == -1)
		{
			ft_putstr_fd("42sh: error while loading .42sh_history\n", 2);
			exit(MALLOC_ERROR);
		}
		else if (ret == 0)
			break ;
		add_history(line, le);
		ft_strdel(&line);
	}
	ft_strdel(&line);
	close(fd);
}
