/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebastien <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 16:36:10 by sebastien         #+#    #+#             */
/*   Updated: 2018/10/01 11:46:09 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

static int	is_bypass_token(t_ast *node)
{
	int		i;

	i = 0;
	while (g_token_bypass[i])
	{
		if (node->type_details == g_token_bypass[i])
			return (1);
		i++;
	}
	return (0);
}

static int	manage_heredoc(t_lexeme *lex)
{
	if (lex->type_details == TK_DLESS)
		return (1);
	return (0);
}

int			put_node(t_lexeme **lex, t_ast **root, t_ast *new, \
			void(*const node_placer)(t_ast *, t_ast *))
{
	int		flag_heredoc_eof;

	flag_heredoc_eof = manage_heredoc(*lex);
	if (!is_bypass_token(new))
	{
		log_trace("Put_node: %s", new->data[0]);
		node_placer(*root, new);
		*root = new;
	}
	else
		log_trace("BYPASS Put_node: %s", new->data[0]);
	if (lex[0]->type == T_WORD && !is_bypass_token(new))
		while (*lex && lex[0]->type == T_WORD)
			*lex = lex[0]->next;
	else
		*lex = lex[0]->next;
	if (is_bypass_token(new))
		free_node(&new);
	return (flag_heredoc_eof);
}
