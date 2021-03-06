/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare_sh_42sh_outputs.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/29 17:36:33 by jjaniec           #+#    #+#             */
/*   Updated: 2018/12/04 19:25:13 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tests.h"

/*
** Compares output of 42sh and sh by redirecting file descs in files and reading it,
** as sh does not support '<<<' redirs on linux,
** compare w/ output of bash instead, which handles '<<<' tokens
*/

void	compare_sh_42sh_outputs(char *test_name, char *_42sh_cmd, char *sh_cmd)
{
	int		test_name_str_len;
	char	*new_test_name;
	int		tmp;
	int		ret;
	int		backup_stdout_fd;
	int		backup_stderr_fd;
	char	*cmd_sh;

	if (!(*MODE == 'L' && ft_strstr(_42sh_cmd, "<<<")))
	{
		redirect_both_fds(&backup_stdout_fd, &backup_stderr_fd, _42SH_DATA_FILE_STDOUT, _42SH_DATA_FILE_STDERR);
		asprintf(&cmd_sh, SH_EXEC_CMD_PREFIX"\"%s\"", _42sh_cmd);
		system(cmd_sh);
		free(cmd_sh);
		dup2(backup_stdout_fd, fileno(stdout));
		close(backup_stdout_fd);
		dup2(backup_stderr_fd, fileno(stderr));
		close(backup_stderr_fd);

		redirect_both_fds(&backup_stdout_fd, &backup_stderr_fd, SH_DATA_FILE_STDOUT, SH_DATA_FILE_STDERR);
		asprintf(&cmd_sh, "sh -c \"%s\"", ((sh_cmd) ? (sh_cmd) : (_42sh_cmd)));
		system(cmd_sh);
		free(cmd_sh);
		dup2(backup_stdout_fd, fileno(stdout));
		close(backup_stdout_fd);
		dup2(backup_stderr_fd, fileno(stderr));
		close(backup_stderr_fd);

		test_name_str_len = ft_strlen(test_name);
		new_test_name = malloc(test_name_str_len * sizeof(char) + 10);
		ft_strcpy(new_test_name, test_name);
		ret = system("diff "_42SH_DATA_FILE_STDOUT" "SH_DATA_FILE_STDOUT);
		ft_strcpy(new_test_name + test_name_str_len, " - stdout");
		ok(!ret, test_name);
		ret = system("diff "_42SH_DATA_FILE_STDERR" "SH_DATA_FILE_STDERR);
		ft_strcpy(new_test_name + test_name_str_len, " - stderr");
		ok(!ret, test_name);
		free(new_test_name);

	//system("echo '42sh stdout:'; cat "_42SH_DATA_FILE_STDOUT"; echo '42sh stderr:'; cat "_42SH_DATA_FILE_STDERR"; echo 'sh stdout:';  cat "SH_DATA_FILE_STDOUT"; echo 'sh stderr:'; cat "SH_DATA_FILE_STDERR);

		remove(_42SH_DATA_FILE_STDOUT);
		remove(_42SH_DATA_FILE_STDERR);
		remove(SH_DATA_FILE_STDOUT);
		remove(SH_DATA_FILE_STDERR);
	}
}
