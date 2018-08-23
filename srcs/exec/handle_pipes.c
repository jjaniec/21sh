/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/20 13:04:45 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/19 15:54:47 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Search for last pipe and apply pipe redirections depending on
** which side is our element compared to the pipe node (right/left)
*/

static void	apply_pipes_fds(t_ast *node, t_ast *last_pipe)
{
	t_ast	*ptr;

	ptr = node;
	while (ptr->parent != last_pipe)
		ptr = ptr->parent;
	if (ptr == last_pipe->right)
	{
		handle_redir_fd(STDIN_FILENO, *(&(last_pipe->data[1][0])));
		if (last_pipe->parent && last_pipe->parent->type_details == TK_PIPE)
			handle_redir_fd(STDOUT_FILENO, *(&(last_pipe->parent->data[1][sizeof(int)])));
	}
	else if (ptr == last_pipe->left)
		handle_redir_fd(STDOUT_FILENO, *(&(last_pipe->data[1][sizeof(int)])));
}

/*
** Search for specified pipes in ast and handle std(in/out) redirections
** with pipe fds before redirections like 1>file as pipes are
** prioritized
*/

void	handle_pipes(t_ast *node)
{
	t_ast	*last_pipe_node;

	log_trace("Handle pipes of %s", node->data[0]);
	if (!(last_pipe_node = get_last_pipe_node(node)))
		return ;
	apply_pipes_fds(node, last_pipe_node);
}
