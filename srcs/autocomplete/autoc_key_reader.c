/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoc_key_reader.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 16:46:17 by cgaspart          #+#    #+#             */
/*   Updated: 2018/09/10 16:46:19 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

int		autoc_key_reader(t_autoc *autoc)
{
	char		buffer[3];

	if (read(1, buffer, 3))
	{
		if (((autoc->key_function)[(int)buffer[0]])(buffer, autoc))
			return (0);
	}
	return (1);
}
