/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 11:08:25 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/05 19:35:30 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Take in a path (like "/usr/bin") and a name (like "open").
** Get back a full path (like "/usr/bin/open")
** Allocate a new string.
** In case of failure, exit with MALLOC_ERROR
** If *s1 or *s2 is NULL, return NULL
*/

static char	*new_path(char *s1, char *s2)
{
	return (ft_strjoin_path(s1, s2));
}

/*
** Take in all paths from the environment var PATH in a char**
** and a commande name.
** Look for the rigth place of the correspondig file, and for execution rights.
** If found, return the full path corresponding in an allocated char*
** Allocation made by new_path()
** If not found, return NULL
*/

char	*isin_path(char *path_entry, char *cmd)
{
	int		i;
	char	*prog_path;
	char	**paths;

	if (!path_entry || !(paths = ft_strsplit(path_entry, ':')))
		return (NULL);
	i = 0;
	while (paths[i])
	{
		prog_path = new_path(paths[i], cmd);
		if (access(prog_path, F_OK) == 0)
		{
			ft_free_argv(paths);
			return (prog_path);
		}
		ft_strdel(&prog_path);
		i++;
	}
	ft_free_argv(paths);
	return (NULL);
}
