/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare_fds_with_strings.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/29 17:48:52 by jjaniec           #+#    #+#             */
/*   Updated: 2018/10/01 20:11:03 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

void		compare_fds_with_strings(char *test_name, \
				char *expected_stdout, char *expected_stderr, \
				int backup_stdout_fd, int backup_stderr_fd)
{
	char	buf[CMP_BUFF_SIZE + 1];
	int		bytes_read = 0;
	char	*tmp_test_name;
	int		fd;

	if (backup_stdout_fd)
	{
		dup2(backup_stdout_fd, fileno(stdout));
		close(backup_stdout_fd);
		if (expected_stdout)
		{
		    fd = open(redirect_both_fds_STDOUT_FILENAME, O_RDWR|O_CREAT|O_APPEND, 0600);
   				if (-1 == fd) { exit(255); }
			if ((bytes_read = read(fd, buf, CMP_BUFF_SIZE)) == -1)
				printf("Can't read comparison file desc %d!\n", fd);
			else
				buf[bytes_read] = '\0';
			tmp_test_name = ft_strjoin(test_name, " - stdout");
			is(buf, expected_stdout, tmp_test_name);
			free(tmp_test_name);
			close(fd);
		}
	}

	if (backup_stderr_fd)
	{
		dup2(backup_stderr_fd, fileno(stderr));
		close(backup_stderr_fd);
		if (expected_stderr)
		{
		    fd = open(redirect_both_fds_STDERR_FILENAME, O_RDWR|O_CREAT|O_APPEND, 0600);
   				if (-1 == fd) { exit(255); }
			if ((bytes_read = read(fd, buf, CMP_BUFF_SIZE)) == -1)
				printf("Can't read comparison file desc %d!\n", fd);
			else
				buf[bytes_read] = '\0';
			tmp_test_name = ft_strjoin(test_name, " - stderr");
			is(buf, expected_stderr, tmp_test_name);
			free(tmp_test_name);
			close(fd);
		}
	}
}
