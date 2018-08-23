/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_open_error.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 17:02:29 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/14 17:41:16 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

void	handle_open_error(int errno_code, char *filename)
{
	if (errno_code == EACCES)
		print_error(filename, "permission denied");
	else if (errno_code == EISDIR)
		print_error(filename, "is a directory");
	else if (errno_code == ENOENT)
		print_error(filename, "no such file or directory");
	else if (errno_code == EROFS)
		print_error(filename, "is on a read-only file system");
	else if (errno_code == ETXTBSY)
		print_error(filename, "can't write to running executable");
	else
		print_error(filename, "can't open file");
	errno = 0;
}
