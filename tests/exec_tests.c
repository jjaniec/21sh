/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tests.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/27 17:24:03 by sbrucker          #+#    #+#             */
/*   Updated: 2018/11/21 17:54:14 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

extern t_environ *g_envp;

static void		compare_fds_w_strings(char *test_name, char *str_test, char *expected_stdout, char *expected_stderr)
{
	int		backup_stdout_fd;
	int		backup_stderr_fd;
	char	*cmd_sh;

	redirect_both_fds(&backup_stdout_fd, &backup_stderr_fd, NULL, NULL);
	asprintf(&cmd_sh, SH_EXEC_CMD_PREFIX"\"%s\"", str_test);
	system(cmd_sh);
	compare_fds_with_strings(test_name, expected_stdout, expected_stderr, backup_stdout_fd, backup_stderr_fd);
	remove(redirect_both_fds_STDOUT_FILENAME);
	remove(redirect_both_fds_STDERR_FILENAME);
	free(cmd_sh);
}

void	exec_tests(t_environ *env)
{
	(void)env;
	int		tk_less_tests_tmp_fd;

	compare_fds_w_strings("Error check tests 1 - dir as command path", "/", "", SH_NAME": /: "ERR_ISDIR);
	system("ln -s / /tmp/temp_test_symlink_____");
	compare_fds_w_strings("Error check tests 2 - symlink as command path", "/tmp/temp_test_symlink_____", "", SH_NAME": /tmp/temp_test_symlink_____: "ERR_ISDIR);
	remove("/tmp/temp_test_symlink_____");
	system("cp /bin/ls ./temp_exec___ && chmod -x ./temp_exec___");
	compare_fds_w_strings("Error check tests 3 - Non-executable binary", "./temp_exec___", "", SH_NAME": "ERR_NORIGHTS"./temp_exec___\n");
	remove("./temp_exec___");
	compare_fds_w_strings("Error check tests 4 - No such file or dir path", "./___abcdefghijkl___", "", SH_NAME": "ERR_NO_ENTRY"./___abcdefghijkl___\n");
	compare_fds_w_strings("Error check tests 5 - Command not found", "___abcdefghijkl___", "", SH_NAME": ___abcdefghijkl___: "ERR_CMD_NOT_FOUND);
	compare_fds_w_strings("Error check tests 6 - error redirection", "/ 2> /dev/null", "", "");

	compare_sh_42sh_outputs("Simple OR", "/bin/echo a || /bin/echo b", NULL);
	compare_sh_42sh_outputs("Simple AND", "/bin/echo a && /bin/echo b", NULL);
	compare_sh_42sh_outputs("AND w/ cd;", "pwd; cd .. && /bin/echo a; /bin/echo b", NULL);
	compare_sh_42sh_outputs("AND w/ OR", "/bin/echo a && pwd || /bin/echo ABC", NULL);
	compare_sh_42sh_outputs("OR w/ AND", "/bin/echo a || pwd && /bin/echo ABC", NULL);
	compare_sh_42sh_outputs("AND x2", "/bin/echo a && pwd && /bin/echo ABC", NULL);
	compare_sh_42sh_outputs("OR x2", "/bin/echo a || pwd || /bin/echo ABC", NULL);

	compare_sh_42sh_outputs("Pipes 1 - Simple", "/bin/echo a | cat", NULL);
	compare_sh_42sh_outputs("Pipes 2 - Simple", "/bin/echo a | cat | cat", NULL);
	compare_sh_42sh_outputs("Pipes 3 - redirs", "/bin/echo a 2>&1 | cat", NULL);
	compare_sh_42sh_outputs("Pipes 4 - redirs", "/bin/echo a 2>/dev/null 3>/dev/null | cat", NULL);
	compare_sh_42sh_outputs("Pipes 5 - redirs", "/bin/echo a 1>&2 | cat", NULL);
	compare_sh_42sh_outputs("Pipes 6 - AND", "/bin/echo a | cat && /bin/echo b | cat", NULL);
	compare_sh_42sh_outputs("Pipes 7 - OR", "/bin/echo a | cat || /bin/echo b | cat", NULL);
	compare_sh_42sh_outputs("Pipes 8 - AND, OR", "/bin/echo | cat && /bin/echo a | cat || /bin/echo b | cat", NULL);
	compare_sh_42sh_outputs("Pipes 9 - AND, OR", "/bin/echo a && /bin/echo b | cat || /bin/echo c", NULL);
	compare_sh_42sh_outputs("Pipes 10 - AND, OR, redirs", "ls /dev/thisdoesnotexist 2>&1 | cat || /bin/echo b | cat && ls 1>/dev/null", NULL);
	compare_sh_42sh_outputs("Pipes 11 - Long Mixed", "/bin/echo a 2>&1 | cat | cat || /bin/echo b | cat | cat && /bin/echo c 1>/dev/null | cat ; /bin/echo lol | cat", NULL);
	compare_sh_42sh_outputs("Pipes 12 - misc 1", "if which toilet && which cowsay; then echo lol | cat | toilet | cowsay | head -c 10; fi", NULL);
	compare_sh_42sh_outputs("Pipes 13 - Particular cases 1", "if which toilet && which cowsay; then env | cat | lolcat | cowsay | head -c 10; fi", NULL);
	//here we mask the output as base64 /dev/random will never give twice the same string, those tests are to see if head stops base64
	compare_sh_42sh_outputs("Pipes 14 - Particular cases 2", "base64 /dev/random | head -c 100 | cat | wc -c", NULL);
	compare_sh_42sh_outputs("Pipes 15 - Particular cases 3", "base64 /dev/random | head -c 100 | cat | echo lol | cat", NULL);
	compare_sh_42sh_outputs("Pipes 16 - Command not found hang test", "doesnotexists 2> /dev/null | cat", NULL);
	compare_sh_42sh_outputs("Pipes 17 - Command not found hang test 2", "echo lol | doesnotexists 2> /dev/null | cat", NULL);
	compare_sh_42sh_outputs("Pipes 18 - Command not found hang test 3", "echo lol | doesnotexists 2> /dev/null", NULL);

	tk_less_tests_tmp_fd = open(TESTS_TMP_FILENAME, O_WRONLY | O_CREAT, DEFAULT_OUTPUT_REDIR_FILE_MODE);
	ft_putstr_fd("Some random text in"TESTS_TMP_FILENAME, tk_less_tests_tmp_fd);
	close(tk_less_tests_tmp_fd);
	compare_sh_42sh_outputs("Redirs - TK_LESS 1 - Simple", "cat < "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_LESS 2 - Pipe", "cat < "TESTS_TMP_FILENAME" | cat", NULL);
	compare_sh_42sh_outputs("Redirs - TK_LESS 3 - Multiple pipes", "cat < "TESTS_TMP_FILENAME" | cat | cat | cat|cat|cat|cat", NULL);
	compare_sh_42sh_outputs("Redirs - TK_LESS 4 - OR", "ls /zzz || cat < "TESTS_TMP_FILENAME" |cat ", NULL);
	compare_sh_42sh_outputs("Redirs - TK_LESS 5 - AND", "cat a && cat < "TESTS_TMP_FILENAME" |cat ", NULL);
	compare_sh_42sh_outputs("Redirs - TK_LESS 6 - OR - AND - Pipes", "cat a && cat < "TESTS_TMP_FILENAME" |cat && cat "TESTS_TMP_FILENAME" | cat || cat 'this should not append'; cat <"TESTS_TMP_FILENAME" | cat", NULL);
	compare_sh_42sh_outputs("Redirs - TK_LESS 7 - rewrite input redir to same file", "cat <"TESTS_TMP_FILENAME" 2>&1 | cat >"TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_LESS 8 - Simple on closed stdin", "cat <&- < "TESTS_TMP_FILENAME, NULL);
	//close(tk_less_tests_tmp_fd);
	remove(TESTS_TMP_FILENAME);

	compare_sh_42sh_outputs("Redirs - TK_MORE 1 - Simple", "/bin/echo aaa > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 1 - Simple builtin", "echo aaa > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 2 - Simple w/ pipe", "/bin/echo bbb | cat > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 2 - Simple w/ pipe builtin", "echo bbb | cat > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 3 - Simple w/ pipes 2", "/bin/echo ccc | cat |cat |cat |cat> "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 3 - Simple w/ pipes 2 builtin", "echo ccc | cat |cat |cat |cat> "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 4 - Content replacement w/ AND", "/bin/echo aaa > "TESTS_TMP_FILENAME" && /bin/echo bbb > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 4 - Content replacement w/ AND builtin", "echo aaa > "TESTS_TMP_FILENAME" && echo bbb > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 5 - Content replacement w/ ; & OR", "/bin/echo aaa > "TESTS_TMP_FILENAME";cat /doesnotexists || /bin/echo ccc > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 5 - Content replacement w/ ; & OR builtin", "echo aaa > "TESTS_TMP_FILENAME";cat /doesnotexists || echo ccc > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 6 - Content replacement w/ ; & OR", "echo aaa > "TESTS_TMP_FILENAME";cat /doesnotexists || echo ccc > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_MORE 6 - Content replacement w/ ; & OR builtin", "echo aaa > "TESTS_TMP_FILENAME";cat /doesnotexists || echo ccc > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);

	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - TK_DMORE 1 - Simple - file creation", "/bin/echo a >> "TESTS_TMP_FILENAME\
		"; cat "TESTS_TMP_FILENAME"; rm "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_DMORE 1 - Simple - file creation builtins", "echo a >> "TESTS_TMP_FILENAME\
		"; cat "TESTS_TMP_FILENAME"; rm "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_DMORE 2 - Simple - add addional data", "/bin/echo a > "TESTS_TMP_FILENAME" && /bin/echo b >> "TESTS_TMP_FILENAME"; /bin/echo c >> "TESTS_TMP_FILENAME\
		"; cat "TESTS_TMP_FILENAME"; rm "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_DMORE 2 - Simple - add addional data builtins", "echo a > "TESTS_TMP_FILENAME" && echo b >> "TESTS_TMP_FILENAME"; echo c >> "TESTS_TMP_FILENAME\
		"; cat "TESTS_TMP_FILENAME"; rm "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_DMORE 3 - w/ Pipes", "/bin/echo a | cat|cat|cat|cat >> "TESTS_TMP_FILENAME\
		"; cat "TESTS_TMP_FILENAME"; rm "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_DMORE 3 - w/ Pipes builtins", "echo a | cat|cat|cat|cat >> "TESTS_TMP_FILENAME\
		"; cat "TESTS_TMP_FILENAME"; rm "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - TK_DMORE 4 - w/ TK_MORE replacement", "/bin/echo a >> "TESTS_TMP_FILENAME";/bin/echo b > "TESTS_TMP_FILENAME";/bin/echo c >> "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - TK_DMORE 4 - w/ TK_MORE replacement builtins", "echo a >> "TESTS_TMP_FILENAME";echo b > "TESTS_TMP_FILENAME";echo c >> "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_redirected_files_contents("Redirs - TK_DMORE 5 - Stderr as prefix fd redirect", "cat /doesnotexists 2>> "TESTS_TMP_FILENAME, "cat: /doesnotexists: No such file or directory\n");
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - TK_DMORE 6 - OR - AND - PIPES", "/bin/echo a |cat|cat >> "TESTS_TMP_FILENAME" && /bin/echo b >> "TESTS_TMP_FILENAME" || /bin/echo thisisafail >> "TESTS_TMP_FILENAME"; /bin/echo itworks >> "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - TK_DMORE 6 - OR - AND - PIPES builtins", "echo a |cat|cat >> "TESTS_TMP_FILENAME" && echo b >> "TESTS_TMP_FILENAME" || echo thisisafail >> "TESTS_TMP_FILENAME"; /bin/echo itworks >> "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);

	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 1 - Simple", "/bin/cat <<< lol", NULL);
	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 2 - Simple", "/bin/cat <<< diajdiwajdoiwdjdwadjaiwdjwaidj", NULL);
	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 3 - Simple w/ lexer quotes concats", "/bin/cat <<< aaaa\"bbbbb\"'ccccc'ddddd", NULL);
	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 4 - OR", "/bin/cat <<< lol || /bin/cat <<< lal", NULL);
	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 5 - AND", "/bin/cat <<< lol && /bin/cat <<< lal", NULL);
	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 6 - OR & AND", "/bin/cat <<< lol || /bin/cat <<< lal && /bin/cat <<< lel", NULL);
	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 7 - Pipes", "/bin/cat <<< lol | cat", NULL);
	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 8 - Pipes", "/bin/cat <<< lol | cat | cat | cat | cat", NULL);
	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 9 - Pipes", "/bin/cat <<< lol | cat", NULL);
	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 10 - Pipes w/ AND & OR", "/bin/cat <<< lol | cat | cat <<< lal | cat <<< lql && /bin/echo lsl | cat <<< lel || /bin/echo lul | cat <<< lzl", NULL);
	compare_sh_42sh_outputs("Redirs - TK_TLESS Here-documents 11 - Simple on closed stdin", "/bin/cat <&- <<< lol", NULL);

	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - TK_LESSGREAT 1 - Check file creation", "/bin/echo lol 3<>"TESTS_TMP_FILENAME"; ls "TESTS_TMP_FILENAME, NULL);
	compare_sh_42sh_outputs("Redirs - TK_LESSGREAT 2 - Check write to already created file w/ double redir", "/bin/echo lol 3<>"TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - TK_LESSGREAT 3 - Check file creation & write to created file w/ double redir", "/bin/echo lol 3<>"TESTS_TMP_FILENAME" >&3; cat "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - TK_LESSGREAT 4 - Check file creation & write to created file w/ double redir 2 builtin", "/bin/echo lol 3<>"TESTS_TMP_FILENAME" lal >&3 lel; cat "TESTS_TMP_FILENAME, NULL);

	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Filedesc closes & redirects", "/bin/echo lol 1>&-", NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Filedesc closes & redirects 2", "/bin/echo > "TESTS_TMP_FILENAME" lol 1>&-; cat "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Filedesc closes & redirects 3", "cat <&- 2> "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Filedesc closes & redirects 4", "ls 2<&- > "TESTS_TMP_FILENAME" /thisdoesnotexists; cat "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Filedesc closes & redirects 5", "ls 2<&- >&2 /doesntexists 2> "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	if (*_OS_ == 'D') // linux's sh fd closing operators does not support things like '2>&1-' but only '2>&-'
		compare_sh_42sh_outputs("Redirs - Filedesc closes & redirects 6", "cat 2>&1- "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Filedesc closes & redirects 7", "cat 2>&- "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Filedesc closes & redirects 8", "cat >&- "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	if (*_OS_ == 'D')
		compare_sh_42sh_outputs("Redirs - Filedesc closes & redirects 9", "cat 2>/dev/null <&3- && echo lol", NULL);
	else
		compare_fds_w_strings("Redirs - Filedesc closes & redirects 9 - linux", "cat 2>/dev/null <&3- && echo lol", "", "");
	compare_fds_w_strings("Redirs - Filedesc closes & redirects 10", "cat <&8- 2>/dev/null && echo lol", "", SH_NAME": 8: "ERR_BAD_FILEDESC);
	compare_fds_w_strings("Redirs - Filedesc closes & redirects 11 - Test filedesc backup & echo write error", "echo lol >&- ; echo lol", "lol\n", SH_NAME": write error: "ERR_BAD_FILEDESC);

	remove(TESTS_TMP_FILENAME);
	compare_fds_w_strings("Redirs - Bad filedesc err check 1", "echo lol 1>&- 2>&1", "", SH_NAME": 1: "ERR_BAD_FILEDESC);
	remove(TESTS_TMP_FILENAME);
	compare_fds_w_strings("Redirs - Bad filedesc err check 2", "echo lol > "TESTS_TMP_FILENAME" ; cat < "TESTS_TMP_FILENAME" <&9 9<&0 ; cat "TESTS_TMP_FILENAME"; rm "TESTS_TMP_FILENAME, "lol\n", SH_NAME": 9: "ERR_BAD_FILEDESC);
	remove(TESTS_TMP_FILENAME);
	compare_fds_w_strings("Redirs - TK_LESS non-existant file err check", "echo lol < /aaaa", "", SH_NAME": "ERR_NO_ENTRY"/aaaa\n");
	remove(TESTS_TMP_FILENAME);
	compare_fds_w_strings("Redirs - TK_LESS file w/o rights err check", "echo lol > "TESTS_TMP_FILENAME"; chmod 000 "TESTS_TMP_FILENAME"; cat < "TESTS_TMP_FILENAME"; chmod 777 "TESTS_TMP_FILENAME, "", SH_NAME": "ERR_NORIGHTS""TESTS_TMP_FILENAME"\n");
	remove(TESTS_TMP_FILENAME);
	compare_fds_w_strings("Redirs - TK_MORE in a dir w/o rights err check", "mkdir "TESTS_TMP_FILENAME"; chmod 000 "TESTS_TMP_FILENAME"; echo lol > "TESTS_TMP_FILENAME"/test; chmod 777 "TESTS_TMP_FILENAME, \
		"", SH_NAME": "ERR_NORIGHTS""TESTS_TMP_FILENAME"/test\n");
	remove(TESTS_TMP_FILENAME);
	compare_fds_w_strings("Redirs - TK_MORE in a non-existant dir err check", "echo lol > /test", "", SH_NAME": "ERR_NORIGHTS"/test\n");
	compare_fds_w_strings("Redirs - TK_MORE in a non-existant dir err check 2", "echo lol > /test/test", "", SH_NAME": "ERR_NO_ENTRY"/test/test\n");
	compare_fds_w_strings("Redirs - Failed redir return code check 1", "echo lol > /test/test && echo lol", "", SH_NAME": "ERR_NO_ENTRY"/test/test\n");
	compare_fds_w_strings("Redirs - Failed redir return code check 2", "echo lol > /test/test || echo lol", "lol\n", SH_NAME": "ERR_NO_ENTRY"/test/test\n");

	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Mixed 1", "/bin/echo > "TESTS_TMP_FILENAME" lol 1>&- <>"TESTS_TMP_FILENAME";cat "TESTS_TMP_FILENAME"; rm "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Mixed 2", "ls /doesnotexists 2>"TESTS_TMP_FILENAME" 1>&2", NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Mixed 3", "/bin/echo lol|cat|cat|cat>&2 2>&1|cat >> "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Mixed 4", "/bin/echo lol|cat|cat|cat>&2 2>&1|cat >> "TESTS_TMP_FILENAME"; /bin/echo lal|cat|cat|cat > "TESTS_TMP_FILENAME"; cat "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Mixed 5", "echo def > "TESTS_TMP_FILENAME";cat 9<"TESTS_TMP_FILENAME" 8<&9 7<&8 6<&7 -e 5<&6 4<&5 3<&4 2<&3 1<&2 <&1;cat "TESTS_TMP_FILENAME"; rm -f "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Mixed 6", "echo lol > "TESTS_TMP_FILENAME"; cat 4< "TESTS_TMP_FILENAME" 3<&4 2<&3 <&2 | cat -e", NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Mixed 7", "echo lol > "TESTS_TMP_FILENAME" ; cat 3< "TESTS_TMP_FILENAME" <&3 ; rm "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Mixed 8", "cat <Makefile <&- < Makefile > /dev/null < Makefile", NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Mixed 9", "cat <Makefile <&- < Makefile > /dev/null < Makefile >&-", NULL);
	remove(TESTS_TMP_FILENAME);
	compare_sh_42sh_outputs("Redirs - Mixed 10", "cat <Makefile <&- < Makefile > /dev/null < Makefile >&- > "TESTS_TMP_FILENAME, NULL);
	remove(TESTS_TMP_FILENAME);

	compare_fds_w_strings("Aliases 1 - assignation", "alias rofl 'echo lol'; alias --save", "", NULL);
	compare_fds_w_strings("Aliases 1 - Simple expansion", "rofl", "lol\n", "");
	compare_fds_w_strings("Aliases 2 - redefinition", "alias rofl 'echo lal'; alias --save", "", NULL);
	compare_fds_w_strings("Aliases 2 - Simple expansion", "rofl", "lal\n", "");
	compare_fds_w_strings("Aliases 3 - deletion", "alias -d rofl; alias --save", "", NULL);
	compare_fds_w_strings("Aliases 3 - Deleted alias expansion", "rofl", "", SH_NAME": rofl: "ERR_CMD_NOT_FOUND);
	compare_fds_w_strings("Aliases 4 - Invalid assignation w/ IFS separators", "alias 'ls -la' echo", "", SH_NAME": "ERR_INVALID_ALIAS_NAME);
}
