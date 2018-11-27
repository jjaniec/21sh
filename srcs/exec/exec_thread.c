/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_thread.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/25 11:16:01 by sbrucker          #+#    #+#             */
/*   Updated: 2018/11/27 12:08:49 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Handle pipes and redirections then
** launch specified program in child process
**
** $cmd wizard casts explanations:
** if cmd[0] is equal to EXEC_THREAD_NOT_BUILTIN,
** we know the the passed command
** is not a builtin and should be executed by execve,
** with path stored in cmd[1] and args in cmd[2],
**
** otherwise if cmd[0] is equal to EXEC_THREAD_BUILTIN,
** we know that the command we
** have to execute is a builtin, and so should not be executed with execve() but
** with a function pointer to the builtin instead,
** the function pointer will then be stored in cmd[1] (w/ args in cmd[2]),
** and be casted to be executed by this function
** (which allows pipes and redirs handling for builtins
** without duplicating code)
*/

static void	backup_origin_fds(int *backup_fds)
{
	int			i;

	i = 0;
	while (i < DEFAULT_SUPPORTED_FDS_COUNT)
		(backup_fds)[i++] = -1;
	i = 0;
	while (i < DEFAULT_SUPPORTED_FDS_COUNT)
	{
		if (((backup_fds)[i] = dup(i)))
			log_error("PID %zu: Fd %d duplication failed!", getpid(), i);
		i += 1;
	}
}

static void	restore_origin_fds(int *backup_fds)
{
	int			i;

	i = 0;
	while (i < DEFAULT_SUPPORTED_FDS_COUNT)
	{
		handle_redir_fd(i, backup_fds[i]);
		i++;
	}
}

static void	remove_tmp_env_assigns(t_environ *environ_used, char **env_assign_vars)
{
	t_environ	*env;

	if (env_assign_vars)
	{
		env = get_shell_vars()->env;
		if (environ_used == env)
		{
			while (*env_assign_vars)
				env->del_var(env, *env_assign_vars);
		}
		else
		{
			free_env_entries(environ_used, environ_used->env_entries_list);
			free(environ_used);
		}
	}
}

static void	child_process(void **cmd, t_exec *exe, \
				t_ast *node, int **pipe_fds)
{
	int		backup_fds[DEFAULT_SUPPORTED_FDS_COUNT];
	bool	can_run_cmd;
	int		r;
	char	**cmd_args;
	char	**env_;
	char	*tmp;
	t_ast	**ast_ptr;
	char	**env_assign_vars;
	t_environ	*env_assigns_environ;

	if (!(exe->prog_forked) && (node->parent && node->parent->type == T_REDIR_OPT))
		backup_origin_fds(backup_fds);
	if (node && node->left && node->left->type == T_ENV_ASSIGN)
		env_assign_vars = handle_env_assigns(node, exe, &env_assigns_environ);
	else
	{
		env_assigns_environ = exe->env;
	}
	handle_pipes(pipe_fds);
	handle_redirs(node);
	can_run_cmd = true;
	if ((intptr_t)*cmd == EXEC_THREAD_NOT_BUILTIN)
		can_run_cmd = !(resolve_cmd_path(&(cmd[1]), exe));
	if (can_run_cmd)
	{
		log_debug("PID %zu: Exec child process cmd: %p - cmd[0] : %d", getpid(), cmd, (intptr_t)cmd[0]);
		if ((intptr_t)*cmd == EXEC_THREAD_BUILTIN)
			(*(void (**)(char **, t_environ *, t_exec *))(cmd[1]))\
				(cmd[2],/* exe->env */env_assigns_environ, exe);
		else
		{
			cmd_args = ft_dup_2d_array(cmd[2]);
			tmp = ft_xstrdup(cmd[1]);
			log_debug("PID %zu -> child process cmd[1]: %s", getpid(), cmd[1]);
			//cmd[1] = ft_strdup(cmd[1]);
			t_shell_vars	*vars = get_shell_vars();
			free_hashtable(vars->hashtable);
			if ((ast_ptr = access_ast_data()))
				ast_free(*ast_ptr);
			env_ = exe->env->environ;
			free(exe);
			errno = 0;
			if (execve(tmp, cmd_args, env_))
			{
				log_error("PID %zu - Execve() not working", getpid());
				perror("PERROR execve");
			}
			exit(EXIT_FAILURE);
		}
	}
	if (node && node->left && node->left->type == T_ENV_ASSIGN)
		remove_tmp_env_assigns(env_assigns_environ, env_assign_vars);
	if (!(exe->prog_forked) && (node->parent && node->parent->type == T_REDIR_OPT))
		restore_origin_fds(backup_fds);
	if (!can_run_cmd || (intptr_t)*cmd != EXEC_THREAD_BUILTIN || pipe_fds)
	{
		log_trace("PID %zu: Forcing exit of child process", getpid());
		r = exe->ret;
		free_all_shell_datas();
		free(exe);
		free_job(g_jobs);
		exit(r);
	}
	//exit(EXIT_FAILURE);
}

