/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbrucker <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 15:25:36 by sbrucker          #+#    #+#             */
/*   Updated: 2018/07/19 15:08:58 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Check if lex1 and lex2 can be neighbour on the commandline
*/

static int	check_lexeme(t_lexeme *lex1, t_lexeme *lex2)
{
	if (!lex2 && (is_op3(lex1) || is_op2(lex1) || is_op1(lex1)))
		return (0);
	else if (lex2 && (is_op0(lex1) || is_op1(lex1) || is_op2(lex1))
			&& (is_op0(lex2) || is_op1(lex2) || is_op2(lex2)))
		return (0);
	return (1);
}

/*
** Main function for checking if the commandline is acceptable by its syntax
** If there is an error, make the ast() function return NULL to the main()
*/

int			check_parsing(t_lexeme *lex)
{
	if (!lex)
		return (0);
	if (lex->type != T_WORD && lex->type != T_ENV_ASSIGN && !is_op3(lex))
	{
		ft_printf("Parsing error just after |%s|", lex->data);
		return (0);
	}
	lex = lex->next;
	while (lex)
	{
		if (!check_lexeme(lex, lex->next))
		{
			ft_printf("Parsing error just after |%s|", lex->data);
			return (0);
		}
		lex = lex->next;
	}
	return (1);
}
