/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 16:19:06 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/18 14:54:08 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

static t_exec	*loop_body(char *input, char **envp)
{
	t_lexeme	*lex;
	t_ast		*ast_root;
	t_exec		*exe;

	errno = 0;
	if (!VERBOSE_MODE)
		log_set_quiet(1);
	lex = lexer(input);
	ast_root = ast(lex);
	if (!ast_root)
		exit(1);
	exe = create_exec((const char **)envp);
	exe = exec_cmd(ast_root, exe);
	ast_free(ast_root);
	free_lexemes(lex);
	return (exe);
}

static void twentyonesh(char **envp)
{
	char		*input;
	t_exec		*exe;

	tty_debug = fopen(TTY_DEBUG, "w");
	while (1)
	{
		ft_putstr("%> ");
		input = line_edition();
		ft_putchar('\n');
		exe = loop_body(input, envp);
		if (exe && exe->tmp_envp)
			envp = exe->tmp_envp;
		else if (exe)
			envp = exe->envp;
		else
			exit(1);
		free_exec(&exe);
		free(input);
	}
}

int	main(int ac, char **av, char **envp)
{
	if (!VERBOSE_MODE)
		log_set_quiet(1);
	if (ac > 1)
		loop_body(av[1], cp_envp((const char **)envp));
	else
		twentyonesh(cp_envp((const char **)envp));
	return (0);
}
