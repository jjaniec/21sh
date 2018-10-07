/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_tests.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/30 20:38:39 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/05 19:24:17 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

void		builtins_tests(t_environ *env)
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
	//compare_sh_42sh_outputs("Builtin cd 11 - cd - ", "mkdir janiec; cd janiec ; pwd ; chmod 000 . ; cd ; pwd ; cd - 2> /dev/null; pwd", NULL);
	//compare_sh_42sh_outputs("Builtin cd 12 - cd", "cd janiec; pwd; chmod 777 janiec; rm -rf janiec", NULL);


	compare_sh_42sh_outputs("Builtin env 1 - env w/o args w/ pipe", "env | grep -v", NULL);
	//compare_sh_42sh_outputs("Builtin env 2 - env w/ T_ENV_ASSIGN", "TMP=test env | grep TMP", NULL); // 15/09: Not implemented yet
	compare_sh_42sh_outputs("Builtin env 3 - env w/ valid args & pipe", "env TEST1=TEST__ TEST2=TEST______ | grep -v _", "export TEST1=TEST__ TEST2=TEST______ | grep -v _");
	compare_sh_42sh_outputs("Builtin env 4 - env -i w/ valid args", "env -i A=B TEST1=TEST__ TEST2=TEST______", "env -i A=B TEST1=TEST__ TEST2=TEST______");
	compare_sh_42sh_outputs("Builtin env 5 - env -i w/ valid args & pipe", "env -i A=B TEST1=TEST__ TEST2=TEST______", NULL);
	compare_sh_42sh_outputs("Builtin env 6 - env -i w/o args w/ pipe", "env -i", NULL);
	compare_sh_42sh_outputs("Builtin env 7 - env -i empty assignations", "env -i LS=     AAAA=   ", NULL);
	//compare_sh_42sh_outputs("Builtin env 8 - env -i invalid assignations", "env -i LS=     AAAA  AAAAAA  LOL=   | grep -v _", NULL); -> should say command not found -> exec not yet handled

	// test env execution ex: env A=B ls - not yet implemented
	//compare_sh_42sh_outputs("Builtin env 6 - env -i w/ valid args & execution", "env -i A=B TEST1=TEST__ TEST2=TEST______ ls", NULL);
	//compare_sh_42sh_outputs("Builtin env 7 - env -i w/o valid args", "env -i A", NULL);
	//compare_sh_42sh_outputs("Builtin env 8 - env -i w/o valid args", "env -i ls", NULL);
	//compare_sh_42sh_outputs("Builtin env 9 - env -i w/ assign & execution", "env -i HOME=idontexist ls $HOME", NULL);

	compare_sh_42sh_outputs("Builtin setenv 1 - Expansions of new variables", "setenv LOL=LAL\" \"echo $LOL", "export LOL=LAL; echo $LOL");
/*
	compare_sh_42sh_outputs("Builtin setenv 1 - w/o args", "setenv", "export | cut -d ' ' -f 2");
	compare_sh_42sh_outputs("Builtin setenv 1 - w/o valid args", "setenv lol", "export | cut -d ' ' -f 2");
	compare_sh_42sh_outputs("Builtin setenv 1", "setenv lol=", "export | cut -d ' ' -f 2");
	//compare_sh_42sh_outputs("Builtin setenv 1 - Expansions of new variables", "setenv LOL=LAL; echo $LOL", "export LOL=LAL; echo $LOL");
	compare_sh_42sh_outputs("Builtin setenv 2 - w/o valid args 1", "setenv a", "export a");
	compare_sh_42sh_outputs("Builtin setenv 3 - w/o valid args 2", "setenv a=b b=c d=e f=g h=", "export a=b b=c d=e f=g h=");
	compare_sh_42sh_outputs("Builtin setenv 4 - w/o valid args 3", "setenv a=b b=c d=e f=g =i", "export a=b b=c d=e f=g =i");
	compare_sh_42sh_outputs("Builtin setenv 5 - w/o valid args 4", "setenv a=b b=c d=e f=g h =i", "export a=b b=c d=e f=g h =i");
	compare_sh_42sh_outputs("Builtin setenv 6 - w/o valid args 5", "setenv a=b b=c d=e f=g h= i", "export a=b b=c d=e f=g h= i");
	compare_sh_42sh_outputs("Builtin setenv 7 - w/o valid args 6", "setenv a=b b=c d=e f=gh=i", "export a=b b=c d=e f=gh=i");
	compare_sh_42sh_outputs("Builtin setenv 8 - w/ valid args", "setenv a=b c=d", "export a=b c=d");
	compare_sh_42sh_outputs("Builtin setenv 9 - w/ many valid args", "setenv a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a1 a1=b b1=c c1=d d1=e e1=f f1=g g1=h h1=i i1=j j1=k k1=l l1=m m1=n n1=o o1=p p1=q q1=r && env | grep -v _", \
		"export a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a1 a1=b b1=c c1=d d1=e e1=f f1=g g1=h h1=i i1=j j1=k k1=l l1=m m1=n n1=o o1=p p1=q q1=r && env | grep -v _");
	compare_sh_42sh_outputs("Builtin setenv 10 - valid args w/ re-assignations", "setenv a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a a=b b=c c=d d=e e=f f=g g=h h=i i=j j=k k=l l=m m=n n=o o=p p=q q=r && env | grep -v _", \
		"export a=b c=d e=f g=h i=j k=l m=n o=p q=r s=t u=v x=y z=a a=b b=c c=d d=e e=f f=g g=h h=i i=j j=k k=l l=m m=n n=o o=p p=q q=r && env | grep -v _");
*/
/*
	compare_sh_42sh_outputs("Builtins 17 - setenv basic", "env | grep test____ || setenv test____ tmp && env | grep test____", \
		"{ env | grep test____ || export test____=tmp && env | grep test____ ;}");
	compare_sh_42sh_outputs("Builtins 18 - setenv basic without env", "env | grep test____ || setenv test____ tmp && env | grep test____", \
		"{ env | grep test____ || export test____=tmp && env | grep test____ ;}");
*/
	system(SH_EXEC_CMD_PREFIX"\"unsetenv tmp && env\"");
	//ok((*envp_ptr == NULL), "Builtins 19 - unsetenv without env");
	/* 15/09: does not work
	*envp_ptr = (char **)cp_envp((const char *[3]){"tmp=LOL", "tmp2=LOL", NULL});
	system(SH_EXEC_CMD_PREFIX"\"unsetenv tmp\"");
	ok((envp_ptr[1] == NULL), "Builtins 20 - unsetenv basic");
	*envp_ptr = (char **)cp_envp((const char *[3]){"tmp=LOL", "tmp2=LOL", NULL});
	system(SH_EXEC_CMD_PREFIX"\"unsetenv tmp tmp2\"");
	ok((envp_ptr[0] == NULL), "Builtins 21 - unsetenv basic multiple elems");
	*envp_ptr = (char **)cp_envp((const char *[3]){"tmp=LOL", "tmp2=LOL", NULL});
	system(SH_EXEC_CMD_PREFIX"\"unsetenv tmp tmp2 wwwww\"");
	ok((envp_ptr[0] == NULL), "Builtins 21 - unsetenv basic multiple elems w/ non-existant");
	*envp_ptr = (char **)cp_envp((const char *[3]){"tmp=LOL", "tmp2=LOL", NULL});
	system(SH_EXEC_CMD_PREFIX"\"unsetenv tmp tmp tmp tmp\"");
	ok((envp_ptr[1] == NULL), "Builtins 21 - unsetenv basic already deleted");*/
	//*envp_ptr = env; //restore env
}
