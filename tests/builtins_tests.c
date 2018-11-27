/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 20:38:39 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/21 12:49:25 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

#define SETENV_INVALID_IDENTIFIERS_STR_ERR SH_NAME": invalid identifiers\n"
#define UNSETENV_INVALID_IDENTIFIERS_STR_ERR SH_NAME": invalid identifiers\n"

extern t_environ *g_envp;

static void		compare_fds_w_strings(char *test_name, char *str_test, char *expected_stdout, char *expected_stderr, char *stdin_content)
{
	int		backup_stdout_fd;
	int		backup_stderr_fd;
	char	*cmd_sh;

	redirect_both_fds(&backup_stdout_fd, &backup_stderr_fd, NULL, NULL);
	if (!stdin_content)
		asprintf(&cmd_sh, SH_EXEC_CMD_PREFIX"\"%s\"", str_test);
	else
		asprintf(&cmd_sh, SH_EXEC_CMD_PREFIX"\"%s\" <<< '%s'", str_test, stdin_content);
	system(cmd_sh);
	compare_fds_with_strings(test_name, expected_stdout, expected_stderr, backup_stdout_fd, backup_stderr_fd);
	remove(redirect_both_fds_STDOUT_FILENAME);
	remove(redirect_both_fds_STDERR_FILENAME);
	free(cmd_sh);
}

