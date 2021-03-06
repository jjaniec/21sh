/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_lexeme_type.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/13 15:35:59 by jjaniec           #+#    #+#             */
/*   Updated: 2018/12/10 17:58:26 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <forty_two_sh.h>

/*
** Makes a substring of our operator to store it in lexeme->data
*/

int			store_optlexeme(char *s, int integer[2], int *pos, char **data)
{
	int	data_len;
	int	type;

	data_len = integer[0];
	type = integer[1];
	*data = ft_strsub(s, 0, data_len);
	if (*data == NULL)
		exit(MALLOC_ERROR);
	*pos += data_len;
	return (type);
}

/*
** Parse control operators (see lexer.h) and create a substring of it in *data
*/

static int	lexeme_type_ctrlopt(char *s, int *pos, \
					char **data, int *type_details)
{
	int		data_len;

	if (*s == '&' || *s == '|' || *s == ';' || *s == '\n')
	{
		if (*s != ';' && *s == s[1] && *s != '\n' && (data_len = 2))
		{
			if (*s == '&')
				*type_details = TK_DAND;
			else if (*s == '|')
				*type_details = TK_OR;
		}
		else
		{
			if ((data_len = 1) && *s == '&')
				*type_details = TK_AND;
			else if (*s == '|')
				*type_details = TK_PIPE;
			else if (*s == ';')
				*type_details = TK_SEMICOLON;
			else if (*s == '\n')
				*type_details = TK_NEWLINE;
		}
		return (store_optlexeme(s, (int[2]){data_len, T_CTRL_OPT}, pos, data));
	}
	return (0);
}

/*
** Determines if lexeme is composed of a redirection operator and numbers
** before it to specify
** fd to redirect, if a redirection operator is found after numbers,
** remake data string and *pos offset
*/

static int	is_redir_inputfd(char *s, int *pos, \
					char **data, int *type_details)
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
		if (new_data == NULL)
			exit(MALLOC_ERROR);
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

int			get_lexeme_type(char *s, int *pos, \
					char **data, int *type_details)
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
	return (lexeme_type_word(s, pos, data, *pos));
}
