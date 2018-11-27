/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 16:19:06 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/27 12:07:57 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

struct s_cmd_status	g_cmd_status = {
	.cmd_running = false, .keep_le_cmd = NULL, .resize_happened = false, \
	.sigint_happened = false, .interactive_mode = false, .builtin_running = false
};

t_option		g_sh_opts[] = {
	{{"h", "-help"}, "Print help and exit", false},
	{{"c"}, "Non-interactive mode: Execute command line parameters", false},
	{{"G"}, "Disable syntax highlighting", false},
	{{"v", "-verbose"}, "Enable verbose mode", false},
	{{"-le-debug"}, "Enable line edition debugging in "TTY_DEBUG, false},
	{{NULL}, NULL, false}
};

t_job			*g_jobs;

char		*get_valid_input(t_lexeme **lexemes, int sub_prompt)
{
	char		*input;
	char		*unmatched_quote_err_ptr;
	t_lexeme	*lexemes_ret;

	input = RESIZE_IN_PROGRESS;
	while (input == RESIZE_IN_PROGRESS)
		input = line_edition(sub_prompt);
	if (g_cmd_status.resize_happened == false)
		ft_putchar('\n');
	while (lexer(input, &lexemes_ret, &unmatched_quote_err_ptr) == \
	UNMATCHED_QUOTE_ERR)
	{
		free_lexemes(lexemes_ret);
		if (!subpp_string(&input))
			return (NULL);
	}
	*lexemes = lexemes_ret;
	return (input);
}

static int		forty_two_sh(char *input, t_shell_vars *vars, \
					t_option *opt_list, t_option **char_opt_index)
{
	t_ast		*ast_root;
	t_exec		*exe;
	t_lexeme	*lexemes;

	(void)opt_list;
	(void)char_opt_index;
	lexemes = NULL;
	if (/*is_option_activated("c", opt_list, char_opt_index) &&*/ \
		lexer(input, &lexemes, NULL) == UNMATCHED_QUOTE_ERR)
	{
		ft_printf("Non-interactive mode: unmatched quote error, exiting\n");
		free(input);
		free_lexemes(lexemes);
		exit(EXIT_FAILURE);
	}
	ast_root = ast(&lexemes);
	free(input);
	free_lexemes(lexemes);
	if (!ast_root)
		return (-1);
	link_ast_data(ast_root);
	exe = create_exec(vars->env);
	exe = exec_cmd(ast_root, exe);
	ast_free(ast_root);
	vars->last_cmd_return = exe->ret;
	free_exec(&exe);
	free_job(g_jobs);
	g_jobs = NULL;
	return (0);
}

static void		loop_body(t_shell_vars *vars, t_option *opt_list, t_option **char_opt_index)
{
	t_lexeme	*lex;
	char		*input;

	errno = 0;
	while (!!~!~!~(intptr_t)("taco""cat" - ((0xB16B00B5 ^ 0xDEADC0DE) * (!42))))
	{
		if (!(input = get_valid_input(&lex, 0)))
			continue ;
		else
			free_lexemes(lex);
		if (input != NULL && input[0] != '\0' && input[0] != '\n')
			add_history(input, access_le_main_datas());
		forty_two_sh(input, vars, opt_list, char_opt_index);
	}
}

static int	open_error_print_msg(char *file)
{
	ft_putstr_fd(SH_NAME": ", 2);
	ft_putstr_fd(file, 2);
	ft_putendl_fd(": No such file or directory", 2);
	return (1);
}

static char	*read_file(int fd)
{
	char	*line;
	char	*final;
	int		ret;

	final = ft_strnew(0); errno = 0;
	while((ret = get_next_line(fd, &line)))
	{
		if (ret == -1)
			exit(EXIT_FAILURE);
		final = ft_strjoin_free(final, line);
		final = ft_strjoin_free(final, ft_strdup("\n"));
	}
	return (final);
}

