/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sub_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbrucker <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/23 14:59:17 by sbrucker          #+#    #+#             */
/*   Updated: 2018/08/19 17:20:19 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

void	subp_string(char **s)
{
	char	*new;
	char	*input;

	ft_putstr("> ");
	input = line_edition();
	ft_putchar('\n');
	new = ft_strjoin(*s, input);
	*s = new;
}

t_lexeme	*subp_lexeme(t_lexeme *lex)
{
	char		*input;
	t_lexeme	*new;
	t_lexeme	*save;

	ft_putstr("> ");
	input = line_edition();
	ft_putchar('\n');
	new = lexer(input);
	if (!lex)
		return (new);
	save = lex;
	while (lex->next && lex->next->next)
		lex = lex->next;
	if (lex->next && lex->next->type_details == TK_NEWLINE)
	{
		free(lex->next->data);
		free(lex->next);
		lex->next = new;
	}
	else if (lex->next && lex->next->type_details != TK_NEWLINE)
	{
		lex->next->next = new;
	}
	else if (lex->next && !lex->next->next && lex->next->type == T_WORD && new->type == T_WORD)
	{
		lex->next->data = ft_strjoin(lex->next->data, new->data);
		lex->next->next = new->next;
	}
	else if (lex && !lex->next && lex->type == T_WORD && new->type == T_WORD)
	{
		lex->data = ft_strjoin(lex->data, new->data);
		lex->next = new->next;
	}
	else
		lex->next = new;
	return (save);
}

void	subp_heredoc(t_lexeme *lex, char *eof_word)
{
	char	*input;
	char	*final;

	input = NULL;
	final = (char *)ft_memalloc(sizeof(char));
	if (!final)
		exit(MALLOC_ERROR);
	eof_word = ft_strjoin(eof_word, "\n");
	while (!input)
	{
		ft_putstr("heredoc> ");
		input = line_edition();
		ft_putchar('\n');
		if (ft_strequ(input, eof_word))
			break ;
		final = ft_strjoin(final, input);
		free(input);
		input = NULL;
	}
	lex->next->data = final;
}
