/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_storage.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/11 16:19:43 by cfermier          #+#    #+#             */
/*   Updated: 2018/09/13 20:25:15 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

void		*ft_realloc(void *ptr, size_t original_size, size_t new_size)
{
	void	*new;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	else if (ptr == NULL)
		return (malloc(new_size));
	if ((new = malloc(new_size)) == NULL)
		return (NULL);
	if (new_size < original_size)
		ft_memcpy(new, ptr, new_size);
	else
		ft_memcpy(new, ptr, original_size);
	free(ptr);
	return (new);
}

/*
**	Check if there is enough space in the shell's clipboard buffer
**	to store 'nb_char' more chatacters in it.
**	If no, the shell's clipboard buffer is reallocated by multiplying
**	the buffer's size by 2, and as many times as necessary.
*/

void	check_clipboard_storage(struct s_line *le, unsigned int nb_char)
{
	void	*tmp_realloc;

	while (le->clipboard_len + nb_char + 1 >= le->clipboard_size)
	{
		//fprintf(tty_debug, "REALLOCATION DE LA MEMOIRE DE L\'ORDINATEUR clipboard\n");
		tmp_realloc = ft_realloc(le->clipboard, le->clipboard_size, \
		le->clipboard_size * 2);
		if (tmp_realloc == NULL)
			le_exit("Memory allocation failed\n", "malloc", errno);
		le->clipboard = tmp_realloc;
		le->clipboard_size *= 2;
		ft_memset(le->clipboard + le->clipboard_len, '\0', \
		le->clipboard_size - le->clipboard_len);
	}
}

/*
**	Check if there is enough space in the command line's buffer
**	to store 'nb_char' more chatacters in it.
**	If no, the command line's buffer is reallocated by multiplying
**	the buffer's size by 2, and as many times as necessary.
*/

void	check_cmd_storage(struct s_line *le, unsigned int nb_char)
{
	void	*tmp_realloc;

	while (le->line_index + nb_char + 1 >= le->line_size)
	{
		//fprintf(tty_debug, "REALLOCATION DE LA MEMOIRE DE L\'ORDINATEUR\n");
		tmp_realloc = ft_realloc(le->line, le->line_size, le->line_size * 2);
		if (tmp_realloc == NULL)
			le_exit("Memory allocation failed\n", "malloc", errno);
		le->line = tmp_realloc;
		le->line_size *= 2;
		ft_memset(le->line + le->line_index, '\0', \
		le->line_size - le->line_index);
	}
}
