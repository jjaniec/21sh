/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_last_pipe_node.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 18:24:16 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/17 15:18:35 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Look for pipes in ast before && and || tokens,
** if found, return pointer to node,
** otherwise return NULL
*/

t_ast		*get_last_pipe_node(t_ast *node)
{
	t_ast	*ptr;

	ptr = node;
	while (ptr && ptr->parent && ptr->type != T_CTRL_OPT)
		ptr = ptr->parent;
	if (ptr && ptr->type_details == TK_PIPE)
		return (ptr);
	return (NULL);
}
