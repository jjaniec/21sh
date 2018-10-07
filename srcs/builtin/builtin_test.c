/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 12:13:57 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/01 18:56:14 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Posix options
** http://pubs.opengroup.org/onlinepubs/9699919799/utilities/test.html
*/

t_option		g_tests_opts[] = {
	{{"h"}, "-h:\t\tPrint this help with all the options listed.", false},
	{{"b"}, "-b file:\tTrue if file exists and is a block special file.", false},
	{{"c"}, "-c file:\tTrue if file exists and is a character special file.", false},
	{{"d"}, "-d file:\tTrue if file exists and is a directory.", false},
	{{"e"}, "-e file:\tTrue if file exists (regardless of type).", false},
	{{"f"}, "-f file:\tTrue if file exists and is a regular file.", false},
	{{"k"}, "-k file:\tTrue if file exists and its sticky bit is set.", false},
	{{"p"}, "-p file:\tTrue if file is a named pipe (FIFO).", false},
	{{"r"}, "-r file:\tTrue if file exists and is readable.", false},
	{{"s"}, "-s file:\tTrue if file exists and has a size greater than zero.", false},
	{{"u"}, "-u file:\tTrue if file exists and its set user ID flag is set.", false},
	{{"w"}, "-w file:\tTrue if file exists and is writable.\n\
			\tTrue indicates only that the write flag is on.\n\
			\tThe file is not writable on a read-only file system even if this test indicates true.", false},
	{{"x"}, "-x file:\tTrue if file exists and is executable.\n\
			\tTrue indicates only that the execute flag is on.\n\
			\tThe file is not executable on a read-only file system even if this test indicates true.", false},
	{{"L"}, "-L file:\tTrue if file exists and is a symbolic link.", false},
	{{"S"}, "-S file:\tTrue if file exists and is a socket.", false},
	{{"--------------------------"}, "" , false},
	{{"\e="}, "s1 = s2:\tTrue if the strings s1 and s2 are identical.", false},
	{{"\e!="}, "s1 != s2:\tTrue if the strings s1 and s2 are not identical.", false},
	{{"\e<"}, "s1 < s2:\tTrue if the strings s1 comes before s2 based on the binary value of their characters.", false},
	{{"\e>"}, "s1 > s2:\tTrue if the strings s1 comes after s2 based on the binary value of their characters.", false},
	{{"--------------------------"}, "" , false},
	{{"eq"}, "n1 -eq n2:\tTrue if the intengers n1 and n2 are algebraically equal.", false},
	{{"ne"}, "n1 -ne n2:\tTrue if the intengers n1 and n2 are not algebraically equal.", false},
	{{"gt"}, "n1 -gt n2:\tTrue if the intengers n1 is algebraically greater than the integer n2.", false},
	{{"ge"}, "n1 -ge n2:\tTrue if the intengers n1 is algebraically greater than or equal to the integer n2.", false},
	{{"lt"}, "n1 -lt n2:\tTrue if the intengers n1 is algebraically less than the integer n2.", false},
	{{"le"}, "n1 -le n2:\tTrue if the intengers n1 is algebraically less than or equal to the integer n2.", false},
	{{NULL}, NULL, false}
};

static int	right_format_builtin(char **argv, int *argc)
{
	if (ft_strequ(argv[0], "test"))
		return (1);
	else if (ft_strequ(argv[0], "[") && ft_strequ(argv[*argc - 1], "]"))
	{
		argv[*argc - 1] = NULL;
		(*argc)--;
		return (1);
	}
	else
		return (0);
}

