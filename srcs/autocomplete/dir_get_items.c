/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoc_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/30 21:27:30 by cgaspart          #+#    #+#             */
/*   Updated: 2018/09/25 16:01:29 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

static int			nbr_tab(char *dirname)
{
	int				i;
	DIR				*dir;
	struct dirent	*file;

	i = 0;
	dir = opendir(dirname);
	while ((file = readdir(dir)))
	{
		if (file->d_name[0] != '.')
			i++;
	}
	closedir(dir);
	return (i);
}

static char			*cp_item(char *in, char *item)
{
	char *res;
	char *tmp;

	tmp = ft_strjoin(in, item);
	if (autoc_check_path(tmp) == 'd' || autoc_check_path(tmp) == 'l')
		res = ft_strjoin(item, "/");
	else
		res = ft_strdup(item);
	free(tmp);
	return (res);
}

char				**dir_get_items(char *in)
{
	DIR				*dir;
	struct dirent	*file;
	char			**res;
	int				i;

	dir = opendir(in);
	res = (char**)malloc(sizeof(char*) * nbr_tab(in) + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while ((file = readdir(dir)))
	{
		if (file->d_name[0] != '.')
		{
			res[i] = cp_item(in, file->d_name);
			i++;
		}
	}
	res[i] = NULL;
	closedir(dir);
	return (order_tab_ascii(res));
}
