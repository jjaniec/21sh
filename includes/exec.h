/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 13:04:09 by sbrucker          #+#    #+#             */
/*   Updated: 2018/11/19 17:40:14 by jjaniec          ###   ########.fr       */
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
** Char signaling filedesc redir close
*/

# define CLOSE_FD_REDIR_SYMBOL '-'

/*
** Validity checks for redirections,
** those defines are used only in check_redir_suffix_validity.c
*/

# define MODE_CHECK_REDIR_VALID_FILE	1
# define MODE_CHECK_REDIR_VALID_FD		2
# define REDIRS_AMBIGUOUS_CHARS	""

/*
** Default count of filedesc supported for redirects like 4>&-
*/

# define DEFAULT_SUPPORTED_FDS_COUNT 3

/*
** Define to indicate to fork_and_exec() that passed command is a builtin
** and should be executed with a function pointer instead of execve()
*/

# define PROG_NOT_BUILTIN	0
# define PROG_BUILTIN		1
# define STATEMENT_BREAK			-1
# define STATEMENT_CONTINUE			-2
# define STATEMENT_NOCMD			-3

/*
** Specify attributes passed to open for each output redirection token
*/

# define REDIRS_OPEN_BASE_ATTR	O_WRONLY | O_CREAT

# define TK_GREAT_OPEN_ATTR		REDIRS_OPEN_BASE_ATTR | O_TRUNC
# define TK_DGREAT_OPEN_ATTR 	REDIRS_OPEN_BASE_ATTR | O_APPEND
# define TK_LESSGREAT_OPEN_ATTR	REDIRS_OPEN_BASE_ATTR | O_RDONLY

/*
** Modes used by beckup_apply_origin_fds() in child_process.c
*/

# define MODE_BACKUP_ORIGIN_FDS		1
# define MODE_RESTORE_ORIGIN_FDS	2

void		clean_data(char **data);

// Binary finding & execution

t_exec	*exec_cmd(t_ast *root, t_exec *exe);
t_exec	*fork_and_exec(void **cmd, t_exec *exe, t_ast *node);
void	child_process(void **cmd, t_exec *exe, \
				t_ast *node, int **pipe_fds);
t_exec	*pre_exec(t_ast *node, t_exec *exe);
t_exec	*in_exec(t_ast *node, t_exec *exe);
t_exec	*post_exec(t_ast *node, t_exec *exe);
int		resolve_cmd_path(void **cmd, t_exec *exe);
int		get_process_return_code(int *status, pid_t waited_pid, pid_t expected_waited_pid);

void	io_manager_in(t_ast *node, t_exec *exe);
void	io_manager_pre(t_ast *node, t_exec *exe);

void	free_exec(t_exec **exe);
t_exec	*create_exec(t_environ *env);

// Path parsing

char	*isin_path(char *path_entry, char *cmd);


char	*get_env(const char *name, const char **envp);
int		get_env_pos(const char *name, const char **envp);

int		parse_expr_file(char **argv, t_option *opt_list, \
		t_option *char_opt_index[CHAR_OPT_INDEX_SIZE]);
int		parse_expr_comp(char **argv);

// Redirects

int		handle_redirs(t_ast *redir_ast_node);
void	handle_redir_fd(int input_fd, int target_fd);
int		check_redir_suffix_validity(t_ast *redir_ast_node);

// Pipes

int		handle_pipes(int **pipe_fds);
void	init_pipe_data(char ***node_data, t_ast *pipe_node_ptr);
t_ast	*get_last_pipe_node(t_ast *node);
int		**get_pipe_fds(t_ast *last_pipe_node, t_ast *node);

// Error Handling

int				handle_open_error(int errno_code, char *filename);
void			print_error(char *subject, char *err_str);

t_job		*create_job(char *command);

void			debug_jobs(t_job *jobs);

t_process		*add_running_process(char **cmd, pid_t process_pid, t_job **job);
pid_t		remove_task_pid_from_job(t_job *job, pid_t process_pid);


void		refresh_job_running_processes(t_job *job);

void			free_job(t_job *job);

void			clear_running_process_list(t_process *process_list);
#endif
