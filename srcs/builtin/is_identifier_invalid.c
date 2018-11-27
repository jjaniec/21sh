/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_identifier_invalid.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 15:13:34 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/14 19:51:50 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Check if variable identifier is valid and does not contain
** any forbidden characters like '$'
*/

int		is_identifier_invalid(char *str, char *assign_ptr)
{
	char	*identifier_begin;

	if (!(str) || (*str >= '0' && *str <= '9'))
		return (1);
	identifier_begin = str;
	while (*identifier_begin && \
		identifier_begin != assign_ptr && \
		!ft_strchr(IFS""EXPANSIONS_SPECIFIERS, *identifier_begin))
	{
		identifier_begin++;
	}
	if ((assign_ptr && identifier_begin == assign_ptr && assign_ptr != str) || \
		(!assign_ptr && !(*identifier_begin)))
		return (0);
	return (1);
}
