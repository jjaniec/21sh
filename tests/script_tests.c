/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   script_tests.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 14:25:40 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/04 15:54:10 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

t_environ *g_env_lol;

static	void exec(char *input)
{
	t_lexeme	*lex;
	t_ast		*ast_root;
	t_exec		*exe;

	lexer(input, &lex, NULL);
	ast_root = ast(lex);
	exe = create_exec(g_env_lol);
	if (!ast_root)
		return ;
	exe = create_exec(g_env_lol);
	exe = exec_cmd(ast_root, exe);
	ast_free(ast_root);
	free_lexemes(lex);
	free(exe);
}

static void test_framework(char *str_test, char *expected_stdout, char *test_name)
{
	int		backup_stdout_fd;
	int		backup_stderr_fd;
	char	*tmp;

	redirect_both_fds(&backup_stdout_fd, &backup_stderr_fd, NULL, NULL);
	exec((tmp = ft_strjoin(str_test, "\n")));
	compare_fds_with_strings(test_name, (tmp = ft_strjoin(expected_stdout, "\n")), NULL, backup_stdout_fd, backup_stderr_fd);
	remove(redirect_both_fds_STDOUT_FILENAME);
	remove(redirect_both_fds_STDERR_FILENAME);
	free(tmp);
}

static void tests(t_environ *env)
{
	g_env_lol = env;
	char	error_msg[] = "There is an error in your script.";
	//char	error_msg2[] = "There is an error in your command line.";

	test_framework("if [ 0 ]; then echo OK; fi", "OK", "Simple IF");
	test_framework("if test; then echo NOT OK; fi; echo", "", "Simple IF");
	test_framework("if [ 0 ]; then echo OK; echo ABC; fi", "OK\nABC", "Simple IF");
	test_framework("if [ 0 ]; then echo OK && echo ABC; fi", "OK\nABC", "Simple IF");
	test_framework("if [ 0 ]; then echo OK; elif [ 0 ]; then echo NOT OK; fi", "OK", "Simple IF-ELIF");
	test_framework("if [ 1 ]; then echo NOT OK; elif [ 0 ]; then echo OK; fi", "OK", "Simple IF-ELIF");
	test_framework("if [ 1 ]; then echo NOT OK; elif [ 1 ]; then echo NOT OK; fi; echo", "", "Simple IF-ELIF");
	test_framework("if [ 1 ]; then echo NOT OK; elif [ 1 ]; then echo NOT OK; else echo OK; fi", "OK", "Simple IF-ELIF-ELSE");
	test_framework("if [ 0 ]; then echo OK; elif [ 0 ]; then echo NOT OK; else echo NOT OK neither....; fi", "OK", "Simple IF-ELIF-ELSE");
	test_framework("if [ 1 ]; then echo NOT OK; elif [ 1 ]; then echo NOT OK; else echo OK; fi", "OK", "Simple IF-ELIF-ELSE");
	test_framework("if [ 0 ]; then if [ 0 ]; then echo OK; fi; echo OK; fi", "OK\nOK", "Simple IF nested");
	test_framework("if [ 1 ]; then if [ 0 ]; then echo NOT OK; fi; echo NOT OK2; fi; echo", "", "Simple IF nested");
	test_framework("if [ 0 ]; then if [ 1 ]; then echo NOT OK; fi; echo OK; fi", "OK", "Simple IF nested");
	test_framework("if [ 1 ]; then if [ 1 ]; then echo NOT OK; fi; echo NOT OK2; fi; echo", "", "Simple IF nested");

	test_framework("if", error_msg, "ERROR - Simple IF");
	test_framework("if [ 0 ]; echo NOPE; fi", error_msg, "ERROR - Simple IF");
	test_framework("if [ 0 ] echo NOPE; fi", error_msg, "ERROR - Simple IF");
	test_framework("if [ 0 ] then echo NOPE; fi", error_msg, "ERROR - Simple IF");
	test_framework("if [ 0 ]; then echo NOPE fi", error_msg, "ERROR - Simple IF");
	test_framework("if; [ 0 ]; then echo NOPE; fi", error_msg, "ERROR - Simple IF");
	test_framework("if [ 0 ]; then; echo NOPE; fi", error_msg, "ERROR - Simple IF");
	test_framework("if ; then echo NOPE; fi", error_msg, "ERROR - Simple IF");
	test_framework("if ; then", error_msg, "ERROR - Simple IF");
	test_framework("if ; fi", error_msg, "ERROR - Simple IF");
	test_framework("if [ 0 ]; then echo NOPE;", error_msg, "ERROR - Simple IF");
	test_framework("if if [ 0 ]; echo OK; fi; then echo OK; fi", error_msg, "ERROR Simple IF");
	test_framework("if [ 0 ]; then echo NOPE; elif ;then echo NEITHER; fi", error_msg, "ERROR - Simple IF-ELIF");
	test_framework("if [ 0 ]; then echo NOPE; elif [ 0 ]; echo NEITHER; fi", error_msg, "ERROR - Simple IF-ELIF");
	test_framework("if [ 0 ]; then echo NOPE; elif [ 0 ]; then echo NEITHER;", error_msg, "ERROR - Simple IF-ELIF");
	test_framework("elif [ 0 ]; then echo NOPE;", error_msg, "ERROR - Simple ELIF");
	test_framework("elif [ 0 ]; then echo NOPE; fi", error_msg, "ERROR - Simple ELIF");
	test_framework("elif; then echo NOPE; fi", error_msg, "ERROR - Simple ELIF");
	test_framework("else; then echo NOPE; fi", error_msg, "ERROR - Simple ELSE");
	test_framework("else then echo NOPE; fi", error_msg, "ERROR - Simple ELSE");
	test_framework("else echo NOPE; fi", error_msg, "ERROR - Simple ELSE");
	test_framework("else echo NOPE", error_msg, "ERROR - Simple ELSE");
	
	test_framework("if if [ 0 ]; then echo NOPE;fi; echo", error_msg, "ERROR - Duplicate token");
	test_framework("if [ 0 ]; then then echo NOPE;fi; echo", error_msg, "ERROR - Duplicate token");
	test_framework("if [ 0 ]; then; then echo NOPE;fi; echo", error_msg, "ERROR - Duplicate token");
	test_framework("if [ 0 ]; then echo NOPE; fi; fi; echo", error_msg, "ERROR - Duplicate token");
	test_framework("if [ 0 ]; then fi; echo NOPE; fi; echo", error_msg, "ERROR - Duplicate token");
	
	test_framework("echo ABC; then; echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; then echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; do; echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; do echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; done; echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; done echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; fi echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; fi; echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; if echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; if; echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; elif echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; elif; echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; else echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; else; echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; while echo DEF", error_msg, "ERROR - Token inside statement");
	test_framework("echo ABC; while; echo DEF", error_msg, "ERROR - Token inside statement");

	test_framework("if [ 0 ]; then echo 'fi'; fi", "fi", "Simple IF with arg like token");
	test_framework("echo 'if'", "if", "Simple IF with arg like token");
	test_framework("if [ 0 ]; then echo 'else'; fi", "else", "Simple IF with arg like token");

	test_framework("touch a; while cat a > /dev/null; do echo OK && rm a; done", "OK", "Simple WHILE");
	test_framework("while [ 1 ]; do echo KO; done; echo", "", "Simple WHILE");

	test_framework("\
			touch a b; while cat a > /dev/null; do \
				echo OK && rm a; \
				while cat b > /dev/null; do \
					echo OK2 && rm b; \
				done; \
			done", "OK\nOK2", "Nested WHILE");

	test_framework("while", error_msg, "ERROR - Simple WHILE");
	test_framework("while [ 0 ]", error_msg, "ERROR - Simple WHILE");
	test_framework("while [ 0 ]; do lol", error_msg, "ERROR - Simple WHILE");
	test_framework("while [ 0 ] do lol; done", error_msg, "ERROR - Simple WHILE");
	test_framework("while ; do lol; done", error_msg, "ERROR - Simple WHILE");
	test_framework("while [ 0 ]; do lol done", error_msg, "ERROR - Simple WHILE");
	test_framework("while while; do lol; done", error_msg, "ERROR - Simple WHILE");
	test_framework("while [ 0 ]; then lol; done", error_msg, "ERROR - Simple WHILE");
	test_framework("while [ 0 ]; do lol; fi", error_msg, "ERROR - Simple WHILE");
	test_framework("while if [ 0 ]; then lol; fi", error_msg, "ERROR - Simple WHILE");

	test_framework("\
		if [ 0 ] && [ 0 ]; then \
			echo OK1; \
			if [ 0 ]; then \
				echo OK2; \
				if [ 1 ]; then \
					echo NOPE; \
				fi; \
				echo OK3; \
			fi; \
			echo OK4; \
		fi", "OK1\nOK2\nOK3\nOK4", "Complex IF");
	test_framework("\
		if [ 0 ] && [ 0 ]; then \
			echo OK1; \
			if [ 0 ]; then \
				echo OK2; \
				if [ 0 ]; then \
					echo OK2.5; \
				fi; \
				echo OK3; \
			fi; \
			echo OK4; \
		fi", "OK1\nOK2\nOK2.5\nOK3\nOK4", "Complex IF");
	test_framework("\
		if [ 0 ] && [ 0 ]; then \
			echo OK1; \
			if [ 1 ]; then \
				echo OK2; \
				if [ 0 ]; then \
					echo OK2.5; \
				fi; \
				echo OK3; \
			fi; \
			echo OK4; \
		fi", "OK1\nOK4", "Complex IF");
	test_framework("\
		if [ 0 ] && [ 1 ]; then \
			echo OK1; \
			if [ 0 ]; then \
				echo OK2; \
				if [ 0 ]; then \
					echo OK2.5; \
				fi; \
				echo OK3; \
			fi; \
			echo OK4; \
		fi; echo", "", "Complex IF");
	test_framework("\
		if [ 0 ] && [ 0 ]; then \
			echo OK1; \
			if [ 1 ]; then \
				echo OK2; \
				if [ 0 ]; then \
					echo OK2.5; \
				fi; \
				echo OK3; \
			fi; \
			echo OK4; \
		fi; echo ABC", "OK1\nOK4\nABC", "Complex IF");
	test_framework("\
		echo OKK; \
		if [ 0 ] && [ 1 ]; then \
			echo OK1; \
			if [ 0 ]; then \
				echo OK2; \
				if [ 0 ]; then \
					echo OK2.5; \
				fi; \
				echo OK3; \
			fi; \
			echo OK4; \
		fi;", "OKK", "Complex IF");
	test_framework("\
		echo OKK; \
		if [ 0 ] && [ 0 ]; then \
			if [ 1 ]; then \
				echo OK2; \
				if [ 0 ]; then \
					echo OK2.5; \
				fi; \
				echo OK3; \
			fi; \
			echo OK4; \
		fi;", "OKK\nOK4", "Complex IF");

	test_framework("\
		echo OKK\n \
		if [ 0 ] && [ 0 ]\n then \
			if [ 1 ]\n then \
				echo OK2\n \
				if [ 0 ]\n then \
					echo OK2.5\n \
				fi\n \
				echo OK3\n \
			fi\n \
			echo OK4\n \
		fi", "OKK\nOK4", "Complex IF \\n");
	test_framework("\
		echo OKK\n \
		if [ 0 ] && [ 0 ]\n then \
			if [ 1 ]\n then \
				echo OK2\n \
				if [ 0 ] then \
					echo OK2.5\n \
				fi\n \
				echo OK3\n \
			fi\n \
			echo OK4\n \
		fi", error_msg, "Complex IF error");
	test_framework("\
		echo OKK\n \
		if [ 0 ] && [ 0 ]\n then \
			if [ 1 ]\n then \
				echo OK2\n \
				if [ 0 ] then\n \
				fi\n \
				echo OK3\n \
			fi\n \
			echo OK4\n \
		fi", error_msg, "Complex IF error");

	test_framework("\
		echo 1 && echo 2; \
		if [ 0 ]; then \
			echo 3;\
			if [ 0 ]; then \
				echo 4;\
				if [ 0 ]; then \
					if [ 0 ]; then \
						echo 5;\
						if [ 1 ]; then\
							echo -1; \
						elif [ 0 ]; then \
							echo 6; echo 7; \
						else \
							echo -2; \
						fi;\
						echo 8; \
						echo 9; \
					else \
						echo -3; \
					fi; \
				elif [ 1 ]; then\
					echo -4; \
				elif [ 0 ]; then \
					echo -5; \
				fi; \
				echo 10;\
			fi; \
		elif [ 0 ]; then \
			echo -6; \
		fi; \
		echo 11;\
		", "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11", "Ultimate IF-ELIF-ELSE");
	test_framework("\
		echo 1 && echo 2; \
		touch a; \
		while cat a > /dev/null; do \
			echo 0 && rm a; \
			if [ 0 ]; then \
				echo 3;\
				if [ 0 ]; then \
					echo 4;\
					if [ 0 ]; then \
						if [ 0 ]; then \
							echo 5;\
							if [ 1 ]; then\
								echo -1; \
							elif [ 0 ]; then \
								echo 6; echo 7; \
							else \
								echo -2; \
							fi;\
							echo 8; \
							echo 9; \
						else \
							echo -3; \
						fi; \
					elif [ 1 ]; then\
						echo -4; \
					elif [ 0 ]; then \
						echo -5; \
					fi; \
					echo 10;\
				fi; \
			elif [ 0 ]; then \
				echo -6; \
			fi; \
		done; \
		echo 11;\
		", "1\n2\n0\n3\n4\n5\n6\n7\n8\n9\n10\n11", "Ultimate IF-ELIF-ELSE inside WHILE");
	test_framework("\
		echo 1 && echo 2; \
		touch a; \
			if [ 0 ]; then \
				echo 3;\
				if [ 0 ]; then \
					echo 4;\
					while cat a; do \
						echo 0 && rm a; \
						if [ 0 ]; then \
							if [ 0 ]; then \
								echo 5;\
								if [ 1 ]; then\
									echo -1; \
								elif [ 0 ]; then \
									echo 6; echo 7; \
								else \
									echo -2; \
								fi;\
								echo 8; \
								echo 9; \
							else \
								echo -3; \
							fi; \
						elif [ 1 ]; then\
							echo -4; \
						elif [ 0 ]; then \
							echo -5; \
						fi; \
						echo 9.5; \
					done; \
					echo 10;\
				fi; \
			elif [ 0 ]; then \
				echo -6; \
			fi; \
		echo 11;\
		", "1\n2\n3\n4\n0\n5\n6\n7\n8\n9\n9.5\n10\n11", "WHILE inside ultimate IF-ELIF-ELSE");
}

void script_tests(t_environ *env)
{
	tests(env);
}
/*
				*/
