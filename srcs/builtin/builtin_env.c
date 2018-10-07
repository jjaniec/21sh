/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/27 19:40:20 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/05 18:02:40 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** https://www.unix.com/man-page/posix/1posix/env
*/

t_option		g_env_opts[] = {
	{{"h", "-help"}, "Print help and exit", false},
	{{"i"}, "Invoke utility with exactly the environment specified by the \
		arguments; the inherited environment will be ignored completely.", \
		false},
	{{NULL}, NULL, false}
};

static int		print_env_content(char **environ)
{
	while (environ && *environ)
		ft_putendl(*environ++);
	return (0);
}

static char	*handle_parameters(char **argv, t_environ *env_struct, \
				t_environ **env_struct_to_use, char ***tmp_environ_start)
{
	int				starting_entry_count;

	argv = parse_options(NULL, argv, g_env_opts, NULL);
	if (is_option_activated("i", g_env_opts, NULL))
	{
		(*env_struct_to_use) = malloc(sizeof(t_environ));
		init_environ_struct_ptrs((*env_struct_to_use));
	}
	else
		(*env_struct_to_use) = env_struct;
	*tmp_environ_start = \
		&((*env_struct_to_use)->environ[(*env_struct_to_use)->entry_count]);
	starting_entry_count = (*env_struct_to_use)->entry_count;
	while (argv && *argv && ft_strchr(*argv, '='))
	{
		if (!((*env_struct_to_use)->entry_count < MAX_ENV_ENTRIES))
			break ;
		if ((*env_struct_to_use)->get_var((*env_struct_to_use), *argv))
		{
			ft_strncpy((*env_struct_to_use)->last_used_elem->entry, *argv, MAX_ENV_ENTRY_LEN);
			(*env_struct_to_use)->entry_count += 1;
		}
		else
			(*env_struct_to_use)->add_var((*env_struct_to_use), *argv, NULL);
		argv += 1;
	}
	(*env_struct_to_use)->environ[(*env_struct_to_use)->entry_count] = NULL;
	return *argv;
}



int			builtin_env_(char **argv, t_environ *env)
{
	t_environ		*env_struct_to_use;
	char			*command_begin_ptr;
	char			**tmp_environ_start;

	if (env)
	{
		if (!((command_begin_ptr = handle_parameters(argv, env, &env_struct_to_use, &tmp_environ_start))))
			print_env_content(env_struct_to_use->environ);
		//else
		//	exec_command_passed_in_last_parameters(command_begin_ptr);
		if (env_struct_to_use != env)
			free(env_struct_to_use);
		else
			while (*tmp_environ_start)
				*tmp_environ_start++ = NULL;
		return (0);
	}
	return (1);
}

void			builtin_env(char **argv, t_environ *env, t_exec *exe)
{
	(void)argv;
	(void)exe;

	if (argv[1])
		builtin_env_(argv, env);
	else
		print_env_content(env->environ);
	if (exe)
		exe->ret = 0;
	//exe->ret = (argv[1] && builtin_env_(argv, env)) ??!??! print_env_content(env->environ);
	//exe->ret = builtin_env_(argv, env);
}
