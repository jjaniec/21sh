/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/02 11:22:15 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/30 18:09:59 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Concat a string with a '/' between our two parameters
*/

char	*ft_strjoin_path(char *path1, char *path2)
{
	char	*r;
	size_t	path1_len;
	size_t	path2_len;

	if (!((path1 && *path1) && (path2 && *path2)))
		return (NULL);
	path1_len = ft_strlen(path1);
	path2_len = ft_strlen(path2);
	if (path1_len + path2_len + 1 >= MAX_ENV_ENTRY_LEN)
		log_error("Oops it seems that your path is too long for our env \
		optimization, update includes/twenty_one_sh.h:MAX_ENV_ENTRY_LEN");
	r = ft_xmalloc(sizeof(char) * (path1_len + path2_len + 2));
	ft_strcpy(r, path1);
	*(r + (sizeof(char) * (path1_len))) = '/';
	ft_strcpy(r + (path1_len + 1) * sizeof(char), path2);
	*(r + (sizeof(char) * (path1_len + path2_len + 1))) = '\0';
	return (r);
}
