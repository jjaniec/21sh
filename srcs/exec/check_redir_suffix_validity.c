/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_redir_suffix_validity.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/15 14:57:48 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/15 16:58:18 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Check if node data contains invalid characters depending on redir type
*/

static bool		is_redir_suffix_valid(char *node_data, int mode, t_ast *node)
{
	log_debug("Check validity of: |%s|", node_data);
	while (*node_data && ft_isprint(*node_data) && \
		!ft_strchr(REDIRS_AMBIGUOUS_CHARS, *node_data) && \
		*node_data != CLOSE_FD_REDIR_SYMBOL)
	{
		if ((node->type_details == TK_LESSAND || \
			node->type_details == TK_GREATAND) && \
			ft_isalpha(*node_data))
			node->type_details = \
				(node->type_details == TK_LESSAND) ? (TK_LESS) : (TK_GREAT);
		node_data++;
	}
	log_debug("Check validity of: |%s| end of while", node_data);
	if (mode == MODE_CHECK_REDIR_VALID_FD && \
		*node_data == CLOSE_FD_REDIR_SYMBOL && \
		(node->type_details == TK_LESSAND || \
		node->type_details == TK_GREATAND))
		node_data++;
	if (!(*node_data))
		return (true);
	return (false);
}

/*
** Expanse & remove quotes from suffix redir data string and search for
** invalid characters
*/

int				check_redir_suffix_validity(t_ast *redir_ast_node)
{
	handle_quotes_expansions(&(redir_ast_node->right->data[0]));
	if (!is_redir_suffix_valid(redir_ast_node->right->data[0], \
		(redir_ast_node->type_details == TK_LESSAND || \
		redir_ast_node->type_details == TK_GREATAND) ? \
		(MODE_CHECK_REDIR_VALID_FD) : (MODE_CHECK_REDIR_VALID_FILE), redir_ast_node))
	{
		ft_putstr_fd(SH_NAME": ", 2);
		ft_putstr_fd(redir_ast_node->right->data[0], 2);
		ft_putstr_fd(": "ERR_AMBIGUOUS_REDIR, 2);
		return (1);
	}
	return (0);
}
