/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/15 13:51:41 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/05 18:03:10 by jjaniec          ###   ########.fr       */
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

t_environ		*g_envp;

static void		init_shell_vars(char **env, t_shell_vars *vars)
{
	static t_environ			env_vars;
	static t_local_vars			local_vars;
	static t_internal_vars		internal_vars;

	vars->env = &env_vars;
	vars->locals = &local_vars;
	vars->internals = &internal_vars;
	init_environ(env, vars->env);
	init_environ_struct_ptrs(&local_vars);
	init_environ_struct_ptrs(&internal_vars);
	internal_vars.add_var(&internal_vars, "$", ft_itoa(getpid()));
	internal_vars.add_var(&internal_vars, "!", "0");
	internal_vars.add_var(&internal_vars, "42SH_VERSION", "0.0.42");
	internal_vars.add_var(&internal_vars, "UID", ft_itoa(getuid()));
	internal_vars.add_var(&internal_vars, "IFS", IFS);
}

int	main(int argc, char **argv, char **envp)
{
    clock_t 	start, end;
	id_t		backup_stdout, backup_stderr;
	(void)argc;
	(void)argv;
	(void)envp;
	if (!VERBOSE_MODE)
		log_set_quiet(1);
	init_shell_vars(envp, get_shell_vars());
	t_environ	*env = get_shell_vars()->env;
	//builtin_env((char *[2]){"env", NULL}, env, NULL);
	g_sh_opts[1].opt_status = true;
	/*backup_stdout = dup(STDOUT_FILENO);
	backup_stderr = dup(STDERR_FILENO);*/

	//start = clock();
	lexer_tests(env);
	ast_tests(env);
	builtins_tests(env);
	exec_tests(env);
	syntax_highlighting_tests(env);
	script_tests(env);
	builtin_test_tests(env);

	/*
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	dup2(backup_stdout, STDOUT_FILENO);
	dup2(backup_stderr, STDERR_FILENO);*/
	done_testing();
	//end = clock();

    //printf("Took %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}
