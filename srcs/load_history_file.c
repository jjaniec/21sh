/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_history_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 16:41:01 by cyfermie          #+#    #+#             */
/*   Updated: 2018/09/30 14:41:50 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
**	Load the history file into a linked list used by the shell.
*/

void	load_history_file(struct s_line *le)
{
	int     fd;
	int     ret;
	char    *line;
	char	*his_file_path;

	his_file_path = get_parsed_history_file_path();
	if (his_file_path == NULL || check_history_file(his_file_path) == false
	|| (fd = open(his_file_path, O_RDONLY)) == -1
	|| le->history == NULL)
	{
		ft_putstr_fd("42sh: error while loading .42sh_history\n", \
		STDERR_FILENO);
		return ;
	}
	while ("there is still something to read")
	{
		ret = get_next_line(fd, &line);
		if (ret == -1)
		{
			ft_putstr_fd("42sh: error while loading .42sh_history\n", \
			STDERR_FILENO);
			return ;
		}
		else if (ret == 0)
			break ;
		add_history(line, le);
		free(line);
	}
}
