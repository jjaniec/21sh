/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh_colosyn.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 15:36:29 by cyfermie          #+#    #+#             */
/*   Updated: 2018/09/29 14:11:23 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
**	The command line is checked in order to reprint it with the good colors.
*/

void		refresh_colosyn(struct s_line *le, char *cmd)
{
	t_lexeme	*lexemes;
	char		*unmatched_quote_err_ptr;

	//fprintf(tty_debug, "UPDATED |%s|\n", cmd);
	lexer(cmd, &lexemes, &unmatched_quote_err_ptr);
	actionk_delete_current_input(le);
	print_colorized_input(cmd, g_envp, lexemes, unmatched_quote_err_ptr);
	free_lexemes(lexemes);
}
