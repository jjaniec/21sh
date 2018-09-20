/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script_lexer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sebastien <marvin@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/24 21:07:42 by sebastien         #+#    #+#             */
/*   Updated: 2018/09/20 19:32:38 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Defines if the current lexeme with associated token is a good start \
** after the IF or ELIF or WHILE (for a T_SCRIPT_CONDITION)
*/

static int		good_start(t_lexeme *lex)
{
	int		i;

	i = 0;
	while (g_cant_begin_with[i])
	{
		if (lex->type_details == g_cant_begin_with[i])
			return (0);
		i++;
	}
	return (1);
}

/*
** Defines the next lexeme type and type_details.
** Then check if it can be a T_SCRIPT_CONDITION, otherwise \
** change it to a TK_SCRIPT_FI (for a parsing error every time)
*/

static t_lexeme		*next_lex_condition(t_lexeme *lex, size_t token)
{
	if (lex->next)
		lex->next = is_keyword(lex->next);
	else
		return (lex);
	if (good_start(lex->next) && lex->next->type == T_WORD)
	{
		lex->next->type = T_SCRIPT_CONDITION;
		lex->next->type_details = token;
		log_info("Update elem w/ data |%s| - type : %zu", lex->data, lex->type);
	}
	else if (!is_op0(lex->next))
		lex->next->type_details = TK_SCRIPT_FI;
	return (lex->next);
}

/*
** Defines the lexeme type and type_details.
*/

static t_lexeme	*associate_token(t_lexeme *lex, const char *original, \
				size_t type, size_t type_details)
{
	char	*word;

	word = ft_str_capitalize(ft_strdup(lex->data));
	if (ft_strequ(original, word))
	{
		lex->type = type;
		lex->type_details = type_details;
		if (lex->type_details == TK_SCRIPT_IF \
		|| lex->type_details == TK_SCRIPT_ELIF)
			lex = next_lex_condition(lex, TK_SCRIPT_CONDITION_IF);
		else if (lex->type_details == TK_SCRIPT_WHILE)
			lex = next_lex_condition(lex, TK_SCRIPT_CONDITION_WHILE);
	}
	free(word);
	return (lex);
}

/*
** Defines the lexeme type and type_details.
*/

t_lexeme	*is_keyword(t_lexeme *lex)
{
	if (lex->next)
		lex = associate_token(lex, "IF", T_SCRIPT_LOGICAL, TK_SCRIPT_IF);
	if (lex->next)
		lex = associate_token(lex, "ELIF", T_SCRIPT_LOGICAL, TK_SCRIPT_ELIF);
	lex = associate_token(lex, "ELSE", T_SCRIPT_LOGICAL, TK_SCRIPT_ELSE);
	if (lex->next)
		lex = associate_token(lex, "WHILE", T_SCRIPT_LOGICAL, TK_SCRIPT_WHILE);
	lex = associate_token(lex, "THEN", T_SCRIPT_CONTAINER, TK_SCRIPT_THEN);
	lex = associate_token(lex, "FI", T_SCRIPT_CONTAINER, TK_SCRIPT_FI);
	lex = associate_token(lex, "DO", T_SCRIPT_CONTAINER, TK_SCRIPT_DO);
	lex = associate_token(lex, "DONE", T_SCRIPT_CONTAINER, TK_SCRIPT_DONE);
	return (lex);
}

/*
** Main lexer function for upgrading script elements
*/

void			script_lexemes(t_lexeme *lexemes)
{
	int		first;

	first = 0;
	while (lexemes)
	{
		if (lexemes->type == T_WORD && first == 0)
		{
			first = 1;
			lexemes = is_keyword(lexemes);
			if (lexemes->type >= 5)
				log_info("Update elem w/ data |%s| - type : %zu", \
				lexemes->data, lexemes->type);
		}
		if (lexemes->type != T_WORD)
			first = 0;
		lexemes = lexemes->next;
	}
}
