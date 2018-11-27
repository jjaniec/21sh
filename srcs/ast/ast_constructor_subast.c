/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_constructor_subast.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 16:42:45 by sebastien         #+#    #+#             */
/*   Updated: 2018/11/19 19:01:32 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

static char		**debug_data_node(char *str)
{
	char	**tab_;

	tab_ = (char **)ft_xmalloc(sizeof(char *) * 2);
	tab_[0] = ft_xstrdup(str);
	tab_[1] = NULL;
	return (tab_);
}

/*
** Take t_ast **root such as root[0]->data[0] == "[sub_ast]"
** Doesnt't change *root, but change root[0]->sub_ast
*/

static t_lexeme	*create_sub_ast(t_lexeme *lex, t_ast **root, \
				const int next_tokens[],
				void (*const node_placer)(t_ast *, t_ast *))
{
	t_lexeme	*end_lexeme;

	end_lexeme = find_end_lexeme(lex, next_tokens);
	if (!end_lexeme)
		return (NULL);
	if (lex->type == T_WORD)
		while (lex && lex->type == T_WORD)
			lex = lex->next;
	else
		lex = lex->next;
	root[0]->sub_ast = ast_constructor(&lex, root[0]->sub_ast, end_lexeme, node_placer);
	root[0]->sub_ast->top_ast = root[0];
	return (lex);
}

t_lexeme		*need_subast(t_lexeme *lex, t_ast **root, t_ast *new, \
				void (*const node_placer)(t_ast *, t_ast *))
{
	int			i;
	t_ast		*node_subast;
	t_lexeme	*save;

	i = 0;
	while (g_tokens[i])
	{
		if (new->type_details == g_tokens[i])
		{
			node_subast = create_node(T_WORD, TK_SUBAST, \
				debug_data_node("[subast]"));
			node_placer(*root, node_subast);
			*root = node_subast;
			root[0]->sub_ast = create_node(T_CTRL_OPT, TK_SEMICOLON, NULL);
			save = lex;
			put_node(&save, &(root[0]->sub_ast), new, g_node_placer[i]);
			return (create_sub_ast(lex, root, g_next_tokens[i], \
				g_node_placer[i]));
		}
		i++;
	}
	return (lex);
}
