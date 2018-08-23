/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/14 17:20:41 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/14 17:24:40 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

void	print_error(char *subject, char *err_str)
{
	ft_putstr_fd("21sh: ", 2);
	ft_putstr_fd(err_str, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(subject, 2);
	ft_putstr_fd("\n", 2);
}