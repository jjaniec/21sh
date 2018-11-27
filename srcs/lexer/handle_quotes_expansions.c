/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_expansions.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 15:18:07 by jjaniec           #+#    #+#             */
/*   Updated: 2018/11/19 20:17:56 by sbrucker         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Returns 1 if *s character should be removed or not
** $s: data string
** $i: pos in string
** $jump_ptr: set to ptr to position of corresponding quote if passed argument
**            != NULL
*/

static int		is_quote_removable(char *s, char **jump_ptr,
				int *quote_type)
{
	char	*tmp;
	char	c;
	char	*ptr;

	tmp = NULL;
	ptr = s;
	c = *ptr;
	if ((tmp = has_matching_quote(ptr, c)))
	{
		if (jump_ptr)
			*jump_ptr = tmp;
		log_debug("Found removable quote @ : |%s|", tmp);
		if (c == '"')
			*quote_type = IN_DQUOTES;
		else if (c == '\'')
			*quote_type = IN_SQUOTES;
		return (1);
	}
	return (0);
}

/*
** Returns 1 if c is an expansion specifier char and store handler
** in *callback,
** otherwise return 0
*/

static int		is_expansion_char(t_lexeme_clean_data *l, int in_quote_type, \
					void **expansion_handler_ptr)
{
	char	*ptr;

	ptr = *(l->raw_lexeme_read_ptr);
	*expansion_handler_ptr = NULL;
	if (*ptr == '$')
		*expansion_handler_ptr = handle_dollar_expansion;
	else if (*ptr == '~' && \
		in_quote_type == NOT_IN_QUOTES && \
		(ptr == l->raw_lexeme_data || is_separator(*(ptr - 1))) && \
		(!(*(ptr + 1)) || *(ptr + 1) == '/'))
		*expansion_handler_ptr = handle_tild_expansion;
	return ((*expansion_handler_ptr) ? (1) : (0));
}

/*
** Fills new created data string
** $s: old data string
** $new_data: ptr to new data string
*/

static void		fill_new_data_str(t_lexeme_clean_data *l, t_shell_vars *vars)
{
	char	*ptr;
	int		in_quote_type;
	char	*jump_ptr;
	void	*expansion_handler;

	ptr = l->raw_lexeme_data;
	l->raw_lexeme_read_ptr = &ptr;
	jump_ptr = ptr;
	in_quote_type = NOT_IN_QUOTES;
	while (ptr && *ptr)
	{
		//dprintf(1, "ptr = `%s`\n", ptr);
		if (is_expansion_char(l, in_quote_type, &expansion_handler))
			(*(void (*)(t_lexeme_clean_data *, t_shell_vars *))(expansion_handler))\
				(l, vars);
		else if (*ptr == '\\')
		{
			if (*(ptr += sizeof(char) * handle_escape_offset(ptr, NOT_IN_QUOTES)))
				(*(l->clean_data_write_ptr++)) = *(ptr++);
		}
		else if ((*ptr == '\'' || *ptr == '"') && \
			is_quote_removable(ptr, &jump_ptr, &in_quote_type))
		{
			ptr++;
			while (*ptr && ptr != jump_ptr)
			{
				if (in_quote_type == IN_DQUOTES && is_expansion_char(l, in_quote_type, &expansion_handler))
				{
					(*(void (*)(t_lexeme_clean_data *, t_shell_vars *))(expansion_handler))\
						(l, vars);
					continue ;
				}
				else if (*ptr == '\\')
					ptr += \
						sizeof(char) * handle_escape_offset(ptr, in_quote_type);
				(*(l->clean_data_write_ptr++)) = *(ptr++);
				(*(l->clean_data_write_ptr)) = '\0';
			}
			if (*ptr)
				ptr++;
		}
		else
			(*(l->clean_data_write_ptr++)) = *(ptr++);
	}
	*(l->clean_data_write_ptr) = '\0';
}

/*
** Removes ' and " characters from given string,
** creating a new string and filling it
** without escaping characters '\', quotes pairs
** and expansing '$' or '~' expansions when needed
*/

void			handle_quotes_expansions(char **data)
{
	t_lexeme_clean_data		new_lex_data;

	new_lex_data.raw_lexeme_data = *data;
	new_lex_data.clean_data_size = ft_strlen(*data) * sizeof(char) + 1;
	new_lex_data.clean_data = ft_xmalloc(new_lex_data.clean_data_size);
	ft_memset(new_lex_data.clean_data, '\0', new_lex_data.clean_data_size);
	new_lex_data.clean_data_write_ptr = new_lex_data.clean_data;
	fill_new_data_str(&new_lex_data, get_shell_vars());
	free(*data);
	*data = new_lex_data.clean_data;
	log_trace("Replaced old data w/ after quotes & expansions handling |%s|", *data);
}