static int	interpret_file(char **argv, t_option **char_opt_index)
{
	int		fd;
	struct stat filestat;

	argv++;
	while(*argv && **argv == '-')
		argv++;
	if (*argv && !is_option_activated("c", g_sh_opts, char_opt_index))
	{
		if ((fd = open(*argv, O_RDONLY)) <= 0)
			exit(open_error_print_msg(*argv));
		if (fstat(fd, &filestat) == -1)
			exit(EXIT_FAILURE);
		if (!(filestat.st_mode & S_IFREG))
			ft_putstr_fd(SH_NAME": Filetype not supported.\n", 2);
		else if (!(filestat.st_mode & S_IRUSR))
			ft_putstr_fd(SH_NAME": No read right.\n", 2);
		else
		{
			g_sh_opts[1].opt_status = true;
			forty_two_sh(read_file(fd), get_shell_vars(), g_sh_opts, char_opt_index);
		}
		close(fd);
		return (1);
	}
	return (0);
}

/*
** Copy environnement and initialize local and internal variables
** of the shell
*/

static void		init_shell_vars(char **env, t_shell_vars *vars)
{
	static t_environ			env_vars;
	static t_local_vars			local_vars;
	static t_internal_vars		internal_vars;
	char						*ret_itoa;

	vars->env = &env_vars;
	vars->locals = &local_vars;
	vars->internals = &internal_vars;
	init_environ(env, vars->env);
	init_environ_struct_ptrs(&local_vars);
	init_environ_struct_ptrs(&internal_vars);
	if ((ret_itoa = ft_itoa(getpid())) == NULL)
		exit(MALLOC_ERROR);
	internal_vars.add_var(&internal_vars, "$", ret_itoa);
	free(ret_itoa);
	internal_vars.add_var(&internal_vars, "!", "0");
	internal_vars.add_var(&internal_vars, "42SH_VERSION", "0.0.42");
	if ((ret_itoa = ft_itoa(getuid())) == NULL)
		exit(MALLOC_ERROR);
	internal_vars.add_var(&internal_vars, "UID", ret_itoa);
	free(ret_itoa);
	internal_vars.add_var(&internal_vars, "IFS", IFS);
	vars->hashtable = ht_create(&env_vars);
	load_aliases_file(access_alias_datas());
}

int		main(int ac, char **av, char **envp)
{
	t_option		*opt_list;
	t_option		*char_opt_index[CHAR_OPT_INDEX_SIZE];
	char			**args;

	opt_list = g_sh_opts;
	g_jobs = NULL;
	args = parse_options(&ac, av, opt_list, (t_option **)char_opt_index);
	if (!(VERBOSE_MODE || is_option_activated("v", opt_list, char_opt_index)))
		log_set_quiet(1);
	init_shell_vars(envp, get_shell_vars());
	if (is_option_activated("h", opt_list, char_opt_index))
	{
		format_help(SH_USAGE, opt_list);
		exit(EXIT_SUCCESS);
	}
	if (is_option_activated("-le-debug", opt_list, char_opt_index))
	{
		tty_debug = fopen(TTY_DEBUG, "w");
		get_le_debug_status(LE_DEBUG_STATUS_SET, 1);
		{ le_debug("MAIN() SE LANCE PID %i  GROUPE %i\n", (int) getpid(), (int) getpgrp()  ) } // debug
	}
	init_signals();
	if (interpret_file(av, char_opt_index))
		return (0);
	if (ac >= 0 && is_option_activated("c", opt_list, char_opt_index))
	{
		g_cmd_status.interactive_mode = false;
		while (ac > 0)
		{
			forty_two_sh(ft_strjoin(*args, "\n"), get_shell_vars(), opt_list, char_opt_index);
			args++;
			ac--;
		}
		free_all_shell_datas();
	}
	else
	{
		if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		{
			ft_putstr_fd(NOT_A_TTY_STDINOUT_ERR, 2);
			free_all_shell_datas();
			exit(EXIT_FAILURE);
		}
		g_cmd_status.interactive_mode = true;
		loop_body(get_shell_vars(), opt_list, char_opt_index);
	}
	return (EXIT_SUCCESS);
}
