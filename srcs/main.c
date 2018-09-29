/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 16:19:06 by jjaniec           #+#    #+#             */
/*   Updated: 2018/09/29 15:57:55 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

t_option		g_sh_opts[] = {
	{{"h", "-help"}, "Print help and exit", false},
	{{"c"}, "Non-interactive mode: Execute command line parameters", false},
	{{"G"}, "Disable syntax highlighting", false},
	{{"v", "-verbose"}, "Enable verbose mode", false},
	{{"-le-debug"}, "Enable line edition debugging in "TTY_DEBUG, false},
	{{NULL}, NULL, false}
};

char			**g_envp;
struct s_line	*g_le;

static char		*get_valid_input(t_lexeme **lexemes)
{
	char		*input;
	char		*unmatched_quote_err_ptr;
	t_lexeme	*lexemes_ret;

	input = line_edition(0);
	ft_putchar('\n'); 
	while (lexer(input, &lexemes_ret, &unmatched_quote_err_ptr) == \
		UNMATCHED_QUOTE_ERR)
	{
		free_lexemes(lexemes_ret);
		subp_string(&input);
	}
	*lexemes = lexemes_ret;
	return (input);
}

static int		twenty_one_sh(char *input, char **envp, \
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
		exit(1);
	}
	ast_root = ast(lexemes);
	link_ast_data(ast_root);
	free_lexemes(lexemes);
	if (!ast_root)
		return (1);
	exe = create_exec((const char **)envp);
	exe = exec_cmd(ast_root, exe);
	/*if (exe && exe->tmp_envp)
		envp = exe->tmp_envp;
	else if (exe)
		envp = exe->envp;
	else
		exit(1);*/
	ast_free(ast_root);
	free_exec(&exe);
	free(input);
	return (0);
}

static void		loop_body(char **envp, t_option *opt_list, t_option **char_opt_index)
{
	t_lexeme	*lex;
	char		*input;

	errno = 0;
	if (!VERBOSE_MODE)
		log_set_quiet(1);
	while (1)
	{
		if (!(input = get_valid_input(&lex)))
			return ;
		free_lexemes(lex);
		if (input != NULL && input[0] != '\0' && input[0] != '\n')
			add_history(input, access_le_main_datas());
		twenty_one_sh(input, envp, opt_list, char_opt_index);
	}
}

int			main(int ac, char **av, char **envp)
{
	t_option	*opt_list;
	t_option	*char_opt_index[CHAR_OPT_INDEX_SIZE];
	char		**args;

	g_envp = cp_envp((const char **)envp);
	tty_debug = fopen(TTY_DEBUG, "w");
	opt_list = g_sh_opts;
	args = parse_options(&ac, av, opt_list, (t_option **)char_opt_index);
	if (!(VERBOSE_MODE || is_option_activated("v", opt_list, char_opt_index)))
		log_set_quiet(1);
	if (is_option_activated("h", opt_list, char_opt_index))
	{
		format_help(SH_USAGE, opt_list);
		exit(0);
	}
	if (is_option_activated("-le-debug", opt_list, char_opt_index))
		get_le_debug_status(LE_DEBUG_STATUS_SET, 1);
	if (ac >= 0 && is_option_activated("c", opt_list, char_opt_index))
		while (ac > 0)
		{
			twenty_one_sh(ft_strjoin(*args, "\n"), g_envp, opt_list, char_opt_index);
			args++;
			ac--;
		}
	else
		loop_body(g_envp, opt_list, char_opt_index);
	return (0);
}
