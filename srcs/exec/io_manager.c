/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_manager.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/26 10:56:09 by sbrucker          #+#    #+#             */
/*   Updated: 2018/08/20 18:43:59 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Skip T_REDIR_OPT nodes and look for next T_CTRL_OPT node
*/

static t_ast	*get_next_ctrl_opt(t_ast *node)
{
	t_ast	*ptr;

	ptr = node;
	while (ptr && ptr->type != T_CTRL_OPT && ptr->type_details != TK_PIPE)
		ptr = ptr->parent;
	return (ptr);
}

/*
** Recursively search for $ptr_cmp in ast, beginning at $pos
*/

static int		search_node(t_ast *pos, t_ast *ptr_cmp)
{
	if (!pos)
		return (0);
	if (pos == ptr_cmp)
		return (1);
	if (pos->left)
		return (search_node(pos->left, ptr_cmp));
	if (pos->right)
		return (search_node(pos->right, ptr_cmp));
	return (0);
}

/*
** Handle every T_CTRL_OPT node
*/

static void	io_ctrl_opt(t_ast *node, t_ast *next_ctrl_opt, t_exec *exe)
{

	log_trace("Updating ready_for_exec - exe->ret : %d", exe->ret);
	if (search_node(next_ctrl_opt->left, node))
		return ;
	else if ((next_ctrl_opt->type_details == TK_DAND && exe->ret != 0) || \
		(next_ctrl_opt->type_details == TK_OR && exe->ret == 0))
		exe->ready_for_exec = 1;
	else
		exe->ready_for_exec = 0;
}

/*
** Handle every operator.
** Is called in the in_exec() function, at the second passage in the node AST
** Checks if exe->ready_for_exec should be set to 1
** to skip next program execution
*/

void		io_manager_in(t_ast *node, t_exec *exe)
{
	io_ctrl_opt(node, get_next_ctrl_opt(node), exe);
}
