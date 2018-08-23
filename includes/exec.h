/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 13:04:09 by sbrucker          #+#    #+#             */
/*   Updated: 2018/08/19 22:20:11 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <twenty_one_sh.h>

/*
** Default redirection file descs
** ex: "ls > a.txt" will redirect stdout to a.txt as
** stdout is the default output redirection file desc
*/

# define DEFAULT_INPUT_REDIR_FD STDIN_FILENO
# define DEFAULT_OUTPUT_REDIR_FD STDOUT_FILENO

/*
** Default mode for files created w/ a redirection
*/

# define DEFAULT_OUTPUT_REDIR_FILE_MODE 0644

t_exec	*exec_cmd(t_ast *root, t_exec *exe);
void	exec_argv(char **argv, char **envp, t_exec *exe, t_ast *node);
t_exec	*exec_thread(char *cmd, char **argv, char **envp, t_exec *exe, \
		t_ast *node);
t_exec	*pre_exec(t_ast *node, t_exec *exe);
t_exec	*in_exec(t_ast *node, t_exec *exe);
t_exec	*post_exec(t_ast *node, t_exec *exe);
void	exec_local(char **argv, char **envp, t_exec *exe, t_ast *node);
void	exec_builtin(char **argv, char **envp, t_exec *exe, \
			void (**builtin_fun_ptr)(char **, char **, t_exec *));
void	exec_binary(char **argv, char **envp, t_exec *exe, t_ast *node);
int		is_builtin(char *cmd, \
			void (**builtin_fun_ptr)(char **, char **, t_exec *));

void	io_manager_in(t_ast *node, t_exec *exe);
void	io_manager_pre(t_ast *node, t_exec *exe);

char	*get_env(const char *name, const char **envp);
int		get_env_pos(const char *name, const char **envp);
size_t	size_envp(const char **envp);
char	**cp_envp(const char **envp);
void	show_envp(char **envp);
char	*new_path(char *s1, char *s2);
char	**get_path(char *str);
char	*isin_path(char **paths, char *cmd);

void	builtin_exit(char **argv, char **envp, t_exec *exe);
void	builtin_echo(char **argv, char **envp, t_exec *exe);

void	builtin_env(char **argv, char **envp, t_exec *exe);
void	builtin_setenv(char **argv, char **envp, t_exec *exe);
void	builtin_unsetenv(char **argv, char **envp, t_exec *exe);
void	builtin_cd(char **argv, char **envp, t_exec *exe);
void	builtin_return(char **argv, char **envp, t_exec *exe);

char	**inline_setenv(char *name, char *value, char **envp);
char	**inline_unsetenv(char *name, char **envp);

void	handle_redirs(t_ast *redir_ast_node);
void	handle_redir_fd(int input_fd, int target_fd);

void	handle_pipes(t_ast *node);
void	init_pipe_data(char ***node_data, t_ast *pipe_node_ptr);
t_ast	*get_last_pipe_node(t_ast *node);

void	free_exec(t_exec **exe);
t_exec	*create_exec(const char **envp);

void	handle_open_error(int errno_code, char *filename);
void	print_error(char *subject, char *err_str);

#endif
