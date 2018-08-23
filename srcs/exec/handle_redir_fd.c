/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir_fd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/20 13:21:13 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/19 22:05:47 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Handle redirections to fds ex: 2>&1
*/

void		handle_redir_fd(int input_fd, int target_fd)
{
	log_trace("\tRedir fd %d -> %d", input_fd, target_fd);
	dup2(target_fd, input_fd);
}
