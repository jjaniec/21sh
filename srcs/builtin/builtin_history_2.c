/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_history_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 17:29:55 by cyfermie          #+#    #+#             */
/*   Updated: 2018/11/11 16:28:35 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
**	Print all history's elements with an index.
*/

void	blthis_print_history_with_indexes(void)
{
	const struct s_history	*his = access_le_main_datas()->history;
	unsigned int			index;

	index = 1;
	if (his != NULL)
	{
		while (his->prev != NULL)
			his = his->prev;
		while (his->next != NULL)
		{
			ft_putnbr(index);
			if (index < 10)
				write(STDOUT_FILENO, "    ", sizeof(char) * 4);
			else if (index < 100)
				write(STDOUT_FILENO, "   ", sizeof(char) * 3);
			else if (index < 1000)
				write(STDOUT_FILENO, "  ", sizeof(char) * 2);
			else if (index < 10000)
				write(STDOUT_FILENO, " ", sizeof(char) * 1);
			ft_printf("%s\n", his->cmd);
			his = his->next;
			++index;
		}
	}
}

/*
**	Delete element number 'n' in the history list.
*/

void	blthis_delete_element_number_n(unsigned int n)
{
	struct s_history	*his;
	struct s_history	*del;
	unsigned int		index;

	if (n == 0 || (his = access_le_main_datas()->history) == NULL)
		return ;
	while ((index = 1) && his->prev != NULL)
		his = his->prev;
	while (index < n)
	{
		his = his->next;
		++index;
		if (his == NULL)
			return ;
	}
	if (his->cmd == NULL)
		return ;
	del = his;
	if (access_le_main_datas()->history == del)
		reset_history_on_first_elem(access_le_main_datas());
	if (his->prev != NULL)
		his->prev->next = his->next;
	his->next->prev = his->prev;
	free(del->cmd);
	free(del);
}

/*
**	Replace the previous saved history with the current one.
**	The history is saved into a file.
**	Return 'false' if an error occurs.
*/

bool	blthis_save_history_in_file(struct s_history *his)
{
	int		fd;

	if (check_backup_file(get_parsed_history_file_path()) == false)
		return (false);
	if (his == NULL)
		return (true);
	while (his->prev != NULL)
		his = his->prev;
	if ((fd = open(get_parsed_history_file_path(), O_WRONLY | O_TRUNC)) == -1)
	{
		ft_putstr_fd("42sh: error with file .42sh_history\n", STDERR_FILENO);
		return (false);
	}
	while (his->cmd != NULL)
	{
		if (write(fd, his->cmd, ft_strlen(his->cmd)) == (ssize_t)(-1)
		|| write(fd, "\n", sizeof(char)) == (ssize_t)(-1))
		{
			ft_putstr_fd(".42sh_history: error writing in file\n", 2);
			return (close(fd) ? (false) : (false));
		}
		his = his->next;
	}
	close(fd);
	return (true);
}

/*
**	Empty the history list (not the file).
*/

void	blthis_clear_history(struct s_line *le)
{
	struct s_history	*his;
	struct s_history	*del;

	his = le->history;
	if (his == NULL)
		return ;
	while (his->prev != NULL)
		his = his->prev;
	while (his->cmd != NULL)
	{
		free(his->cmd);
		del = his;
		his = his->next;
		free(del);
	}
	his->prev = NULL;
	his->next = NULL;
}

/*
**	Print the 'n' last elements of the history list.
*/

void	blthis_print_n_last_elem(struct s_line *le, unsigned int n)
{
	struct s_history	*his;
	unsigned int		index;

	his = le->history;
	while (his->prev != NULL)
		his = his->prev;
	index = 0;
	while (his->cmd != NULL)
	{
		his = his->next;
		++index;
	}
	if (n > index)
		return ;
	while (n > 0)
	{
		his = his->prev;
		--n;
	}
	while (his->cmd != NULL)
	{
		ft_putstr(his->cmd);
		ft_putchar('\n');
		his = his->next;
	}
}
