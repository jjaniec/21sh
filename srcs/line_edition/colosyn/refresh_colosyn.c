/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh_colosyn.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 15:36:29 by cyfermie          #+#    #+#             */
/*   Updated: 2018/09/29 13:18:46 by sbrucker         ###   ########.fr       */
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

	log_set_quiet(1);
	lexer(cmd, &lexemes, &unmatched_quote_err_ptr);
	if (VERBOSE_MODE)
		log_set_quiet(0);
	actionk_delete_current_input(le);
	//fprintf(tty_debug, "UPDATED |%s|\n", cmd);
	print_colorized_input(cmd, g_envp, lexemes, unmatched_quote_err_ptr);
}
