/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/15 13:51:41 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/01 12:48:06 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"
#include <time.h>

t_option		g_sh_opts[] = {
	{{"h", "-help"}, "Print help and exit", false},
	{{"c"}, "Non-interactive mode: Execute command line parameters", false},
	{{"G"}, "Disable syntax highlighting", false},
	{{"v", "-verbose"}, "Enable verbose mode", false},
	{{NULL}, NULL, false}
};

char		**g_envp;

int	main(int argc, char **argv, char **envp)
{
    clock_t 	start, end;
	id_t		backup_stdout, backup_stderr;
	(void)argc;
	(void)argv;
	(void)envp;
	if (!VERBOSE_MODE)
		log_set_quiet(1);

	g_envp = cp_envp((const char **)envp);
	g_sh_opts[1].opt_status = true;
	/*backup_stdout = dup(STDOUT_FILENO);
	backup_stderr = dup(STDERR_FILENO);*/

	//start = clock();
	lexer_tests();
	ast_tests();
	exec_tests(&envp);
	syntax_highlighting_tests(envp);
	script_tests(envp);
	builtin_test_tests(envp);

	/*
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	dup2(backup_stdout, STDOUT_FILENO);
	dup2(backup_stderr, STDERR_FILENO);*/
	done_testing();
	//end = clock();


    //printf("Took %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}
