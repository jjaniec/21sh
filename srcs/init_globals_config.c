/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_globals_config.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/14 10:55:00 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/01 12:00:29 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

const char	*g_prompts[] = {
	"$ ",
	"AND ",
	"OR ",
	"... ",
	"PIPE ",
	"QTES ",
	"HRDC "
};

/*
** These are token stored in [t_ast|t_lexemes]->type_details
** If a node own this token, it isn't put in the AST, and just passed.
** Need to be NULL-terminated.
*/

const int	g_token_bypass[] = {
	TK_SCRIPT_THEN,
	TK_SCRIPT_ELSE,
	TK_SCRIPT_FI,
	TK_SCRIPT_DO,
	TK_SCRIPT_DONE,
	0
};

/*
** These are token stored in [t_ast|t_lexemes]->type_details
** This array correlate with the two nexts.
** If a node own a token from this array, it will create a new sub_ast \
** from this node to the next corresponding node from g_next_tokens[].
*/

const int	g_tokens[] = {
	TK_SCRIPT_IF,
	TK_SCRIPT_THEN,
	TK_SCRIPT_ELSE,
	TK_SCRIPT_WHILE,
	TK_SCRIPT_DO,
	TK_SCRIPT_CONDITION_IF,
	TK_SCRIPT_CONDITION_WHILE,
	0
};

/*
** These are token stored in [t_ast|t_lexemes]->type_details
** This array correlate with the last one and the next one.
** It is an array of array of tokens.
** It defines all the possible tokens for closing the sub_ast \
** opened by the array g_tokens[].
*/

const int	g_next_tokens[][4] = {
	{TK_SCRIPT_FI, 0, 0, 0},
	{TK_SCRIPT_ELIF, TK_SCRIPT_ELSE, TK_SCRIPT_FI, 0},
	{TK_SCRIPT_FI, 0, 0, 0},
	{TK_SCRIPT_DONE, 0, 0, 0},
	{TK_SCRIPT_DONE, 0, 0, 0},
	{TK_SCRIPT_THEN, 0, 0, 0},
	{TK_SCRIPT_DO, 0, 0, 0},
};

/*
** These are token stored in [t_ast|t_lexemes]->type_details
** This array correlate with the two lasts.
** This array contain function pointer for placing node in the sub_ast \
** opened by the array g_tokens[] and closed by g_next_tokens[].
*/

void	(* const g_node_placer[])(t_ast *, t_ast *) = {
	&node_placer_if,
	&node_placer_classic,
	&node_placer_classic,
	&node_placer_while,
	&node_placer_classic,
	&node_placer_classic,
	&node_placer_classic
};

/*
** These are token stored in [t_ast|t_lexemes]->type_details
** It defines all the tokens a command_line cannot begin with.
*/

const int g_cant_begin_with[7] = {
	TK_SCRIPT_THEN,
	TK_SCRIPT_ELIF,
	TK_SCRIPT_ELSE,
	TK_SCRIPT_FI,
	TK_SCRIPT_DONE,
	TK_SEMICOLON,
	0
};

