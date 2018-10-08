/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_test.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbrucker <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/01 12:13:57 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/08 14:09:44 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Posix options
** http://pubs.opengroup.org/onlinepubs/9699919799/utilities/test.html
*/

t_option		g_tests_opts[] = {
	{{"\n"}, "If expression is an integer, it becames the value returned.", false},
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
	{{"\n="}, "s1 = s2:\tTrue if the strings s1 and s2 are identical.", false},
	{{"\n!="}, "s1 != s2:\tTrue if the strings s1 and s2 are not identical.", false},
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
	int		ac;

	ac = 0;
	while (argv[ac])
		ac++;
	if (ac > 0 && ft_strequ(argv[0], "test"))
		return (1);
	else if (ft_strequ(argv[0], "[") && ft_strequ(argv[ac - 1], "]"))
	{
		free(argv[ac - 1]);
		argv[ac - 1] = NULL;
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
	else if (is_option_activated("L", opt_list, char_opt_index) \
	&& S_ISLNK(fstat.st_mode))
		return (0);
	else if (is_option_activated("S", opt_list, char_opt_index) \
	&& S_ISSOCK(fstat.st_mode))
		return (0);
	else if (argv[0][0] != '-')
	{
		ft_putstr_fd("21sh: ", 2);
		ft_putstr_fd(argv[-1], 2);
		ft_putchar_fd(' ', 2);
		ft_putchar_fd(argv[0][0], 2);
		ft_putendl_fd(" : unary operator expected", 2);
	}
		return (1);
}

static void	error_msg_operator(char **argv)
{
	if (!ft_strequ(argv[1], "=")
	 && !ft_strequ(argv[1], "!=")
	 && !ft_strequ(argv[1], "<")
	 && !ft_strequ(argv[1], ">")
	 && !ft_strequ(argv[1], "-eq")
	 && !ft_strequ(argv[1], "-ne")
	 && !ft_strequ(argv[1], "-ge")
	 && !ft_strequ(argv[1], "-gt")
	 && !ft_strequ(argv[1], "-le")
	 && !ft_strequ(argv[1], "-lt"))
	{
		ft_putstr_fd("21sh: ", 2);
		ft_putstr_fd(argv[-1], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putendl_fd(": binary operator expected", 2);
	}
}

static int	parse_expr_comp(char **argv)
{
	error_msg_operator(argv);
	if (ft_strequ(argv[1], "=") && ft_strequ(argv[0], argv[2]))
		return (0);
	else if (ft_strequ(argv[1], "!=") && !ft_strequ(argv[0], argv[2]))
		return (0);
	else if (ft_strequ(argv[1], "-eq") && ft_atoll(argv[0]) == ft_atoll(argv[2]))
		return (0);
	else if (ft_strequ(argv[1], "-ne") && ft_atoll(argv[0]) != ft_atoll(argv[2]))
		return (0);
	else if (ft_strequ(argv[1], "-gt") && ft_atoll(argv[0]) > ft_atoll(argv[2]))
		return (0);
	else if (ft_strequ(argv[1], "-ge") && ft_atoll(argv[0]) >= ft_atoll(argv[2]))
		return (0);
	else if (ft_strequ(argv[1], "-lt") && ft_atoll(argv[0]) < ft_atoll(argv[2]))
		return (0);
	else if (ft_strequ(argv[1], "-le") && ft_atoll(argv[0]) <= ft_atoll(argv[2]))
		return (0);
	else
		return (1);
}

static int	test_expression(char **argv, \
			t_option *char_opt_index[CHAR_OPT_INDEX_SIZE])
{
	if (!argv[0])
		return (0);
	else if (is_option_activated("h", g_tests_opts, char_opt_index) || \
	is_option_activated("--------------------------", g_tests_opts, char_opt_index))
		format_help(BUILTIN_TEST_USAGE, g_tests_opts);
	else if (!argv[1] && ft_strlen(argv[0]) > 0)
		return (1);
	else if (argv[1] && !argv[2])
		return (ft_atoi(argv[1]));
	else if (argv[2] && !argv[3])
		return (parse_expr_file((argv + 1), g_tests_opts, char_opt_index));
	else
		format_help(BUILTIN_TEST_USAGE, g_tests_opts);
	return (1);
}

static int	return_value(int ret, int flag_not)
{
	if (flag_not)
	{
		if (ret == 0)
			return (1);
		else
			return (0);
	}
	else
		return (ret);
}

static void	remove_operator_not(char **argv)
{
	int		i;

	ft_strdel(argv + 1);
	i = 2;
	while (argv[i])
	{
		argv[i - 1] = argv[i];
		i++;
	}
	argv[i - 1] = NULL;
}

void		builtin_test(char **argv, char **envp, t_exec *exe)
{
	t_option	*opt_list;
	t_option	*char_opt_index[CHAR_OPT_INDEX_SIZE];
	int			ac;
	int			flag_not;

	(void)exe;
	(void)envp;

	ac = 0;
	while (argv[ac])
		ac++;
	flag_not = 0;
	if (ac >= 2 && ft_strequ(argv[1], "!"))
	{
		ac--;
		flag_not = 1;
		remove_operator_not(argv);
	}
	if (!right_format_builtin(argv, &ac))
	{
		format_help(BUILTIN_TEST_USAGE, g_tests_opts);
		exit (return_value(1, flag_not));
	}
	if (ac == 4 && argv[3] && !argv[4])
		exit (return_value(parse_expr_comp(argv + 1), flag_not));
	opt_list = g_tests_opts;
	parse_options(&ac, argv, opt_list, (t_option **)char_opt_index);
	exit (return_value(test_expression(argv, char_opt_index), flag_not));
}
