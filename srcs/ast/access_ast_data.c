/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   access_ast_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbrucker <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/29 15:05:02 by sbrucker          #+#    #+#             */
/*   Updated: 2018/09/29 15:16:40 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

void	link_ast_data(t_ast *new_ast_root)
{
	t_ast	**ast_root;

	ast_root = access_ast_data();
	*ast_root = new_ast_root;
}

t_ast	**access_ast_data(void)
{
	static t_ast	*ast_root = NULL;

	return (&ast_root);
}
