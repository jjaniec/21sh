/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <cgaspart@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/17 12:19:55 by cgaspart          #+#    #+#             */
/*   Updated: 2018/11/27 19:52:27 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include <forty_two_sh.h>

typedef struct		s_cd
{
	char			*cwd;
	char			*cwd_link;
	bool			link;
	t_exec			*exe;
	t_environ		*env;
}					t_cd;

void				builtin_exit(char **argv, t_environ *env, t_exec *exe);
void				builtin_env(char **argv, t_environ *env, t_exec *exe);
void				builtin_setenv(char **argv, t_environ *env, t_exec *exe);
void				builtin_unsetenv(char **argv, t_environ *env, t_exec *exe);
void				builtin_return(char **argv, t_environ *env, t_exec *exe);
void				builtin_toggle_syntax_highlighting(char **argv,
					t_environ *env, t_exec *exe);
void				builtin_test(char **argv, t_environ *env, t_exec *exe);
void				builtin_history(char **argv, t_environ *env, t_exec *exe);
void				builtin_toggle_syntax_highlighting(char **argv,
					t_environ *env, t_exec *exe);
void				builtin_alias(char **argv, t_environ *env, t_exec *exe);
void				builtin_unalias(char **argv, t_environ *env, t_exec *exe);
void				builtin_cd(char **argv, t_environ *env, t_exec *exe);
bool				cd_in_link(t_environ *env);
int					cd_check_link(t_cd *cd_info, char *av);
char				*cd_add_slash(char *str);
void				cd_l(t_cd *cd_i, char *av);
void				builtin_cd_p(t_cd *cd_info, char *argv);
void				cd_home(t_cd *cd_info);
int					cd_change_dir(t_environ *env, char *path, char *cwd);
char				*cd_clean_last_slash(char *str);
char				*cd_get_last_path(char *path);
void				dot_manager(t_cd *cd_i, char *av);
void				link_env_update(t_cd *cd_i);
void				refresh_cwd_env(t_environ *env);
void				cd_free_struct(t_cd *cd_i);
void				builtin_echo(char **argv, t_environ *env, t_exec *exe);
int					echo_n_opt(void);
int					echo_a_opt(void);
int					echo_b_opt(void);
int					echo_c_opt(void);
int					echo_f_opt(void);
int					echo_r_opt(void);
int					echo_t_opt(void);
int					echo_v_opt(void);
int					echo_bslash_opt(void);
#endif
