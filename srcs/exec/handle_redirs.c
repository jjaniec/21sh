/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/04 18:30:50 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/19 22:16:13 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Handle input redirections
** For here-doc redirections, init a temporary pipe w/
** file descs stored in redir node data,
** then write content of here document in input, close it and redir
** stdin to pipe output
*/

static void		handle_input_redir(int prefix_fd, char *target, \
					size_t tk_type_details, t_ast *node)
{
	int		fd;

	if (prefix_fd == -1)
		prefix_fd = DEFAULT_INPUT_REDIR_FD;
	if (tk_type_details == TK_LESS)
	{
		if ((fd = open(target, O_RDONLY, 0)) == -1)
			handle_open_error(errno, target);
		else
			handle_redir_fd(prefix_fd, fd);
	}
	else if (tk_type_details == TK_DLESS || \
			tk_type_details == TK_TLESS)
	{
		init_pipe_data(&(node->data), NULL);
		ft_putstr_fd(node->right->data[0], \
			*(&(node->data[1][sizeof(int)])));
		ft_putchar_fd('\n', *(&(node->data[1][sizeof(int)])));
		close(*(&(node->data[1][sizeof(int)])));
		log_debug("Here-doc: writing %s to pipe input fd: %d", \
			node->right->data[0], *(&(node->data[1][sizeof(int)])));
		handle_redir_fd(STDIN_FILENO, *(&(node->data[1][0])));
	}
}

/*
** Handle output redirections
*/

static void		handle_output_redir(int prefix_fd, \
					char *target, size_t tk_type_details)
{
	int		fd;

	fd = -1;
	if (prefix_fd == -1)
		prefix_fd = DEFAULT_OUTPUT_REDIR_FD;
	if (tk_type_details == TK_GREAT)
		fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, \
			DEFAULT_OUTPUT_REDIR_FILE_MODE);
	else if (tk_type_details == TK_DGREAT)
		fd = open(target, O_WRONLY | O_CREAT | O_APPEND, \
			DEFAULT_OUTPUT_REDIR_FILE_MODE);
	if (fd == -1)
		handle_open_error(errno, target);
	else
		handle_redir_fd(prefix_fd, fd);
}

/*
** If redirction operator begins w/ '>', call
** handle_output_redir,
** w/ a '<', call handle_input_redir,
** or simply do a dup2 on specified file desc
** if operator contains a '&' like ">&"
*/

static void		handle_redir(int prefix_fd, t_ast *node)
{
	char	*target;

	target = node->right->data[0];
	if (node->type_details == TK_LESSAND \
		|| node->type_details == TK_GREATAND)
		handle_redir_fd(prefix_fd, ft_atoi(target));
	else
	{
		if (ft_strchr(node->data[0], '<'))
			handle_input_redir(prefix_fd, target, node->type_details, node);
		if (ft_strchr(node->data[0], '>'))
			handle_output_redir(prefix_fd, target, node->type_details);
	}
}

static void		get_prefix_fd(int *prefix_fd, char *data)
{
	*prefix_fd = ft_atoi(data);
	if (*prefix_fd == 0 && !(data[0] == '0'))
		*prefix_fd = -1;
}

/*
** Cycle through T_REDIR_OPT nodes,
** find prefix file descriptor stored in $data[0]
** and call handle_redir for each
*/

void			handle_redirs(t_ast *redir_ast_node)
{
	t_ast	*node;
	int		prefix_fd;

	node = redir_ast_node->parent;
	log_trace("Handle redirs of %s(t %zu td %zu)", redir_ast_node->data[0], \
			redir_ast_node->type, redir_ast_node->type_details);
	while (node && node->parent && node->parent->type == T_REDIR_OPT)
		node = node->parent;
	while (node && node->type == T_REDIR_OPT)
	{
		get_prefix_fd(&prefix_fd, node->data[0]);
		handle_redir(prefix_fd, node);
		node = node->left;
	}
}
