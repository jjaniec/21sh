/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 17:46:06 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/05 19:29:28 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Posix options
** https://www.unix.com/man-page/posix/1posix/cd/
*/

t_option		g_cd_opts[] = {
	{{"h", "-help"}, "Print help and exit", false},
	{{"P"}, "Handle the operand dot-dot physically; symbolic link \
		components shall be resolved before dot-dot components are processed", false},
	{{"L"}, "Handle the operand dot-dot logically; symbolic link \
		components shall not be resolved before dot-dot components are processed", false},
	{{NULL}, NULL, false}
};

/*
** Refresh PWD env variable
*/

static void	ft_refresh_cwd_env(t_environ *env)
{
	char	cwd_new_fmt[MAX_ENV_ENTRY_LEN];

	if (getcwd(cwd_new_fmt, sizeof(cwd_new_fmt)))
		env->upt_var(env, "PWD", cwd_new_fmt);
	else
		ft_putstr_fd(SH_NAME": .: Cannot get current working directory !\n", 2);
}

/*
** Print cd error according to errno
*/

static void	ft_print_cd_err(char *path, int errno_err)
{
	ft_putstr_fd(SH_NAME": ", 2);
	ft_putstr_fd(path, 2);
	if (errno_err == EACCES)
		ft_putstr_fd(": permission denied\n", 2);
	else if (errno_err == ENOENT)
		ft_putstr_fd(": does not exists\n", 2);
	else if (errno_err == ENOTDIR)
		ft_putstr_fd(": not a directory\n", 2);
	else
		exit(MALLOC_ERROR);
}

/*
** Change current directory
** here we use MAX_ENV_ENTRY_LEN as max path len
** so we can update properly the PWD and OLDPWD env variables
*/

static void	ft_change_dir(t_environ *env, char *path, char *cwd)
{
	if (path && !chdir(path))
	{
		if (env->get_var(env, "OLDPWD"))
			ft_strncpy(env->last_used_elem->val_begin_ptr, cwd, MAX_ENV_ENTRY_LEN - 7);
		else
			env->add_var(env, "OLDPWD", cwd);
		log_debug("OLDPWD set to |%s|", env->last_used_elem->val_begin_ptr);
	}
	else
	{
		log_error("Failed to change cwd to %s", path);
		ft_print_cd_err(path, errno);
	}
}

/*
** Inspect cd argument to know if passed argument is a relative/absolute path
** or if argument is '-', then refresh last previous location
*/
/*
static int	ft_cd_relative_dir(t_environ *env, char *rel_path, int getcwd_ret, char *cwd)
{
	char	*new_dir_path;
	char	cwd_fmt[MAX_ENV_ENTRY_LEN];

	if (getcwd_ret)
	{
		if ((new_dir_path = ft_strjoin_path(cwd, rel_path)))
		{
			ft_change_dir(env, new_dir_path);
			free(new_dir_path);
			return (0);
		}
	}
	return (1);
}*/

void		builtin_cd(char **argv, t_environ *env, t_exec *exe)
{
	char		cwd[MAX_ENV_ENTRY_LEN];
	(void)exe;

	if (!(getcwd(cwd, MAX_ENV_ENTRY_LEN)))
	{
		if (errno == EACCES)
		{
			exe->ret = 1;
			ft_putstr_fd(SH_NAME ": cd: permission denied\n", 2);
		}
		else
			exit(MALLOC_ERROR);
		return ;
	}
	if (!argv[1])
	{
		if (env->get_var(env, "HOME"))
			ft_change_dir(env, env->last_used_elem->val_begin_ptr, cwd);
		else
		{
			ft_putstr_fd(SH_NAME": cd: HOME not set\n", 2);
			exe->ret = 1;
			return ;
		}
	}
	else if (!ft_strcmp(argv[1], "-"))
	{
		if (env->get_var(env, "OLDPWD"))
		{
			ft_putendl(env->last_used_elem->val_begin_ptr);
			ft_change_dir(env, env->last_used_elem->val_begin_ptr, cwd);
		}
		else
		{
			ft_putstr_fd(SH_NAME": cd: OLDPWD not set\n", 2);
			exe->ret = 1;
			return ;
		}
	}
	else if (argv[1]) //	//else if (ft_strchr(argv[1], '/')
		ft_change_dir(env, argv[1], cwd);
	//else if (argv[1])
	//	ft_cd_relative_dir(env, argv[1], getcwd_ret, cwd);
	ft_refresh_cwd_env(env);
	exe->ret = 0;
}
