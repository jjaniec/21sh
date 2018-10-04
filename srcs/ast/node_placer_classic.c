/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_placer_classic.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbrucker <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/10 12:39:26 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/01 14:44:52 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Get the right level of a node, 0 is being high priority, 4 low priority
*/

int		lvl_node(t_ast *ast)
{
	int		lvl;

	lvl = 5;
	if (is_nodeop0(ast))
		lvl = 0;
	else if (is_nodeop1(ast))
		lvl = 1;
	else if (is_nodeop1_5(ast))
		lvl = 2;
	else if (is_nodeop2(ast))
		lvl = 3;
	else if (is_nodeop3(ast))
		lvl = 4;
	return (lvl);
}

/*
** Place the node *new in the AST, from the node *root
*/

void	node_placer_classic(t_ast *root, t_ast *new)
{
	int		lvl_new;

	lvl_new = lvl_node(new);
	if (lvl_new > lvl_node(root))
	{
		if (root->right)
			new->left = root->right;
		root->right = new;
		new->parent = root;
	}
	else
	{
		while (root->parent && lvl_new <= lvl_node(root->parent))
			root = root->parent;
		if (root->parent)
			root->parent->right = new;
		new->left = root;
		if (root->parent && lvl_node(root->parent) < lvl_new)
			new->parent = root->parent;
		root->parent = new;
	}
}
