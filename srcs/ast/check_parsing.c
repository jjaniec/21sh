/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbrucker <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 15:25:36 by sbrucker          #+#    #+#             */
/*   Updated: 2018/09/29 13:33:38 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Check if lex1 and lex2 can be neighbour on the commandline
*/

static int	check_lexeme(t_lexeme *lex1, t_lexeme *lex2)
{
	if (lex2 && lex1->type_details == TK_DAND && lex2->type_details == TK_NEWLINE)
		return (NEED_SUBPROMPT_DAND);
	else if (lex2 && lex1->type_details == TK_OR && lex2->type_details == TK_NEWLINE)
		return (NEED_SUBPROMPT_OR);
	else if (lex2 && lex1->type_details == TK_PIPE && lex2->type_details == TK_NEWLINE)
		return (NEED_SUBPROMPT_PIPE);
	else if (lex2 && (lvl_lex(lex1) != 5) && (lvl_lex(lex2) != 5) && lex2->type_details != TK_NEWLINE)
		return (0);
	else if (is_op3(lex1) && lex2->type != T_WORD)
		return (0);
	return (1);
}

/*
** Main function for checking if the commandline is acceptable by its syntax
** If there is an error, make the ast() function return NULL to the main()
*/

int			check_parsing(t_lexeme *lex)
{
	int		need_subpropmt;

	if (!lex || lex->type_details == TK_NEWLINE)
		return (1);
	if (lex->type != T_WORD && lex->type != T_ENV_ASSIGN)
	{
		ft_printf("Parsing error just after: %s\n", lex->data);
		return (0);
	}
	lex = lex->next;
	if (!lex)
		return (NEED_SUBPROMPT_NEWLINE);
	while (lex)
	{
		if (lvl_lex(lex) == 4 && !ft_strchr(lex->next->data, '\n'))
		{
			subp_heredoc(lex, lex->next->data);
			if (!ft_strchr(lex->next->data, '\n'))
				lex->next->data = ft_strjoin_free(lex->next->data, ft_strdup("\n"));
		}
		if (!lex->next && lex->type_details != TK_NEWLINE)
			return (NEED_SUBPROMPT_NEWLINE);
		if (!check_lexeme(lex, lex->next))
		{
			ft_printf("Parsing error just after: %s\n", lex->data);
			return (0);
		}
		else if ((need_subpropmt = check_lexeme(lex, lex->next)) < 0)
			return (need_subpropmt);
		lex = lex->next;
	}
	return (1);
}
