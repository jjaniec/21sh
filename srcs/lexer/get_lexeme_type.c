/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_lexeme_type.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 15:35:59 by jjaniec           #+#    #+#             */
/*   Updated: 2018/08/18 15:43:57 by jjaniec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <twenty_one_sh.h>

/*
** Makes a substring of our operator to store it in lexeme->data
*/

static size_t	store_optlexeme(char *s, int data_len, \
				int *pos, char **data, size_t type)
{
	*data = ft_strsub(s, 0, data_len);
	*pos += data_len;
	return (type);
}

/*
** Parse control operators (see lexer.h) and create a substring of it in *data
*/

static size_t	lexeme_type_ctrlopt(char *s, int *pos, \
					char **data, size_t *type_details)
{
	int		data_len;

	if (*s == '&' || *s == '|' || *s == ';' || *s == '\n')
	{
		if (*s != ';' && *s == s[1])
		{
			if (*s == '&')
				*type_details = TK_DAND;
			else if (*s == '|')
				*type_details = TK_OR;
			data_len = 2;
		}
		else
		{
			if (*s == '&')
				*type_details = TK_AND;
			else if (*s == '|')
				*type_details = TK_PIPE;
			else if (*s == ';')
				*type_details = TK_SEMICOLON;
			else if (*s == '\n')
				*type_details = TK_NEWLINE;
			data_len = 1;
		}
		return (store_optlexeme(s, data_len, pos, data, T_CTRL_OPT));
	}
	return (0);
}

/*
** Parse redirection operators (see lexer.h)
** and create a substring of it in *data
*/

static size_t	lexeme_type_rediropt(char *s, int *pos, \
					char **data, size_t *type_details)
{
	int		data_len;

	if (*s == '<' || *s == '>')
	{
		if (s[1] == '&' || s[1] == '>' || \
			(*s == '>' && s[1] == '|') || (s[1] == *s))
		{
			if (*s == '<' && s[1] == *s && (s[2] == *s || s[2] == '-'))
			{
				if (s[2] == '-')
					*type_details = TK_DLESSDASH;
				else if (s[2] == *s)
					*type_details = TK_TLESS;
				data_len = 3;
			}
			else
			{
				if (s[1] == '&')
				{
					if (*s == '<')
						*type_details = TK_LESSAND;
					else if (*s == '>')
						*type_details = TK_GREATAND;
				}
				else if (s[1] == '>')
				{
					if (*s == '>')
						*type_details = TK_DGREAT;
					else if (*s == '<')
						*type_details = TK_LESSGREAT;
				}
				else if (s[1] == '<')
					*type_details = TK_DLESS;
				else if (s[1] == '|' && *s == '>')
					*type_details = TK_CLOBBER;
				data_len = 2;
			}
		}
		else
		{
			if (*s == '<')
				*type_details = TK_LESS;
			else if (*s == '>')
				*type_details = TK_GREAT;
			data_len = 1;
		}
		return (store_optlexeme(s, data_len, pos, data, T_REDIR_OPT));
	}
	return (0);
}

/*
** Determines if lexeme is composed of a redirection operator and numbers
** before it to specify
** fd to redirect, if a redirection operator is found after numbers,
** remake data string and *pos offset
*/

static size_t	is_redir_inputfd(char *s, int *pos, \
					char **data, size_t *type_details)
{
	int		i;
	int		start;
	char	*new_data;

	i = *pos;
	start = i;
	new_data = NULL;
	while (s[i] && s[i] >= '0' && s[i] <= '9')
		i++;
	if (lexeme_type_rediropt(s + i, pos, data, type_details))
	{
		new_data = ft_strsub(s + start, 0, ((i + *pos) - 2 * start));
		free(*data);
		*data = new_data;
		*pos = start + ft_strlen(*data);
		return (T_REDIR_OPT);
	}
	return (0);
}

/*
** Determines type of lexeme (type list in lexer.h)
** If an operator is found, call env_assign_status to toggle creation
** of T_ENV_ASSIGN elements
*/

size_t			get_lexeme_type(char *s, int *pos, \
					char **data, size_t *type_details)
{
	if (!s)
		return (0);
	if (is_operator(s[*pos]) || (s[*pos] >= '0' && s[*pos] <= '9'))
	{
		env_assigns_status(*"stop creating T_ENV_ASSIGN elements", 1);
		if (s[*pos] >= '0' && s[*pos] <= '9')
		{
			if (is_redir_inputfd(s, pos, data, type_details))
				return (T_REDIR_OPT);
		}
		else if (lexeme_type_ctrlopt(s + *pos, pos, data, type_details))
			return (T_CTRL_OPT);
		else if (lexeme_type_rediropt(s + *pos, pos, data, type_details))
			return (T_REDIR_OPT);
	}
	return (lexeme_type_word(s, pos, data));
}
