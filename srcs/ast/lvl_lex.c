/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lvl_lex.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 18:52:33 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/01 14:44:23 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Get the right level of a lexeme, 0 is being high priority, 4 low priority
*/

int				lvl_lex(t_lexeme *lex)
{
	int		lvl;

	lvl = 5;
	if (is_op0(lex))
		lvl = 0;
	else if (is_op1(lex))
		lvl = 1;
	else if (is_op1_5(lex))
		lvl = 2;
	else if (is_op2(lex))
		lvl = 3;
	else if (is_op3(lex))
		lvl = 4;
	return (lvl);
}
