/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 13:04:09 by sbrucker          #+#    #+#             */
/*   Updated: 2018/11/11 16:45:46 by sbrucker         ###   ########.fr       */
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
# define STATEMENT_BREAK			-1
# define STATEMENT_CONTINUE			-2
# define STATEMENT_NOCMD			-3

void		clean_data(char **data);

// Binary finding & execution

t_exec	*exec_cmd(t_ast *root, t_exec *exe);
t_exec	*exec_thread(void **cmd, t_environ *env_struct, t_exec *exe, t_ast *node);
t_exec	*pre_exec(t_ast *node, t_exec *exe);
t_exec	*in_exec(t_ast *node, t_exec *exe);
t_exec	*post_exec(t_ast *node, t_exec *exe);
int		resolve_cmd_path(void **cmd, t_exec *exe);
int		is_builtin(char *cmd, \
			void (**builtin_fun_ptr)(char **, t_environ *, t_exec *));
int		get_process_return_code(int *status, pid_t waited_pid, pid_t expected_waited_pid);

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
void	builtin_alias(char **argv, t_environ *env, t_exec *exe);
void	builtin_unalias(char **argv, t_environ *env, t_exec *exe);

char	*get_env(const char *name, const char **envp);
int		get_env_pos(const char *name, const char **envp);

int		parse_expr_file(char **argv, t_option *opt_list, \
		t_option *char_opt_index[CHAR_OPT_INDEX_SIZE]);
int		parse_expr_comp(char **argv);

// Redirects

void	handle_redirs(t_ast *redir_ast_node);
void	handle_redir_fd(int input_fd, int target_fd);

// Pipes

int		handle_pipes(int **pipe_fds);
void	init_pipe_data(char ***node_data, t_ast *pipe_node_ptr);
t_ast	*get_last_pipe_node(t_ast *node);
int		**get_pipe_fds(t_ast *last_pipe_node, t_ast *node);


// Error Handling

void			handle_open_error(int errno_code, char *filename);
void			print_error(char *subject, char *err_str);

t_job		*create_job(char *command);

void			debug_jobs(t_job *jobs);

t_process		*add_running_process(char **cmd, pid_t process_pid, t_job **job);
pid_t		remove_task_pid_from_job(t_job *job, pid_t process_pid);


void		refresh_job_running_processes(t_job *job);

void			free_job(t_job *job);

void			clear_running_process_list(t_process *process_list);
#endif
