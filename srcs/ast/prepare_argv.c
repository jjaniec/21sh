/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_argv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/11 15:52:17 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/11 19:08:37 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** If pointed lexeme is an operator, only store lexeme data as argv
*/

static char		**prepare_argv_operator(t_lexeme *lexemes)
{
	char		**argv;

	if (!(argv = (char **)ft_memalloc(sizeof(char *) * 2)))
		exit(MALLOC_ERROR);
	argv[0] = lexemes->data;
	argv[1] = NULL;
	return (argv);
}

/*
** Count number of elements found in the lexeme linked list
** to add as program argument
*/

static int		count_argv(t_lexeme *lexemes)
{
	t_lexeme	*ptr;
	int			r;

	ptr = lexemes;
	r = 1;
	while (ptr && ptr->type != T_CTRL_OPT && ptr->type_details != TK_PIPE)
	{
		if (ptr->type == T_WORD)
			r += 1;
		ptr = ptr->next;
	}
	return (r);
}

/*
** Fill $argv pointers tab containing command arguments
*/

static char		**fill_argv_tab(char ***argv, t_lexeme *lexemes)
{
	t_lexeme	*ptr;
	int			i;

	ptr = lexemes;
	i = 0;
	while (ptr && ptr->type != T_CTRL_OPT && ptr->type_details != TK_PIPE)
	{
		if (ptr->type == T_WORD)
			(*argv)[i++] = ptr->data;
		if (ptr->type == T_REDIR_OPT)
			ptr = ptr->next;
		if (ptr)
			ptr = ptr->next;
	}
	(*argv)[i] = NULL;
	return (*argv);
}

/*
** Creates a char pointer tab with each pointer
** containing an argument of the current command to be passed to execve()
*/

char			**prepare_argv(t_lexeme *lexemes)
{
	char	**argv;

	if (lexemes->type != T_WORD || lvl_lex(lexemes) != 5)
		return (prepare_argv_operator(lexemes));
	if (!(argv = malloc(sizeof(char *) * count_argv(lexemes) + 1)))
		exit(MALLOC_ERROR);
	return (fill_argv_tab(&argv, lexemes));
}