/*
** Close input/output fds used by current process, and replace pipe data
** corresponding to closed fd to -1, to be sure it's not double closed
** free pipe data when it's two fds are set to -1
*/

static void		close_child_pipe_fds(int **pipe_fds)
{
	if (pipe_fds)
	{
		if (pipe_fds[0] && *(pipe_fds[0]) != -1)
		{
			log_close(*(pipe_fds[0]));
			*(pipe_fds[0]) = -1;
		}
		if (pipe_fds[1] && *(pipe_fds[1]) != -1)
		{
			log_close(*(pipe_fds[1]));
			*(pipe_fds[1]) = -1;
		}
	}
}

//static void	handle_foregrounded_child

/*
** Parent process function when forking
** Wait child process to end if not in a pipeline,
** if process was killed by a signal and waitpit stopped by it,
** waitpid on child to prevent zombie process
*/

static int	parent_process(char **cmd, pid_t child_pid,	int **pipe_fds)
{
	pid_t		waited_pid;
	int			status;
	int			return_code;
	int			shell_pgid;

	status = -2;
	return_code = status;
	add_running_process((char **)cmd[2], child_pid, &g_jobs);
	debug_jobs(g_jobs);
	//process_ptr->input_descriptor = -1; // ->
	close_child_pipe_fds(pipe_fds);
	if (!pipe_fds)
	{
		if (g_jobs)
			g_jobs->pgid = 0;
		if (setpgid(child_pid, child_pid) != 0)
			exit(EXIT_FAILURE);
		shell_pgid = getpgrp();
		if (tcsetpgrp(STDIN_FILENO, child_pid) != 0) 
			exit(EXIT_FAILURE);
		waited_pid = waitpid(child_pid, &status, 0);
		if (tcsetpgrp(STDIN_FILENO, shell_pgid) != 0)
			exit(EXIT_FAILURE);
		return_code = get_process_return_code(&status, waited_pid, child_pid);
		log_info("PID %zu: Command %s exited w/ return_code: %d", getpid(), \
			((intptr_t)*cmd != EXEC_THREAD_BUILTIN) ? (cmd[1]) : ("-builtin-"), return_code);
		free_job(g_jobs);
		g_jobs = NULL;
	}
	else
		free(pipe_fds);
	return (return_code);
}

/*
** Fork here if passed command
** is not the exit() builtin function (verified by is_cmd_exit())
** pointer stored in cmd[1], then
** call the parent_process() function to wait the child process,
** and the child_process() function to handle pipes, redirs, and process/builtin
** execution.
** Forked builtins should be added to this list
** More explanation of $cmd in commentary of the child_process() function
*/

static bool	should_fork(void **cmd)
{
	void	(*ptr)(char **, t_environ *, t_exec *);

	ptr = *((void (**)(char **, t_environ *, t_exec *))(cmd[1]));
	if ((intptr_t)*cmd == EXEC_THREAD_NOT_BUILTIN || \
		ptr == &builtin_test)
		return (true);
	return (false);
}

t_exec		*exec_thread(void **cmd, t_environ *env_struct, t_exec *exe, \
				t_ast *node)
{
	pid_t	child_pid;
	t_ast	*last_pipe_node;
	int		**pipe_fds;
	(void)env_struct;
	if ((last_pipe_node = get_last_pipe_node(node)) && \
		(!last_pipe_node->data[1] || \
			(last_pipe_node->data[1][0] == -1 && last_pipe_node->data[1][sizeof(int)] == -1)))
		init_pipe_data(&(last_pipe_node->data), last_pipe_node);
	if (last_pipe_node || should_fork(cmd))
	{
		pipe_fds = get_pipe_fds(last_pipe_node, node);
		exe->prog_forked = true;
/*
		struct sigaction s;
		s.sa_flags = 0;
		sigfillset(&(s.sa_mask));
		s.sa_handler = SIG_IGN;
		sigaction(SIGINT, &s, NULL);
*/

		child_pid = fork();
		if (child_pid == -1)
			log_error("Fork() not working");
		else if (child_pid == 0)
			child_process(cmd, exe, node, pipe_fds);
		else
		{
			g_cmd_status.cmd_running = true;
			g_cmd_status.cmd_pid = child_pid;
			log_trace("Forked process pid: %d for cmd: %s", child_pid, node->data[0]);
			exe->ret = parent_process((char **)cmd, child_pid, pipe_fds);
		}
/*
		s.sa_handler = handle_sigint;
		sigaction(SIGINT, &s, NULL); */
	}
	else
	{
		exe->prog_forked = false;
		child_process(cmd, exe, node, NULL);
	}
	return (exe);
}
