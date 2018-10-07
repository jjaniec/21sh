/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/15 13:50:09 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/03 18:29:20 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TESTS_H
# define TESTS_H

# include <forty_two_sh.h>
# include "../libtap/tap.h"

# ifdef __linux__
#  define MODE "Linux"
# endif
# ifndef MODE
#  define MODE "Apple"
# endif

# define TESTS_TMP_FILENAME "/tmp/42sh_tests_tmp.txt"
# define SH_TESTS_TMP_FILENAME "/tmp/tmp.txt"
# define BUFF_SIZE_TMP_FILE 1000000
# define SH_EXEC_CMD_PREFIX "./42sh -c "

# define redirect_both_fds_STDOUT_FILENAME "stdout.log"
# define redirect_both_fds_STDERR_FILENAME "stderr.log"


# define _42SH_DATA_FILE_STDOUT "./21sh.stdout.log"
# define _42SH_DATA_FILE_STDERR "./21sh.stderr.log"
# define SH_DATA_FILE_STDOUT "./sh.stdout.log"
# define SH_DATA_FILE_STDERR "./sh.stderr.log"


# define EXPANSION_TESTS_ENVVAR_NAME "__TEST__"
# define EXPANSION_TESTS_ENVVAR_DATA "TEST123"

# define CMP_BUFF_SIZE 1042


// Lexer tests

void	lexer_tests(t_environ *envp);

void	test_lexeme_list(char *test_name, char *input, ...);

void	compare_sh_42sh_outputs(char *test_name, char *_42sh_cmd, char *sh_cmd);

void	compare_redirected_files_contents(char *test_name, char *test, char *data_expected);

void    compare_fd_with_str(char *test_name, int compared_fd, char *input_str, char *expected_str);

void	redirect_both_fds(int *backup_stdout, int *backup_stderr, char *stdout_filename, char *stderr_filename);

void	compare_fds_with_strings(char *test_name, \
				char *expected_stdout, char *expected_stderr, \
				int backup_stdout_fd, int backup_stderr_fd);

void	lexer_tests(t_environ *envp);
void 	ast_tests(t_environ *env);
void	script_tests(t_environ *env);
void	exec_tests(t_environ *env);
void	builtins_tests(t_environ *env);
void	syntax_highlighting_tests(t_environ *env);
void	builtin_test_tests(t_environ *env);

#endif
