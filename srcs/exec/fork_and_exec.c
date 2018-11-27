/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 15:32:12 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/18 15:32:22 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

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
		waited_pid = waitpid(child_pid, &status, 0);
		return_code = get_process_return_code(&status, waited_pid, child_pid);
		log_info("PID %zu: Command %s exited w/ return_code: %d", getpid(), \
			((intptr_t)*cmd != PROG_BUILTIN) ? (cmd[1]) : ("-builtin-"), return_code);
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
	if ((intptr_t)*cmd == PROG_NOT_BUILTIN || \
		ptr == &builtin_test)
		return (true);
	return (false);
}

t_exec		*fork_and_exec(void **cmd, t_exec *exe, \
				t_ast *node)
{
	pid_t	child_pid;
	t_ast	*last_pipe_node;
	int		**pipe_fds;

	if ((last_pipe_node = get_last_pipe_node(node)) && \
		(!last_pipe_node->data[1] || \
			(last_pipe_node->data[1][0] == -1 && last_pipe_node->data[1][sizeof(int)] == -1)))
		init_pipe_data(&(last_pipe_node->data), last_pipe_node);
	if (last_pipe_node || should_fork(cmd))
	{
		pipe_fds = get_pipe_fds(last_pipe_node, node);
		exe->prog_forked = true;
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
	}
	else
	{
		exe->prog_forked = false;
		child_process(cmd, exe, node, NULL);
	}
	return (exe);
}