void			builtins_tests(t_environ *env)
{
	(void)env;

	int		ret;
	ret = system(SH_EXEC_CMD_PREFIX"exit");
	ok((ret == 0), "Builtin exit 1 - no args");
	ret = system(SH_EXEC_CMD_PREFIX"\"exit 42\"");
	ok((ret / 256 == 42), "Builtin exit 2 - passed exit value");
	ret = system(SH_EXEC_CMD_PREFIX"\"exit 42 43 44\"");
	ok((ret / 256 == 0), "Builtin exit 3 - too many args err");
	ret = system(SH_EXEC_CMD_PREFIX"\"exit 42w\"");
	ok((ret / 256 == 255), "Builtin exit 4 - arg not numeric");
	ret = system(SH_EXEC_CMD_PREFIX"\"exit 42w 45 12\"");
	ok((ret / 256 == 255), "Builtin exit 5 - arg not numeric w/ too many args");

	compare_sh_42sh_outputs("Builtin echo 1 - echo", "echo ls", NULL);
	compare_sh_42sh_outputs("Builtin echo 2 - echo multi args", "echo ls ls ls ls ls ls", NULL);
	compare_sh_42sh_outputs("Builtin echo 3 - echo no args", "echo", NULL);
	compare_sh_42sh_outputs("Builtin echo 4 - Pipes (w/ echo)", "echo ls | cat", NULL);
	compare_sh_42sh_outputs("Builtin echo 5 - Redirs (w/ echo)", "echo ls > /tmp/a && cat < /tmp/a", NULL);
	compare_sh_42sh_outputs("Builtin echo 6 - Pipes & redirs (w/ echo)", "echo ls / /abc 2> /tmp/a; cat < /tmp/a", NULL);
	compare_sh_42sh_outputs("Builtin echo 7 - Particular cases 1", "echo $", NULL);

	compare_sh_42sh_outputs("Builtin cd 1 - cd", "cd / && pwd", NULL);
	compare_sh_42sh_outputs("Builtin cd 2 - cd", "cd / && cd dev && pwd", NULL);
	compare_sh_42sh_outputs("Builtin cd 3 - cd", "cd ~ && cd /dev && pwd", NULL);
	compare_sh_42sh_outputs("Builtin cd 4 - cd", "cd ~ && cd /dev && cd .. && pwd", NULL);
	compare_sh_42sh_outputs("Builtin cd 5 - cd -", "cd && pwd && cd - && pwd && cd /dev && pwd && cd - && pwd && cd .. && pwd", NULL);
	compare_sh_42sh_outputs("Builtin cd 6 - cd -", "cd / && pwd && cd / && pwd && cd - && pwd && cd  && pwd && cd - && pwd && cd - && pwd && echo \"/\" | cd && pwd", NULL);
	compare_sh_42sh_outputs("Builtin cd 7 - cd -", "cd .. && pwd && cd - && pwd", NULL);
	compare_sh_42sh_outputs("Builtin cd 8 - cd -", "cd ft_printf && pwd && cd - && pwd", NULL);
	compare_sh_42sh_outputs("Builtin cd 9 - cd -", "cd ft_printf && pwd && cd .. && pwd && cd - && pwd", NULL);
	compare_sh_42sh_outputs("Builtin cd 10 - cd -", "cd srcs && pwd && cd .. && pwd && cd - && pwd && cd - && pwd;", NULL);
	//compare_sh_42sh_outputs("Builtin cd 11 - cd - ", "mkdir janiec; cd janiec ; pwd ; chmod 000 . ; cd ; pwd ; cd - 2> /dev/null; pwd; rm -rf janiec", NULL);
	//compare_sh_42sh_outputs("Builtin cd 12 - cd", "cd janiec; pwd; chmod 777 janiec; rm -rf janiec", NULL);
	//compare_fds_w_strings("Builtin cd 13 - err check cd to a file", "cd Makefile && pwd", "", SH_NAME": Makefile: "ERR_ENOTDIR);

	compare_sh_42sh_outputs("Builtin env 1 - env w/o args w/ pipe", "env | grep -v", NULL);
	compare_sh_42sh_outputs("Builtin env 2 - env w/ T_ENV_ASSIGN", "TMP=test env | grep '^TMP='", NULL);
	compare_sh_42sh_outputs("Builtin env 3 - env w/ valid args & pipe", \
		"env TEST1=TEST__ TEST2=TEST______ | grep -E 'TEST[12]'", "env TEST1=TEST__ TEST2=TEST______ | grep -E 'TEST[12]'");
	compare_sh_42sh_outputs("Builtin env 4 - env -i w/ valid args", "env -i A=B TEST1=TEST__ TEST2=TEST______", "env -i A=B TEST1=TEST__ TEST2=TEST______");
	compare_sh_42sh_outputs("Builtin env 5 - env -i w/ valid args & pipe", "env -i A=B TEST1=TEST__ TEST2=TEST______", NULL);
	compare_sh_42sh_outputs("Builtin env 6 - env -i w/o args w/ pipe", "env -i", NULL);
	compare_sh_42sh_outputs("Builtin env 7 - env -i empty assignations", "env -i LS=     AAAA=   ", NULL);
	compare_sh_42sh_outputs("Builtin env 8 - env w/ T_ENV_ASSIGN", "TMP=test env -i A=B", NULL);
	//compare_sh_42sh_outputs("Builtin env 8 - env -i invalid assignations", "env -i LS=     AAAA  AAAAAA  LOL=   | grep -v _", NULL); -> should say command not found -> exec not yet handled
	// test env execution ex: env A=B ls - not yet implemented
	//compare_sh_42sh_outputs("Builtin env 6 - env -i w/ valid args & execution", "env -i A=B TEST1=TEST__ TEST2=TEST______ ls", NULL);
	//compare_sh_42sh_outputs("Builtin env 7 - env -i w/o valid args", "env -i A", NULL);
	//compare_sh_42sh_outputs("Builtin env 8 - env -i w/o valid args", "env -i ls", NULL);
	//compare_sh_42sh_outputs("Builtin env 9 - env -i w/ assign & execution", "env -i HOME=idontexist ls $HOME", NULL);

	compare_sh_42sh_outputs("T_ENV_ASSIGNS 1 - Simple", \
		"TEST1=TEST__ env  TEST2=TEST______ | grep -E 'TEST[12]'", "TEST1=TEST__ env TEST2=TEST______ | grep -E 'TEST[12]'");
	compare_sh_42sh_outputs("T_ENV_ASSIGNS 2 - Simple multiple vars", \
		"TEST1=TEST__ TEST2=TEST______ env | grep -E 'TEST[12]'", "TEST1=TEST__ TEST2=TEST______ env | grep -E 'TEST[12]'");
	compare_sh_42sh_outputs("T_ENV_ASSIGNS 3 - Multiple vars w/ re-assigns", \
		"TEST1=TEST__ TEST2=TEST______ TEST2=TEST______42 env | grep -E 'TEST[12]'", "TEST1=TEST__ TEST2=TEST______ TEST2=TEST______42 env | grep -E 'TEST[12]'");
	compare_sh_42sh_outputs("T_ENV_ASSIGNS 4 - Temporary env test w/ multiple vars & re-assign", \
		"TEST1=TEST__ env TEST2=TEST______ > /dev/null; env | grep -E 'TEST[12]'", NULL);
	compare_sh_42sh_outputs("T_ENV_ASSIGNS 5 - Temporary env test w/ multiple vars & re-assign 2", \
		"TEST1=TEST__ TEST2=TEST______ TEST2=TEST______42 env > /dev/null ; env | grep -E 'TEST[12]'", NULL);
	compare_sh_42sh_outputs("T_ENV_ASSIGNS 6 - Redefinition of 1st env var", \
		"$(env | head -n 1 | cut -d '=' -f 1)=TEST env | grep $(env | head -n 1 | cut -d '=' -f 1)", NULL);
	compare_fds_w_strings("T_ENV_ASSIGNS 7 - Path redefinition & cmd execution", \
		"PATH=NULL ls", NULL, SH_NAME": ls: "ERR_CMD_NOT_FOUND, NULL);

	// Travis has some weird env vars
	if (*MODE != 'L' && !CI_TEST)
		compare_sh_42sh_outputs("Builtin setenv 1 - w/o args", "setenv | sort | grep -vE '^_=|^SHELL|^OLDPWD|^TRAVIS|^SONARQUBE|^rvm|^ANSI|^BASH|^}'", "export | cut -d ' ' -f 2 | sort | grep -vE '^_=|^SHELL|^OLDPWD|^TRAVIS|^SONARQUBE|^rvm|^ANSI|^BASH' | tr -d '\\\"'");
	compare_fds_w_strings("Builtin setenv 2 - err check 1", "setenv =", NULL, SETENV_INVALID_IDENTIFIERS_STR_ERR, NULL);
	compare_fds_w_strings("Builtin setenv 3 - err check 2", "setenv ==", NULL, SETENV_INVALID_IDENTIFIERS_STR_ERR, NULL);
	compare_fds_w_strings("Builtin setenv 4 - err check 3", "setenv \\$=", NULL, SETENV_INVALID_IDENTIFIERS_STR_ERR, NULL);
	compare_fds_w_strings("Builtin setenv 5 - err check 4", "setenv \\$==", NULL, SETENV_INVALID_IDENTIFIERS_STR_ERR, NULL);
	compare_fds_w_strings("Builtin setenv 6 - err check 5", "setenv \\$\\=", NULL, SETENV_INVALID_IDENTIFIERS_STR_ERR, NULL);
	compare_fds_w_strings("Builtin setenv 7 - err check 6", "setenv \\=", NULL, SETENV_INVALID_IDENTIFIERS_STR_ERR, NULL);
	compare_fds_w_strings("Builtin setenv 8 - err check 7", "setenv \\$wdwadadaw=", NULL, NULL, NULL);
	compare_fds_w_strings("Builtin setenv 9 - err check 8", "setenv \\$\\$wdwadadaw=", NULL, SETENV_INVALID_IDENTIFIERS_STR_ERR, NULL);
	compare_fds_w_strings("Builtin setenv 10 - err check 9", "setenv lol", NULL, BUILTIN_SETENV_USAGE, NULL);
	compare_fds_w_strings("Builtin setenv 11 - err check 10 - invalid arg in large list", \
		"setenv a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a1 a1=b b1=c c1=d d1=e e1=f f1=g g1=h h1=i i1=j j1=k k1=l l1=m m1=n n1=o o1=p p1=q q1=r r1 =s && env | grep -v _", \
		NULL, BUILTIN_SETENV_USAGE, NULL);
	compare_fds_w_strings("Builtin setenv 12 - w/o valid args 3", "setenv a=b b=c d=e f=g =i", NULL, SETENV_INVALID_IDENTIFIERS_STR_ERR, NULL);
	compare_fds_w_strings("Builtin setenv 13 - w/o valid args 4", "setenv a=b b=c d=e f=g h =i", NULL, BUILTIN_SETENV_USAGE, NULL);
	compare_fds_w_strings("Builtin setenv 14 - w/o valid args 5", "setenv a=b b=c d=e f=g h= i", NULL, BUILTIN_SETENV_USAGE, NULL);

	compare_sh_42sh_outputs("Builtin setenv 15 - Simple assign", "setenv lol= && env | grep 'lol='", "export lol= && env | cut -d ' ' -f 2 | grep 'lol='");
	//compare_sh_42sh_outputs("Builtin setenv 1 - Expansions of new variables", "setenv LOL=LAL; echo $LOL", "export LOL=LAL; echo $LOL"); waiting ast expansions rework
	compare_sh_42sh_outputs("Builtin setenv 16 - w/ valid args 2", "setenv a=b c=d && env | grep -E '^[ac]=' | sort", "export a=b c=d && env | grep -E '^[ac]=' | sort");
	compare_sh_42sh_outputs("Builtin setenv 17 - w/ valid args 3", "setenv a=b b=c d=e f=g h= && env | grep -E '^[a-h]=' | sort", "export a=b b=c d=e f=g h= && env | grep -E '^[a-h]=' | sort");
	compare_sh_42sh_outputs("Builtin setenv 18 - w/ many valid args", "setenv a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a1 a1=b b1=c c1=d d1=e e1=f f1=g g1=h h1=i i1=j j1=k k1=l l1=m m1=n n1=o o1=p p1=q q1=r && env | grep -E '^[a-z][1]?=' | sort", \
		"export a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a1 a1=b b1=c c1=d d1=e e1=f f1=g g1=h h1=i i1=j j1=k k1=l l1=m m1=n n1=o o1=p p1=q q1=r && env | grep -E '^[a-z][1]?=' | sort");
	compare_sh_42sh_outputs("Builtin setenv 19 - valid args w/ re-assignations", "setenv a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a a=b b=c c=d d=e e=f f=g g=h h=i i=j j=k k=l l=m m=n n=o o=p p=q q=r && env | grep -E '^[a-z]=' | sort", \
		"export a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a a=b b=c c=d d=e e=f f=g g=h h=i i=j j=k k=l l=m m=n n=o o=p p=q q=r && env | grep -E '^[a-z]=' | sort");
	compare_sh_42sh_outputs("Builtin setenv 20 - w/o valid args 6", "setenv a=b b=c d=e f=gh=i && env | grep -E '^[a-f]=' | sort", \
		"export a=b b=c d=e f=gh=i && env | grep -E '^[a-f]=' | sort");

	compare_fds_w_strings("Builtin unsetenv 1 - w/o args", "unsetenv", NULL, BUILTIN_UNSETENV_USAGE, NULL);
	compare_fds_w_strings("Builtin unsetenv 2 - err check 1", "unsetenv ==", NULL, BUILTIN_UNSETENV_USAGE, NULL);
	compare_sh_42sh_outputs("Builtin unsetenv 3 - basic unset", \
		"setenv a=b && unsetenv a && env | grep -E '^a='", \
		"export a=b && unset a && env | grep -E '^a='");
	compare_sh_42sh_outputs("Builtin unsetenv 4 - basic unset w/ many vars", \
		"setenv a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a a=b b=c c=d d=e e=f f=g g=h h=i i=j j=k k=l l=m m=n n=o o=p p=q q=r && unsetenv a b c d e f g h i j k l m n o p q && env | grep -E '^[a-q]='", \
		"export a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a a=b b=c c=d d=e e=f f=g g=h h=i i=j j=k k=l l=m m=n n=o o=p p=q q=r && unset a b c d e f g h i j k l m n o p q && env | grep -E '^[a-q]='");


	if (*_OS_ == 'D')
	{
		compare_fds_w_strings("Builtin read 1 - Simple", "read -- ; echo \\$REPLY", "A\n", "", "A");
		compare_fds_w_strings("Builtin read 2 - Simple w/ vars", "read -- REP; echo \\$REP", "A\n", "", "A");
		compare_fds_w_strings("Builtin read 3 - Simple w/ multiple vars", "read -- REP REP2; echo \\$REP", "A\n", "", "A B C");
		compare_fds_w_strings("Builtin read 4 - Simple w/ multiple vars 2", "read -- REP REP2; echo \\$REP2", "B C\n", "", "A B C");
		compare_fds_w_strings("Builtin read 5 - -d", "read -d q -- VAR1 ; echo \\$VAR1", "abc\n", "", "abcqwert");
		compare_fds_w_strings("Builtin read 6 - -d", "read -d q -- VAR1 ; echo \\$VAR1", "\n", "", "qabcqwert");
		compare_fds_w_strings("Builtin read 7 - -d", "read -d q -- VAR1 VAR2 ; echo \\$VAR1 ; echo \\$VAR2", "\n\n", "", "qabcqwert");
		compare_fds_w_strings("Builtin read 8 - -d", "read -d q -- VAR1 VAR2 ; echo \\$VAR1 ; echo \\$VAR2", "a\nbc\n", "", "a bcqwert");
		compare_fds_w_strings("Builtin read 9 - -d", "read -d q -- VAR1 ; echo \\$VAR1", "a bc\n", "", "a bcqwert");
		compare_fds_w_strings("Builtin read 10 - invalid 1", "read -d qq -- VAR1 VAR2 ; echo \\$VAR1 ; echo \\$VAR2", "\n\n", BUILTIN_READ_USAGE  , "a bcqwert");
		compare_fds_w_strings("Builtin read 11 - invalid 2", "read -d q - VAR1 VAR2 ; echo \\$VAR1", "\n", BUILTIN_READ_USAGE  , "a bcqwert");
		compare_fds_w_strings("Builtin read 12 - invalid 3", "read -d q  VAR1 VAR2 ; echo \\$VAR1", "\n", BUILTIN_READ_USAGE  , "a bcqwert");
		compare_fds_w_strings("Builtin read 13 - invalid 4", "read -d q  VAR1 -- VAR2 ; echo \\$VAR1", "\n", BUILTIN_READ_USAGE  , "a bcqwert");
		compare_fds_w_strings("Builtin read 14 - n", "read -n 3 -- VAR1 ; echo \\$VAR1", "a b\n", "", "a bcqwert");
		compare_fds_w_strings("Builtin read 15 - invalid n N", "read -N -d a 3 -- VAR1 ; echo \\$VAR1", "\n", BUILTIN_READ_USAGE, "a bcqwert");
		compare_fds_w_strings("Builtin read 16 - N", "read -N 3 -- VAR1 ; echo \\$VAR1", "a b\n", "", "a bcqwert");
		compare_fds_w_strings("Builtin read 17 - N n", "read -N 3 -n 4 -- VAR1 ; echo \\$VAR1", "\n", BUILTIN_READ_USAGE, "a bcqwert");
		compare_fds_w_strings("Builtin read 18 - N w 2 vars", "read -N 3 -- VAR1 VAR2 ; echo \\$VAR1 ; echo \\$VAR2", "a\nb\n", "", "a bcqwert");
		compare_fds_w_strings("Builtin read 19", "read -N 3 -N 5 -- VAR1 ; echo \\$VAR1", "a bcd\n", "", "a bcdefgh");
		compare_fds_w_strings("Builtin read 20", "read -d q -n 5 -p -s -- VAR1 VAR2 VAR3; echo \\$VAR1 ; echo \\$VAR2 ; echo \\$VAR3", "a\nbcd\n\n", "", "a bcdefgh");
		compare_fds_w_strings("Builtin read 21", "read -d q -n 5 -p -s  VAR1 VAR2 VAR3; echo \\$VAR1 ; echo \\$VAR2 ; echo \\$VAR3", "\n\n\n", BUILTIN_READ_USAGE, "a bcdefgh");
	} // '<<<' Redirects not supported by linux's sh
}
