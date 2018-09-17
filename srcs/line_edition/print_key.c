/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_key.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 14:42:58 by cfermier          #+#    #+#             */
/*   Updated: 2018/09/17 13:45:46 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
**	Print a key in the terminal's window.
*/

void    print_key(t_kno key)
{
    if (write(STDOUT_FILENO, &key, sizeof(char)) != sizeof(char))
		  le_exit("Writing on stdout failed\n", "write", errno);
}
