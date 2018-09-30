/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoc_check_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 10:19:05 by cgaspart          #+#    #+#             */
/*   Updated: 2018/09/18 10:19:08 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

char	autoc_check_path(char *dirname)
{
	struct stat fstat;

	if (lstat(dirname, &fstat) == -1)
		return (0);
	if (S_ISDIR(fstat.st_mode))
		return ('d');
	else if (S_ISLNK(fstat.st_mode))
		return ('l');
	else if (S_ISSOCK(fstat.st_mode))
		return ('s');
	else if (S_ISFIFO(fstat.st_mode))
		return ('p');
	else if (S_ISCHR(fstat.st_mode))
		return ('c');
	else if (S_ISBLK(fstat.st_mode))
		return ('b');
	else if (S_ISREG(fstat.st_mode))
		return ('-');
	return (1);
}
