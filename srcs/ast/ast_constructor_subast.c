/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_constructor_subast.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/25 16:42:45 by sebastien         #+#    #+#             */
/*   Updated: 2018/10/01 11:58:50 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

static char		**debug_data_node(char *str)
{
	char	**tab_;

	tab_ = (char **)malloc(sizeof(char *) * 2);
	if (!tab_)
		exit(MALLOC_ERROR);
	tab_[0] = ft_strdup(str);
	tab_[1] = NULL;
	return (tab_);
}

static t_lexeme	*create_sub_ast(t_lexeme *lex, t_ast **root, \
				const int next_tokens[],
				void (*const node_placer)(t_ast *, t_ast *))
{
	t_lexeme	*end_lexeme;

	end_lexeme = find_end_lexeme(lex, next_tokens);
	if (!end_lexeme)
		return (NULL);
	lex = lex->next;
	if (lex == end_lexeme)
		log_warn("Find end lexeme: end_lexeme is the same than start.");
	*root = ast_constructor(&lex, *root, end_lexeme, node_placer);
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
			while (lex && lex->type == T_WORD)
				lex = lex->next;
			return (create_sub_ast(lex, &(root[0]->sub_ast), g_next_tokens[i], \
				g_node_placer[i]));
		}
		i++;
	}
	return (lex);
}
