/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   twenty_one_sh.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/11 16:15:27 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/18 14:55:03 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TWENTY_ONE_SH_H
# define TWENTY_ONE_SH_H

# include <ft_printf.h>
# include "struct.h"
# include "lexer.h"
# include "log.h"
# include "ast.h"
# include "line_edition.h"
# include "exec.h"
# include <sys/wait.h>
# include <errno.h>

# define IFS			"\t\n "
# define MALLOC_ERROR 	1

# ifndef VERBOSE_MODE
#  define VERBOSE_MODE 0
# endif

void	ft_free_argv(char **tab_);

#endif
