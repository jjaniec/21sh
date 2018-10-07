/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_highlighting_tests.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/28 21:10:22 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/01 20:24:57 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

# define MAX_BUF_SIZE	50000

static void		test_syntax_hightlighting(char *test_name, char *test, \
					char *expected_stdout)
{
	int			backup_stdout_fd;
	int			backup_stderr_fd;
	t_lexeme	*lexemes = NULL;

	redirect_both_fds(&backup_stdout_fd, &backup_stderr_fd, NULL, NULL);
	lexer(test, &lexemes, NULL);
	print_colorized_input(test, lexemes, NULL);
	free_lexemes(lexemes);
	compare_fds_with_strings(test_name, expected_stdout, NULL, backup_stdout_fd, backup_stderr_fd);
	remove(redirect_both_fds_STDOUT_FILENAME);
	remove(redirect_both_fds_STDERR_FILENAME);
}

void			syntax_highlighting_tests(t_environ *env)
{
	(void)env;
	test_syntax_hightlighting("Simple - progname found", "ls", COL_PROG_NAME_FOUND"ls"COL_DEFAULT);
	test_syntax_hightlighting("Simple 2 - progname not found", "ls_", COL_PROG_NAME_NOT_FOUND"ls_"COL_DEFAULT);
	test_syntax_hightlighting("Simple 3 - options", "ls -la", COL_PROG_NAME_FOUND"ls"COL_DEFAULT COL_PROG_OPT" -la"COL_DEFAULT);
	test_syntax_hightlighting("Simple 4 - Path as progname", "/bin/ls -la", COL_PROG_NAME_FOUND"/bin/ls"COL_DEFAULT COL_PROG_OPT" -la"COL_DEFAULT);
	remove("/tmp/thisdoesnotexists");
	test_syntax_hightlighting("Simple 5 - Builtin as progname", "env", COL_PROG_NAME_FOUND"env"COL_DEFAULT);
	test_syntax_hightlighting("Simple 6 - Builtin as progname w/ options & operators", "echo ls && env && echo ls -la", COL_PROG_NAME_FOUND"echo"COL_DEFAULT COL_PROG_ARG_NOT_FOUND" ls"COL_DEFAULT\
		COL_OPERATORS" &&" COL_DEFAULT COL_PROG_NAME_FOUND" env"COL_DEFAULT COL_OPERATORS" &&"COL_DEFAULT COL_PROG_NAME_FOUND" echo"COL_DEFAULT COL_PROG_ARG_NOT_FOUND" ls"COL_DEFAULT\
		COL_PROG_OPT" -la"COL_DEFAULT);
	test_syntax_hightlighting("Prog found options & redirs", "ls -la 2>/dev/null 1>/tmp/thisdoesnotexists", COL_PROG_NAME_FOUND"ls"COL_DEFAULT COL_PROG_OPT" -la"COL_DEFAULT\
		COL_REDIRS" 2>"COL_DEFAULT COL_PROG_ARG_FILE"/dev/null"COL_DEFAULT COL_REDIRS" 1>"COL_DEFAULT COL_PROG_ARG_NOT_FOUND"/tmp/thisdoesnotexists"COL_DEFAULT);
	remove("/tmp/thisdoesnotexists");
	test_syntax_hightlighting("Quoted args 1", "ls -\\\" \" arg \"   '  arg  ' \" arg\"'arg ' ||ls_", COL_PROG_NAME_FOUND"ls"COL_DEFAULT COL_PROG_OPT" -\\\""COL_DEFAULT COL_QUOTED_ARG" \" arg \""COL_DEFAULT\
		COL_QUOTED_ARG"   '  arg  '"COL_DEFAULT COL_QUOTED_ARG" \" arg\"'arg '"COL_DEFAULT COL_OPERATORS" ||"COL_DEFAULT COL_PROG_NAME_NOT_FOUND"ls_"COL_DEFAULT);
	close(open("./temp_file", O_WRONLY | O_CREAT | O_TRUNC, 0666));
	test_syntax_hightlighting("Long 1 - prog not found - whitespaces - multiple options - redirs", "ls___      -a -b -c   \t-d\t  \t-- ./temp_file ./srcs 2>/dev/null 2>&1", COL_PROG_NAME_NOT_FOUND"ls___"COL_DEFAULT\
		COL_PROG_OPT"      -a"COL_DEFAULT COL_PROG_OPT" -b"COL_DEFAULT COL_PROG_OPT" -c"COL_DEFAULT COL_PROG_OPT"   \t-d"COL_DEFAULT COL_PROG_OPT"\t  \t--"COL_DEFAULT COL_PROG_ARG_FILE" ./temp_file"COL_DEFAULT\
		COL_PROG_ARG_DIR" ./srcs"COL_DEFAULT COL_REDIRS" 2>"COL_DEFAULT COL_PROG_ARG_FILE"/dev/null"COL_DEFAULT COL_REDIRS" 2>&"COL_DEFAULT COL_PROG_ARG_NOT_FOUND"1"COL_DEFAULT);
	test_syntax_hightlighting("Long 2 - w/ operators", "ls -la ./temp_file; ls; ls -la;ls ;   ls;", COL_PROG_NAME_FOUND"ls"COL_DEFAULT COL_PROG_OPT" -la"COL_DEFAULT COL_PROG_ARG_FILE" ./temp_file"COL_DEFAULT\
		COL_OPERATORS";"COL_DEFAULT COL_PROG_NAME_FOUND" ls"COL_DEFAULT COL_OPERATORS";"COL_DEFAULT COL_PROG_NAME_FOUND" ls"COL_DEFAULT COL_PROG_OPT" -la"COL_DEFAULT COL_OPERATORS";"COL_DEFAULT\
		COL_PROG_NAME_FOUND"ls"COL_DEFAULT COL_OPERATORS" ;"COL_DEFAULT COL_PROG_NAME_FOUND"   ls"COL_DEFAULT COL_OPERATORS";"COL_DEFAULT);
	test_syntax_hightlighting("Long 3 - operators - env expansions", "ls -la $PWD ./temp_file $HOME ||ls|| ls&&ls_", COL_PROG_NAME_FOUND"ls"COL_DEFAULT COL_PROG_OPT" -la"COL_DEFAULT COL_ENV_VAR_EXPANSION" $PWD"COL_DEFAULT\
		COL_PROG_ARG_FILE" ./temp_file"COL_DEFAULT COL_ENV_VAR_EXPANSION" $HOME"COL_DEFAULT COL_OPERATORS" ||"COL_DEFAULT COL_PROG_NAME_FOUND"ls"COL_DEFAULT COL_OPERATORS"||"COL_DEFAULT COL_PROG_NAME_FOUND" ls"COL_DEFAULT\
		COL_OPERATORS"&&"COL_DEFAULT COL_PROG_NAME_NOT_FOUND"ls_"COL_DEFAULT);
	remove("./temp_file");
}
