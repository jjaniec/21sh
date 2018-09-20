/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_word_lexeme.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/21 15:18:07 by jjaniec           #+#    #+#             */
/*   Updated: 2018/09/20 16:16:29 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

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

static int		is_expansion_char(char c, void **callback)
{
	*callback = NULL;
	if (c == '$')
		*callback = handle_dollar_expansion;
	/*else if (c == '~')
		*callback = handle_tild_expansion;*/
	return ((*callback) ? (1) : (0));
}

/*
** Fills new created data string
** $s: old data string
** $new_data: ptr to new data string
*/

static void		fill_new_data_str(char *s, char **new_data)
{
	int		i;
	int		j;
	int		in_quote_type;
	char	*jump_ptr;
	void	*expansion_handler;

	j = 0;
	i = 0;
	jump_ptr = s;
	while (s[i])
		if (is_expansion_char(s[i], &expansion_handler))
		{
			(*(void (*)(char *, char **, char **, int *))(expansion_handler))\
				(s + i, g_envp, &s, &i);
			
		}
		else if (s[i] == '\\')
		{
			i += handle_escape_offset(s + i, NOT_IN_QUOTES);
			(*new_data)[j++] = s[i++];
		}
		else if (((s)[i] == '\'' || (s)[i] == '"') && \
			is_quote_removable(s + i, &jump_ptr, &in_quote_type))
		{
			i++;
			while (s[i] && (s + i) != jump_ptr)
			{
				if (s[i] == '\\')
					i += handle_escape_offset(s + i, in_quote_type);
				(*new_data)[j++] = s[i++];
			}
			if (s[i])
				i++;
		}
		else
			(*new_data)[j++] = s[i++];
	(*new_data)[j] = '\0';
}

/*
** Removes ' and " characters from given string by calculating
** count of characters to remove, creating a new string and filling it
** without escaping characters '\' and quotes pairs
*/

void			clean_word_lexeme(char **data)
{
	char	*new_data;

	new_data = malloc(ft_strlen(*data) * sizeof(char) + 1);
	log_trace("Filling new cleaned str of |%s|", *data);
	fill_new_data_str(*data, &new_data);
	free(*data);
	*data = new_data;
	log_debug("Replaced old data w/ |%s|", *data);
}
