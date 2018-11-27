/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_open_error.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 17:02:29 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/18 15:13:52 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

int		handle_open_error(int errno_code, char *filename)
{
	int		r;

	r = 0;
	if (errno_code)
		r = 1;
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
	return (r);
}
