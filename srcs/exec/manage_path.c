/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 11:08:25 by sbrucker          #+#    #+#             */
/*   Updated: 2018/08/09 16:41:07 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Take in a path (like "/usr/bin") and a name (like "open").
** Get back a full path (like "/usr/bin/open")
** Allocate a new string.
** In case of failure, exit with MALLOC_ERROR
** If *s1 or *s2 is NULL, return NULL
*/

char	*new_path(char *s1, char *s2)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	str = NULL;
	if (s1[ft_strlen(s1) - 1] == '/' && s2[0] == '/')
		asprintf(&str, "%s%s", s1, s2 + 1);
	else if (s1[ft_strlen(s1) - 1] != '/' && s2[0] != '/')
		asprintf(&str, "%s/%s", s1, s2);
	else
		asprintf(&str, "%s%s", s1, s2);
	if (!str)
		exit(MALLOC_ERROR);
	return (str);
}

/*
** Cut the environment var PATH in an allocated char**.
** In case of failure, return NULL
*/

char	**get_path(char *str)
{
	if (!str)
		return (NULL);
	return (ft_strsplit(str, ':'));
}

/*
** Take in all paths from the environment var PATH in a char**
** and a commande name.
** Look for the rigth place of the correspondig file, and for execution rights.
** If found, return the full path corresponding in an allocated char*
** Allocation made by new_path()
** If not found, return NULL
*/

char	*isin_path(char **paths, char *cmd)
{
	int		i;
	char	*pth;

	i = 0;
	while (paths && paths[i])
	{
		pth = new_path(paths[i], cmd);
		if (access(pth, F_OK) == 0 && access(pth, X_OK) == 0)
			return (pth);
		ft_strdel(&pth);
		i++;
	}
	ft_putstr_fd("21sh: no such file or directory: ", 2);
	ft_putendl_fd(cmd, 2);
	return (NULL);
}