static int	parse_expr_file(char **argv, t_option *opt_list, \
			t_option *char_opt_index[CHAR_OPT_INDEX_SIZE])
{
	struct stat	fstat;

	errno = 0;
	stat(argv[1], &fstat);
	if (errno != ENOENT && is_option_activated("e", opt_list, char_opt_index))
		return (0);
	else if (errno != 0)
		return (1);
	else if (is_option_activated("b", opt_list, char_opt_index) \
	&& S_ISBLK(fstat.st_mode))
		return (0);
	else if (is_option_activated("c", opt_list, char_opt_index) \
	&& S_ISCHR(fstat.st_mode))
		return (0);
	else if (is_option_activated("d", opt_list, char_opt_index) \
	&& S_ISDIR(fstat.st_mode))
		return (0);
	else if (is_option_activated("f", opt_list, char_opt_index) \
	&& S_ISREG(fstat.st_mode))
		return (0);
	else if (is_option_activated("p", opt_list, char_opt_index) \
	&& S_ISFIFO(fstat.st_mode))
		return (0);
	else if (is_option_activated("L", opt_list, char_opt_index) \
	&& S_ISLNK(fstat.st_mode))
		return (0);
	else if (is_option_activated("S", opt_list, char_opt_index) \
	&& S_ISSOCK(fstat.st_mode))
		return (0);
	else if (is_option_activated("r", opt_list, char_opt_index) \
	&& fstat.st_mode & S_IRUSR)
		return (0);
	else if (is_option_activated("w", opt_list, char_opt_index) \
	&& fstat.st_mode & S_IWUSR)
		return (0);
	else if (is_option_activated("x", opt_list, char_opt_index) \
	&& fstat.st_mode & S_IXUSR)
		return (0);
	else if (is_option_activated("s", opt_list, char_opt_index) \
	&& fstat.st_size > 0)
		return (0);
	else if (is_option_activated("k", opt_list, char_opt_index) \
	&& fstat.st_mode & S_ISVTX)
		return (0);
	else if (is_option_activated("u", opt_list, char_opt_index) \
	&& fstat.st_mode & S_ISUID)
		return (0);
	else
		return (1);
}

static int	parse_expr_comp(char **argv)
{
	t_option	*opt_list;
	t_option	*char_opt_index[CHAR_OPT_INDEX_SIZE];
	int			ac;
	char		**args;

	ac = 0;
	while (argv[ac])
		ac++;
	opt_list = g_tests_opts;
	args = parse_options(&ac, argv + 1, opt_list, (t_option **)char_opt_index);
	if (ft_strequ(argv[1], "=") && ft_strequ(argv[0], argv[2]))
		return (0);
	else if (ft_strequ(argv[1], "!=") && !ft_strequ(argv[0], argv[2]))
		return (0);
	else if (ft_strequ(argv[1], "<") && ft_strcmp(argv[0], argv[2]) < 0)
		return (0);
	else if (ft_strequ(argv[1], ">") && ft_strcmp(argv[0], argv[2]) > 0)
		return (0);
	else if (is_option_activated("eq", opt_list, char_opt_index) \
	&& ft_atoi(argv[1]) == ft_atoi(argv[2]))
		return (0);
	else if (is_option_activated("ne", opt_list, char_opt_index) \
	&& ft_atoi(argv[1]) != ft_atoi(argv[2]))
		return (0);
	else if (is_option_activated("gt", opt_list, char_opt_index) \
	&& ft_atoi(argv[1]) > ft_atoi(argv[2]))
		return (0);
	else if (is_option_activated("ge", opt_list, char_opt_index) \
	&& ft_atoi(argv[1]) >= ft_atoi(argv[2]))
		return (0);
	else if (is_option_activated("lt", opt_list, char_opt_index) \
	&& ft_atoi(argv[1]) < ft_atoi(argv[2]))
		return (0);
	else if (is_option_activated("le", opt_list, char_opt_index) \
	&& ft_atoi(argv[1]) <= ft_atoi(argv[2]))
		return (0);
	else
		return (1);
}

void		builtin_test(char **argv, t_environ *env, t_exec *exe)
{
	t_option	*opt_list;
	t_option	*char_opt_index[CHAR_OPT_INDEX_SIZE];
	int			ac;

	(void)exe;
	(void)env;

	ac = 0;
	while (argv[ac])
		ac++;

	opt_list = g_tests_opts;
	parse_options(&ac, argv, opt_list, (t_option **)char_opt_index);

	ac = 0;
	while (argv[ac])
		ac++;

	if (!argv[0])
		exit (0);
	else if (is_option_activated("h", opt_list, char_opt_index) || \
	is_option_activated("--------------------------", opt_list, char_opt_index)\
	|| !right_format_builtin(argv, &ac))
	{
		format_help(BUILTIN_TEST_USAGE, opt_list);
		exit (1);
	}
	else if (!argv[1] && ft_strlen(argv[0]) > 0)
		exit (1);
	else if (argv[1] && !argv[2])
		exit (ft_atoi(argv[1]));
	else if (argv[2] && !argv[3])
		exit (parse_expr_file((argv + 1), opt_list, char_opt_index));
	else if (argv[3] && !argv[4])
		exit (parse_expr_comp(argv + 1));
	format_help(BUILTIN_TEST_USAGE, opt_list);
	exit (1);
}
