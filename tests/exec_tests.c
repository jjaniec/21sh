/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 17:24:03 by sbrucker          #+#    #+#             */
/*   Updated: 2018/08/21 21:29:50 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

# define TESTS_TMP_FILENAME "/tmp/21sh_tests_tmp.txt"
# define BUFF_SIZE_TMP_FILE 1000000

static void	exec_fd_test(char *filename_sh, char *filename_21sh, int redirected_fd, char *str)
{
	char	*cmd_sh;
	int		tmp_fd;
	int		dup_redirected_fd;

	dup_redirected_fd = dup(redirected_fd);
	tmp_fd = open(filename_sh, O_WRONLY | O_CREAT, DEFAULT_OUTPUT_REDIR_FILE_MODE);
	dup2(tmp_fd, redirected_fd);
	asprintf(&cmd_sh, "{ %s ; }", str);
	system(cmd_sh);
	close(tmp_fd);
	tmp_fd = open(filename_21sh, O_WRONLY | O_CREAT, DEFAULT_OUTPUT_REDIR_FILE_MODE);
	dup2(tmp_fd, redirected_fd);
	asprintf(&cmd_sh, "./21sh \"%s\"", str);
	system(cmd_sh);
	close(tmp_fd);
	dup2(dup_redirected_fd, redirected_fd);
	close(dup_redirected_fd);
}

static void	exec_diff_fd(char *test_name, char *str, int redirected_fd)
{
	int		ret;

	exec_fd_test("/tmp/exec_sh.txt", "/tmp/exec_21sh.txt", redirected_fd, str);
	//system("cat /tmp/exec_21sh.txt");
	//system("cat /tmp/exec_sh.txt");
	ret = system("diff /tmp/exec_21sh.txt /tmp/exec_sh.txt");
	ok(!ret, test_name);
	remove("/tmp/exec_sh.txt");
	remove("/tmp/exec_21sh.txt");
}

static void	exec_diff(char *test_name, char *str)
{
	int		test_name_str_len;
	char	*new_test_name;
	int		tmp;

	test_name_str_len = ft_strlen(test_name);
	new_test_name = malloc(test_name_str_len * sizeof(char) + 10);
	ft_strcpy(new_test_name, test_name);

	ft_strcpy(new_test_name + test_name_str_len, " - stdout");
	tmp = dup(STDERR_FILENO);
	freopen("/dev/null", "w", stderr);
	exec_diff_fd(new_test_name, str, STDOUT_FILENO);
	dup2(tmp, STDERR_FILENO);

	ft_strcpy(new_test_name + test_name_str_len, " - stderr");
	tmp = dup(STDOUT_FILENO);
	freopen("/dev/null", "w", stdout);
	exec_diff_fd(new_test_name, str, STDERR_FILENO);
	dup2(tmp, STDOUT_FILENO);

	free(new_test_name);
}

static void	comp_output_redir(char *test_name, char *test, char *data_expected)
{
	int		fd;
	char	data_in_file[BUFF_SIZE_TMP_FILE];
	int		tmp;

	system(test);
	fd = open(TESTS_TMP_FILENAME, O_RDONLY);
	if ((tmp = read(fd, data_in_file, BUFF_SIZE_TMP_FILE)) == -1)
	{
		printf("READ ERROR FOR TEST %s\n", test_name);
		return ;
	}
	data_in_file[tmp] = '\0';
	is(data_in_file, data_expected, test_name);
	remove(TESTS_TMP_FILENAME);
}

void	exec_tests(char **envp)
{
	(void)envp;
	int		tk_less_tests_tmp_fd;

	exec_diff("Simple OR", "/bin/echo a || /bin/echo b");
	exec_diff("Simple AND", "/bin/echo a && /bin/echo b");
	exec_diff("AND w/ cd;", "pwd; cd .. && /bin/echo a; /bin/echo b");
	exec_diff("AND w/ OR", "/bin/echo a && pwd || /bin/echo ABC");
	exec_diff("OR w/ AND", "/bin/echo a || pwd && /bin/echo ABC");
	exec_diff("AND x2", "/bin/echo a && pwd && /bin/echo ABC");
	exec_diff("OR x2", "/bin/echo a || pwd || /bin/echo ABC");
	exec_diff("Builtin cd - 1", "cd .. && pwd");
	exec_diff("Builtin cd - 2", "cd .. && /bin/echo '-' && cd ~ && pwd");
	exec_diff("AND - Long 1", "cd / && pwd && cd ~ && ls && cd /usr && ls && cd - && ls && cd && ls");

	exec_diff("Pipes 1 - Simple", "/bin/echo a | cat");
	exec_diff("Pipes 2 - Simple", "/bin/echo a | cat | cat");
	exec_diff("Pipes 3 - redirs", "/bin/echo a 2>&1 | cat");
	exec_diff("Pipes 4 - redirs", "/bin/echo a 2>/dev/null 3>/dev/null | cat");
	exec_diff("Pipes 5 - redirs", "/bin/echo a 1>&2 | cat");
	exec_diff("Pipes 6 - AND", "/bin/echo a | cat && /bin/echo b | cat");
	exec_diff("Pipes 7 - OR", "/bin/echo a | cat || /bin/echo b | cat");
	exec_diff("Pipes 8 - AND, OR", "ls | cat && /bin/echo a | cat || /bin/echo b | cat");
	exec_diff("Pipes 9 - AND, OR", "/bin/echo a && /bin/echo b | cat || /bin/echo c");
	exec_diff("Pipes 10 - AND, OR, redirs", "ls 2>&1 | cat || /bin/echo b | cat && ls 1>/dev/null");
	exec_diff("Pipes 11 - Long Mixed", "/bin/echo a 2>&1 | cat | cat || /bin/echo b | cat | cat && /bin/echo c 1>/dev/null | cat ; /bin/echo lol | cat");

	tk_less_tests_tmp_fd = open(TESTS_TMP_FILENAME, O_WRONLY | O_CREAT, DEFAULT_OUTPUT_REDIR_FILE_MODE);
	ft_putstr_fd("Some random text in"TESTS_TMP_FILENAME, tk_less_tests_tmp_fd);
	exec_diff("TK_LESS 1 - Simple", "cat < "TESTS_TMP_FILENAME);
	exec_diff("TK_LESS 2 - Pipe", "cat < "TESTS_TMP_FILENAME" | cat");
	exec_diff("TK_LESS 3 - Multiple pipes", "cat < "TESTS_TMP_FILENAME" | cat | cat | cat|cat|cat|cat");
	exec_diff("TK_LESS 4 - OR", "ls /zzz || cat < "TESTS_TMP_FILENAME" |cat ");
	exec_diff("TK_LESS 5 - AND", "cat a && cat < "TESTS_TMP_FILENAME" |cat ");
	exec_diff("TK_LESS 6 - OR - AND - Pipes", "cat a && cat < "TESTS_TMP_FILENAME" |cat && cat "TESTS_TMP_FILENAME" | cat || cat 'this should not append'; cat <"TESTS_TMP_FILENAME" | cat");
	exec_diff("TK_LESS 7 - rewrite input redir to same file", "cat <"TESTS_TMP_FILENAME" 2>&1 | cat >"TESTS_TMP_FILENAME);
	close(tk_less_tests_tmp_fd);

	comp_output_redir("TK_MORE 1 - Simple", "/bin/echo aaa > "TESTS_TMP_FILENAME, "aaa\n");
	comp_output_redir("TK_MORE 2 - Simple w/ pipe", "/bin/echo bbb | cat > "TESTS_TMP_FILENAME, "bbb\n");
	comp_output_redir("TK_MORE 3 - Simple w/ pipes", "/bin/echo ccc | cat |cat |cat |cat> "TESTS_TMP_FILENAME, "ccc\n");
	comp_output_redir("TK_MORE 4 - Content replacement w/ AND", "/bin/echo aaa > "TESTS_TMP_FILENAME" && /bin/echo bbb > "TESTS_TMP_FILENAME, "bbb\n");
	comp_output_redir("TK_MORE 5 - Content replacement w/ ; & OR", "/bin/echo aaa > "TESTS_TMP_FILENAME";cat /doesnotexists || /bin/echo ccc > "TESTS_TMP_FILENAME, "ccc\n");

	comp_output_redir("TK_DMORE 1 - Simple - file creation", "/bin/echo a >> "TESTS_TMP_FILENAME, "a\n");
	comp_output_redir("TK_DMORE 2 - Simple - add addional data", "/bin/echo a >> "TESTS_TMP_FILENAME"; /bin/echo b >> "TESTS_TMP_FILENAME, "a\nb\n");
	comp_output_redir("TK_DMORE 3 - w/ Pipes", "/bin/echo a | cat|cat|cat|cat >> "TESTS_TMP_FILENAME, "a\n");
	comp_output_redir("TK_DMORE 4 - w/ TK_MORE replacement", "/bin/echo a >> "TESTS_TMP_FILENAME";/bin/echo b > "TESTS_TMP_FILENAME";/bin/echo c >> "TESTS_TMP_FILENAME, "b\nc\n");
	comp_output_redir("TK_DMORE 5 - Stderr as prefix fd redirect", "cat /doesnotexists 2>> "TESTS_TMP_FILENAME, "cat: /doesnotexists: No such file or directory\n");
	comp_output_redir("TK_DMORE 5 - OR - AND - PIPES", "/bin/echo a |cat|cat >> "TESTS_TMP_FILENAME" && /bin/echo b >> "TESTS_TMP_FILENAME" || /bin/echo thisisafail >> "TESTS_TMP_FILENAME"; /bin/echo itworks >> "TESTS_TMP_FILENAME, "a\nb\nitworks\n");

	exec_diff("TK_TLESS Here-documents 1 - Simple", "/bin/cat <<< lol");
	exec_diff("TK_TLESS Here-documents 2 - Simple", "/bin/cat <<< diajdiwajdoiwdjdwadjaiwdjwaidj");
	exec_diff("TK_TLESS Here-documents 3 - Simple w/ lexer quotes concats", "/bin/cat <<< aaaa\"bbbbb\"'ccccc'ddddd");
	exec_diff("TK_TLESS Here-documents 4 - OR", "/bin/cat <<< lol || /bin/cat <<< lal");
	exec_diff("TK_TLESS Here-documents 5 - AND", "/bin/cat <<< lol && /bin/cat <<< lal");
	exec_diff("TK_TLESS Here-documents 6 - OR & AND", "/bin/cat <<< lol || /bin/cat <<< lal && /bin/cat <<< lel");
	exec_diff("TK_TLESS Here-documents 7 - Pipes", "/bin/cat <<< lol | cat");
	exec_diff("TK_TLESS Here-documents 8 - Pipes", "/bin/cat <<< lol | cat | cat | cat | cat");
	exec_diff("TK_TLESS Here-documents 9 - Pipes", "/bin/cat <<< lol | cat");
	exec_diff("TK_TLESS Here-documents 10 - Pipes w/ AND & OR", "/bin/cat <<< lol | cat | cat <<< lal | cat <<< lql && /bin/echo lsl | cat <<< lel || /bin/echo lul | cat <<< lzl");
	//exec_diff_file_redir("TK_LESS input redirects 1 - Simple", "/bin/echo a > ", "/tmp/21sh_tests_tmp_file.txt", "/tmp/comp_tests_tmp_file.txt");
	//exec_diff("env A=B env C=D && env && setenv A B && env && unsetenv A && env");

}
