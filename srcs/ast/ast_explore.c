/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_explore.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 12:41:13 by sbrucker          #+#    #+#             */
/*   Updated: 2018/12/09 16:23:39 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Wait for all childs created for the pipeline,
** each child will be removed in our g_jobs->processes
** linked list by the sigchild handler,
** when this list is empty, we know no more processes are running
*/

static int		wait_childs(t_job *job)
{
	int			status;
	int			r;
	pid_t		waited_pid;

	if (!(job && job->first_process))
		return (0);
	r = 0;
	while ((waited_pid = waitpid(0, &status, 0)) != -1)
	{
		if (waited_pid == g_jobs->last_process_pid)
		{
			r = get_process_return_code(&status, waited_pid, \
				g_jobs->last_process_pid);
			log_info("Last process of pipeline terminated: Return code: %d", r);
		}
		log_info("PID %zu terminated w/ exitstatus: %d", \
			waited_pid, WEXITSTATUS(status));
	}
	while (waitpid(0, NULL, 0) != -1)
		;
	log_debug("End of wait_childs debug_jobs:");
	debug_jobs(g_jobs);
	return (r);
}

/*
** Set process group of our new job for pipeline and
** continue ast exploration in this fork,
** when all processes are started and fds piped to each others,
** wait for all childs before free'ing unnecessary data and
** exiting w/ last process of the pipeline as return value
**
** note: setpgrp: same as 'setpgid(0, 0)'
*/

static int		new_pipeline_job(t_ast *ast, t_exec *exe)
{
	int		r;

	if (setpgrp())
		exit(EXIT_FAILURE);
	g_jobs = create_job("PIPE MANAGER");
	if ((g_jobs->pgid = getpgid(getpid())) == -1)
		exit(EXIT_FAILURE);
	ast_explore(ast, exe);
	debug_jobs(g_jobs);
	log_trace("Pipe Manager: Waiting pipeline processes");
	r = wait_childs(g_jobs);
	if (VERBOSE_MODE || is_option_activated("v", g_sh_opts, NULL))
		ast_debug(ast, 1);
	free_all_shell_datas();
	log_trace("PIPE MANAGER: All processes terminated: Exiting w/ code: %d", r);
	exit(r);
}

static int		no_job_control(bool *is_in_pipeline, t_ast *ast, t_exec *exe)
{
	int	r;

	r = 0;
	g_jobs = create_job("PIPE");
	g_jobs->pgid = getpid();
	*is_in_pipeline = true;
	ast_explore(ast, exe);
	r = wait_childs(g_jobs);
	free_job(g_jobs);
	g_jobs = NULL;
	*is_in_pipeline = false;
	return (r);
}

/*
** When a new pipeline process is needed,
** verify if we're not already in a fork or
** fork to current ast node if needed, and actualize is_in_pipeline
** to tell ast_explore not to fork a 2nd time
*/

static int		handle_new_pipeline(t_ast *ast, t_exec *exe, \
					bool *is_in_pipeline)
{
	pid_t	pipeline_manager_pid;
	int		status;
	pid_t	waited_pid;
	int		r;

	if ((*is_in_pipeline = true) && ENABLE_JOB_CONTROL)
	{
		if ((pipeline_manager_pid = fork()) <= 0)
		{
			if (pipeline_manager_pid == -1)
				fatal_fork_fail();
			else if (pipeline_manager_pid == 0)
				new_pipeline_job(ast, exe);
		}
		g_jobs = create_job("PIPE MANAGER");
		g_jobs->pgid = pipeline_manager_pid;
		log_trace("MAIN PROCESS (PID %d): waiting pipe manager pid %d", \
		getpid(), pipeline_manager_pid);
		waited_pid = waitpid(pipeline_manager_pid, &status, 0);
		r = get_process_return_code(&status, waited_pid, pipeline_manager_pid);
		log_trace("MAIN PROCESS: Pipe manager terminated w/ return code: %d - \
		status: %d - wexitstatus: %d", \
		r, status, WEXITSTATUS(status));
		free_job(g_jobs);
		g_jobs = NULL;
		*is_in_pipeline = false;
		return (r);
	}
	else
		return (no_job_control(is_in_pipeline, ast, exe));
}

/*
** Function used to recursively exec ast nodes,
** when a new pipeline is found, stop the recursion for the main process and
** fork it to execute the pipeline in its own new process,
** this is done to be able to group kill all of the
** forked processes in the pipeline
** without exiting the shell and easier job control
*/

t_exec			*ast_explore(t_ast *ast, t_exec *exe)
{
	static bool	is_in_pipeline = false;
	int			tmp;

	if (!ast)
		return (exe);
	if (ast->data)
		log_debug("Current node: %p (%s) - rdy for exec: %d - is in pipeline ?"\
		": %s", ast, ast->data[0], exe->ready_for_exec, \
		(is_in_pipeline) ? ("true") : ("false"));
	if (!is_in_pipeline && ast->type_details == TK_PIPE)
	{
		tmp = handle_new_pipeline(ast, exe, &is_in_pipeline);
		if (!exe->ready_for_exec)
			exe->ret = tmp;
		return (exe);
	}
	ast_explore(ast->left, exe);
	if (exe->statement)
		return (exe);
	handle_node(ast, exe);
	if (exe->statement)
		return (exe);
	ast_explore(ast->right, exe);
	return (exe);
}
