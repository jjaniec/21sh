/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tests.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 16:55:55 by sbrucker          #+#    #+#             */
/*   Updated: 2018/11/16 19:52:28 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

static void	tests_multi_ok(char *test_name, int nbr_tests, ...)
{
	va_list	va_ptr;
	int		i;
	char	*str;
	int		result;
	t_lexeme	*tmp;
	t_lexeme	*error;
	char	*a_mort_brubru_qui_free_jamais; //tkt jtm

	i = 0;
	va_start(va_ptr, nbr_tests);
	while (i < nbr_tests)
	{
		str = va_arg(va_ptr, char *);
		result = va_arg(va_ptr, int);
		lexer((a_mort_brubru_qui_free_jamais = ft_strdup(str)), &tmp, NULL);
		if (result == 1)
			ok(check_parsing(tmp, &error), test_name);
		else
			ok(!check_parsing(tmp, &error), test_name);
		free_lexemes(tmp);
		free(a_mort_brubru_qui_free_jamais);
		i++;
	}
	va_end(va_ptr);
}

/*
** 0: false
** 1: true
*/
void ast_check(void)
{
	tests_multi_ok("Ast check parsing", 11, 
	"", 1,
	"ls", 1,
	"<", 0,
	"ls -l > | wc", 0,
	"ls -l | wc > a.txt < cat ;", 1,
	"ls ; ; ls", 0,
	"ls 2>&1 6>&1 > a.txt", 1,
	"> ls", 0,
	"<< a", 0,
	"ls ; << a", 0,
	"PWD=nope > a.txt", 1,
	"2>1", 1,
	"a 2>a", 1,
	">1", 1,
	"1>", 0,
	">", 0,
	"1<2", 1,
	"1<", 0,
	"<1", 1,
	"ls &&", 0,
	"&&", 0,
	";", 0,
	"; ls ;", 0,
	"ls ;", 1);
}


static void	ast_tree_diff(int test_nbr, char *str)
{
	char	*cmd;
	int		ret;
	int		save_stdout;
	t_ast	*root;
	char	*a_mort_brubru_qui_free_jamais;
	t_lexeme	*tmp;

	asprintf(&cmd, "diff tests/ast_tree/A.txt tests/ast_tree/%d.txt", test_nbr);
	save_stdout = dup(1);
	close(1);
	open("tests/ast_tree/A.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	root = NULL;
	if (lexer((a_mort_brubru_qui_free_jamais = ft_xstrjoin(str, "\n")), &tmp, NULL))
	{
		root = ast(&tmp);
		ast_debug(root, 0);
	}
	close(1);
	dup2(save_stdout, 1);
	close(save_stdout);
	ret = system(cmd);
	ok(!ret, "AST tree construction");
	free(cmd);
	ast_free(root);
	free_lexemes(tmp);
	free(a_mort_brubru_qui_free_jamais);
}

void	ast_tree(void)
{
	ast_tree_diff(1, "1 ; 2 | 3 && 4 || 5 > 6 > 7 < 8 ;");
	ast_tree_diff(2, "1 ; 2 ; 3 > 4 > 5 | 6 || 7");
	ast_tree_diff(3, "1");
	ast_tree_diff(4, "A=b C=d E=f 1 > 2 && 3");
	ast_tree_diff(5, "1 2>&3 > 4");
	ast_tree_diff(6, "1 ; 2 > 3 || 4 > 5");
	ast_tree_diff(7, "1 ; 2 ; 3 >> 4 > 5 | 6 && 7 | 8 > 9 ; 10 < 11 ; 12");
	ast_tree_diff(8, "A=b ls 2>&- 1> a.txt && pwd");
	system("rm tests/ast_tree/A.txt");
}

void ast_tests(t_environ *env)
{
	(void)env;
	ast_check();
	ast_tree();
}
