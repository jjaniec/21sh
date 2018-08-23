/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbrucker <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 15:25:36 by sbrucker          #+#    #+#             */
/*   Updated: 2018/08/20 15:21:30 by sebastien        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Check if lex1 and lex2 can be neighbour on the commandline
*/

static int	check_lexeme(t_lexeme *lex1, t_lexeme *lex2)
{
	if (lex2 && (is_op1(lex1) || is_op1_5(lex1)) && lex2->type_details == TK_NEWLINE)
		return (NEED_SUBPROMPT);
	else if (lex2 && (lvl_lex(lex1) != 5) && (lvl_lex(lex2) != 5) && lex2->type_details != TK_NEWLINE)
		return (0);
	return (1);
}

/*
** Main function for checking if the commandline is acceptable by its syntax
** If there is an error, make the ast() function return NULL to the main()
*/

int			check_parsing(t_lexeme *lex)
{
	if (!lex || lex->type_details == TK_NEWLINE)
		return (1);
	if (lex->type != T_WORD && lex->type != T_ENV_ASSIGN)
	{
		ft_printf("Parsing error just after: %s\n", lex->data);
		return (0);
	}
	lex = lex->next;
	if (!lex)
		return (NEED_SUBPROMPT);
	while (lex)
	{
		if (!lex->next && lex->type_details != TK_NEWLINE)
			return (NEED_SUBPROMPT);
		if (!check_lexeme(lex, lex->next))
		{
			ft_printf("Parsing error just after: %s\n", lex->data);
			return (0);
		}
		else if (check_lexeme(lex, lex->next) == NEED_SUBPROMPT)
			return (NEED_SUBPROMPT);
		lex = lex->next;
	}
	return (1);
}
