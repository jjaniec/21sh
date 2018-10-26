/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_explore.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/23 12:41:13 by sbrucker          #+#    #+#             */
/*   Updated: 2018/10/26 20:44:04 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

static int		wait_childs(t_job *job)
{
	int			status;
	int			r = 0;
	pid_t		waited_pid;
	t_process	*ptr;

	ptr = g_jobs->first_process;
	debug_jobs(g_jobs);
	//refresh_job_running_processes(g_jobs);
	/*
	while (ptr)
	{
		waited_pid = waitpid(ptr->pid, &status, 0);
		ptr = ptr->next;
	}*/

	while ((waited_pid = waitpid(0, &status, 0)) != -1)
	{
		if (waited_pid == g_jobs->last_process_pid)
		{
			r = get_process_return_code(&status, waited_pid, g_jobs->last_process_pid);
			log_info("Last process of pipeline terminated: Return code set to %d", r);
		}
		log_info("PID %zu terminated w/ exitstatus: %d - last_process_pid: %d", \
			waited_pid, WEXITSTATUS(status), g_jobs->last_process_pid);
		remove_task_pid_from_job(g_jobs, waited_pid);
		debug_jobs(g_jobs);
	}
	//refresh_job_running_processes(g_jobs);
	return r;
}

static int		new_pipeline_job(t_ast *ast, t_exec *exe)
{
	int		r;

	setpgrp(); // -> alias to setpgid(0, 0);
	//if (setpgid(0, 0))
	//	perror("Setpgid");
	g_jobs = create_job("PIPE MANAGER");
	if ((g_jobs->pgid = getpgid(getpid())) == -1)
		perror("Getpgid in pipeline manager");
	ast_explore(ast, exe);
	debug_jobs(g_jobs);
	log_trace("Pipe Manager: Waiting pipeline processes");
	r = wait_childs(g_jobs);
	if (VERBOSE_MODE || is_option_activated("v", g_sh_opts, NULL))
		ast_debug(ast);
	free_all_shell_data();
	log_trace("PIPE MANAGER: All processes terminated: Exiting w/ code: %d", r);
	exit(r);
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
	int		status = 0;
	pid_t	waited_pid = 0;
	int		r = 0;

/*
	*is_in_pipeline = true;
	if ((pipeline_manager_pid = fork()) <= 0)
	{
		if (pipeline_manager_pid == -1)
			ft_putstr_fd(SH_NAME": Failed to fork pipeline", 2);
		else if (pipeline_manager_pid == 0)
			new_pipeline_job(ast, exe);
	}
	g_jobs = create_job("PIPE MANAGER");
	g_jobs->pgid = pipeline_manager_pid;
	log_trace("MAIN PROCESS (PID %d): waiting pipe manager pid %d", getpid(), pipeline_manager_pid);
	errno = 0;
	waited_pid = waitpid(pipeline_manager_pid, &status, 0);
	r = get_process_return_code(&status, waited_pid, pipeline_manager_pid);
	log_trace("MAIN PROCESS: Pipe manager terminated w/ return code: %d - status: %d - wexitstatus: %d", \
		r, status, WEXITSTATUS(status));
	free_job(g_jobs);
	g_jobs = NULL;
	*is_in_pipeline = false;
	return (r);
*/

	g_jobs = create_job("PIPE MANAGER");
	*is_in_pipeline = true;
	ast_explore(ast, exe);
	r = wait_childs(g_jobs);
	free_job(g_jobs);
	g_jobs = NULL;
	*is_in_pipeline = false;
	return (r);
}

/*
** Function used to recursively exec ast nodes,
** when a new pipeline is found, stop the recursion for the main process and
** fork it to execute the pipeline in its own new process,
** this is done to be able to group kill all of the
** forked processes in the pipeline
** without exiting the shell
*/

t_exec	*ast_explore(t_ast *ast, t_exec *exe)
{
	static bool	is_in_pipeline_fork = false;
	int			tmp;

	if (!ast)
		return (exe);
	if (ast->data)
		log_debug("Current node: %p (%s) - rdy for exec: %d - is in pipeline fork ?: %s", \
			ast, ast->data[0], exe->ready_for_exec, (is_in_pipeline_fork) ? ("true") : ("false"));
	if (!is_in_pipeline_fork && ast->type_details == TK_PIPE)
	{
		tmp = handle_new_pipeline(ast, exe, &is_in_pipeline_fork);
		if (!exe->ready_for_exec)
			exe->ret = tmp;
		/*is_in_pipeline_fork = false;
		if (!is_in_pipeline_fork)*/
		return (exe);
	}
	pre_exec(ast, exe);
	ast_explore(ast->left, exe);
	in_exec(ast, exe);
	ast_explore(ast->right, exe);
	post_exec(ast, exe);
	return (exe);
}
