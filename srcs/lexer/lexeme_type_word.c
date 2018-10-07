/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexeme_type_word.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/14 14:44:31 by jjaniec           #+#    #+#             */
/*   Updated: 2018/09/30 18:57:03 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** If a quote is found, call this to
** search for next (simple/double)quote in string $s
** and skip all chars in quotes instead stopping to next $IFS separator
*/

static int	skip_quotes_substring(char *s, int *pos)
{
	char	*quote_pos;

	quote_pos = has_matching_quote(s + *pos, s[*pos]);
	if (!(quote_pos))
		return (UNMATCHED_QUOTE_ERR);
	*pos = ((quote_pos - s) / sizeof(char));
	return (0);
}

/*
** Determines if string should be passed to handle_quotes_expansions
** to remove quotes / backslashes / replace expansion specifiers
*/

static int	is_clean_needed_expansions(char **data)
{
	char	*ptr;

	ptr = EXPANSIONS_SPECIFIERS;
	while (*ptr)
		if (ft_strchr(*data, *ptr++))
			return (1);
	return (0);
}

static int	is_clean_needed(char **data)
{
	if (*data && (ft_strchr(*data, '\'') || \
			ft_strchr(*data, '"') || ft_strchr(*data, '\\') || \
			is_clean_needed_expansions(data)))
		return (1);
	return (0);
}
/*
** Parse word operators (default type),
** if string $s do not contains quotes,
** a substring will be made to the next $IFS separator,
** if ' or " characters are found, apply skip_quotes_substring
** to skip $IFS separators until corresponding quote
*/

int			lexeme_type_word(char *s, int *pos, char **data)
{
	int		start;

	start = *pos;
	while (s[*pos] && !is_separator(s[*pos]) && !is_operator(s[*pos]))
	{
		if (s[*pos] == '\\')
			*pos += handle_escape_offset(s + *pos, NOT_IN_QUOTES);
		else if (s[*pos] == '\'' || s[*pos] == '"')
		{
			if (skip_quotes_substring(s, pos))
				return (UNMATCHED_QUOTE_ERR);
		}
		if (s[*pos])
			*pos += 1;
	}
	if (start != *pos)
	{
		*data = ft_strsub(s, start, *pos - start);
		if (is_clean_needed(data))
			handle_quotes_expansions(data);
	}
	else
		*data = NULL;
	if (!(env_assigns_status(0, 0)) && ft_strchr(*data, '='))
		return (T_ENV_ASSIGN);
	env_assigns_status(1, 1);
	return (T_WORD);
}
