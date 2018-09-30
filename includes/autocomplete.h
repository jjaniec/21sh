/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgaspart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/28 19:11:19 by cgaspart          #+#    #+#             */
/*   Updated: 2018/08/28 19:11:20 by cgaspart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOCOMPLETE_H
# define AUTOCOMPLETE_H
# include <twenty_one_sh.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <sys/syslimits.h>

typedef struct			s_autoc
{
	int					(*key_function[128])(char buffer[3], struct s_autoc *aut);
	struct s_line		*le;
	struct winsize		win;
	char					**items;
	char					*search;
	int					nbr_items;
	int					nbr_items_in_line;
	int					nbr_line;
	int					max_item_len;
	int					menu_selected;
	int					menu_line;
}							t_autoc;

void	autocomplete(struct s_line *le);
char	**autoc_dir(t_autoc *autoc);
int	autoc_menu(t_autoc *autoc, struct s_line *le);
void	autoc_mem_free(t_autoc *autoc);
void	autoc_free_items(char **items);
int	autoc_key_reader(t_autoc *autoc);
void	autoc_menu_print_items(t_autoc *autoc, struct s_line *le);
void	autoc_menu_print_spaces(int longest, int len, struct s_line *le);
void	autoc_push_in_line(struct s_line *le, char *str, char *search);
char	autoc_check_path(char *dirname);
char	**dir_get_items(char *in);
char	**dir_get_items_search(char *in, t_autoc *autoc);
char	**order_tab_ascii(char **tabl);
void	init_key_func(t_autoc *autoc);
void	ft_ul_video(char *str);
void	ft_ul(char *str);
void	ft_video(char *str);
int	autoc_arrow_process(int arrow_mask[3], t_autoc *autoc);

#endif
