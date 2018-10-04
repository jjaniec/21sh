/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 15:22:08 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/04 12:21:30 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Main function of AST.
** Return NULL if there is a problem during the check / construction
*/

t_ast	*ast(t_lexeme *lex)
{
	t_ast	*root;
	t_lexeme	*error;
	int		check;

	error = NULL;
	root = NULL;
	check = NEED_SUBPROMPT_NEWLINE;
	while (check < 0)
	{
		if (lex)
			check = check_parsing(lex, &error);
		if (check < 0 && is_option_activated("c", g_sh_opts, NULL))
		{
			ft_putendl_fd("Non-interactive mode: unexpected end of file, exiting.", 2);
			return (NULL);
		}
		else if (check < 0)
			lex = subp_lexeme(lex, check);
		else if (check == 0)
		{
			ft_putstr_fd("21sh: syntax error near unexpected token '", 2);
			if (((char *)error->data)[0] == '\n')
				ft_putstr_fd("newline", 2);
			else
				ft_putstr_fd(error->data, 2);
			ft_putendl_fd("'", 2);
			return (NULL);
		}
	}
	if (!is_script_complete(lex, lex, 0))
	{
		ft_putendl_fd("There is an error in your script.", 1);
		return (NULL);
	}
	root = create_node(T_CTRL_OPT, TK_SEMICOLON, NULL);
	root = ast_constructor(&lex, root, NULL, &node_placer_classic);
	if (!root)
		ft_putendl_fd("There is an error in your script.", 1);
	if (VERBOSE_MODE)
		ast_debug(root);
	return (root);
}
