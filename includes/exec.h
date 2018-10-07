/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 13:04:09 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/02 12:06:24 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <forty_two_sh.h>

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

/*
** Define to indicate to exec_thread() that passed command is a builtin
** and should be executed with a function pointer instead of execve()
*/

# define EXEC_THREAD_NOT_BUILTIN	0
# define EXEC_THREAD_BUILTIN		1

// Binary finding & execution

t_exec	*exec_cmd(t_ast *root, t_exec *exe);
void	exec_argv(char **argv, t_exec *exe, t_ast *node);
t_exec	*exec_thread(void **cmd, t_environ *env_struct, t_exec *exe, t_ast *node);
t_exec	*pre_exec(t_ast *node, t_exec *exe);
t_exec	*in_exec(t_ast *node, t_exec *exe);
t_exec	*post_exec(t_ast *node, t_exec *exe);
void	exec_local(char **argv, t_environ *env_struct, t_exec *exe, t_ast *node);
int		exec_builtin(char **argv, t_environ *env_struct, t_exec *exe, t_ast *node);
void	exec_binary(char **argv, t_environ *env_struct, t_exec *exe, t_ast *node);
int		is_builtin(char *cmd, \
			void (**builtin_fun_ptr)(char **, t_environ *, t_exec *));

//

void	io_manager_in(t_ast *node, t_exec *exe);
void	io_manager_pre(t_ast *node, t_exec *exe);

void	free_exec(t_exec **exe);
t_exec	*create_exec(t_environ *env);

// Path parsing

char	*isin_path(char *path_entry, char *cmd);

// Builtins

void	builtin_exit(char **argv, t_environ *env, t_exec *exe);
void	builtin_echo(char **argv, t_environ *env, t_exec *exe);
void	builtin_env(char **argv, t_environ *env, t_exec *exe);
void	builtin_setenv(char **argv, t_environ *env, t_exec *exe);
void	builtin_unsetenv(char **argv, t_environ *env, t_exec *exe);
void	builtin_cd(char **argv, t_environ *env, t_exec *exe);
void	builtin_return(char **argv, t_environ *env, t_exec *exe);
void	builtin_toggle_syntax_highlighting(char **argv, t_environ *env, \
			t_exec *exe);
void	builtin_test(char **argv, t_environ *env, t_exec *exe);
void	builtin_history(char **argv, t_environ *env, t_exec *exe);
void	builtin_toggle_syntax_highlighting(char **argv, t_environ *env,
			t_exec *exe);

// Redirects

void	handle_redirs(t_ast *redir_ast_node);
void	handle_redir_fd(int input_fd, int target_fd);

// Pipes

void	handle_pipes(t_ast *node);
void	init_pipe_data(char ***node_data, t_ast *pipe_node_ptr);
t_ast	*get_last_pipe_node(t_ast *node);

// Error Handling

void	handle_open_error(int errno_code, char *filename);
void	print_error(char *subject, char *err_str);

#endif
