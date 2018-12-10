/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_parsed_backup_files_path.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/07 17:14:37 by cyfermie          #+#    #+#             */
/*   Updated: 2018/12/10 21:50:05 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

char	*get_parsed_history_file_path(void)
{
	static bool		already_done = false;
	static char		*parsed_path = NULL;

	if (!already_done)
	{
		already_done = true;
		parsed_path = ft_xstrdup(HISTORY_FILE_PATH);
		handle_quotes_expansions(&parsed_path);
	}
	return (parsed_path);
}

char	*get_parsed_aliases_file_path(void)
{
	static bool		already_done = false;
	static char		*parsed_path = NULL;

	if (!already_done)
	{
		already_done = true;
		parsed_path = ft_xstrdup(ALIASES_FILE_PATH);
		handle_quotes_expansions(&parsed_path);
	}
	return (parsed_path);
}
