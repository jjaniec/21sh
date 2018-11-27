/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_assigns_status.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 20:29:09 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/12 17:30:49 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Controls value of the env_assigns_passed variable,
**
** the goal of this variable is to tell whether or not
** we can set T_WORDs lexemes containing a '=' as 'T_ENV_ASSIGN';
** which set a value for an environnement variable when starting our program
**   example: 'LS_COLORS= ls -G' clears the LS_COLORS env variable before
**   launching 'ls'
**
** Once all environnements variable assignations are skipped in our string,
** we'll set this variable to '1' to stop creating T_ENV_ASSIGN elements
** for strings with the '=' char but instead create it as T_WORDs
** (aka command name or arguments)
**
** if mode_set is passed as a value != 0, this function act as a 'setter'
** and update the value of env_assigns_passed to mode_set_value,
** otherwise it act as a 'getter' to only return value of the variable
**
** We could have done this behavior with a global variable or only a pointer
** but our coding norm does not allow globals and functions
** with more than 4 functions parameters
*/

t_acocat	env_assigns_status(int mode_set, int new_value)
{
	static int		env_assigns_passed = (t_acocat)0;

	if (mode_set)
		env_assigns_passed = new_value;
	return (env_assigns_passed);
}
