/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 15:22:08 by sbrucker          #+#    #+#             */
/*   Updated: 2018/08/18 17:15:49 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Malloc and create an AST node
*/

t_ast	*create_node(size_t type, size_t type_details, char **data)
{
	t_ast	*node;

	node = (t_ast *)ft_memalloc(sizeof(t_ast));
	if (!node)
		exit(MALLOC_ERROR);
	node->type = type;
	node->type_details = type_details;
	node->data = data;
	return (node);
}

/*
** Main function of AST.
** Return NULL if there is a problem during the check / construction
*/

t_ast	*ast(t_lexeme *lex)
{
	t_ast	*root;

	if (!lex)
		return (NULL);
	root = NULL;
	if (!check_parsing(lex))
		return (NULL);
	root = create_node(T_CTRL_OPT, TK_SEMICOLON, NULL);
	root = construct_ast(lex, root);
	if (VERBOSE_MODE)
		ast_debug(root);
	return (root);
}
