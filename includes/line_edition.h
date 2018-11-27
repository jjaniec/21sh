/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edition.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyfermie <cyfermie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/22 15:45:45 by cyfermie          #+#    #+#             */
/*   Updated: 2018/11/09 17:13:34 by cyfermie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINE_EDITION_H

# define LINE_EDITION_H

//////////////////////////////////////////////////////// debug

# include <stdio.h>
# define TTY_DEBUG "/dev/ttys000"

# ifndef FOOLOL
	extern FILE *tty_debug;
# endif

# define LE_DEBUG_STATUS_SET	1

int		get_le_debug_status(int mode, int new_value);
# define le_debug(str, ...) if (get_le_debug_status(0, 0)) {fprintf(tty_debug, str, __VA_ARGS__); }

////////////////////////////////////////////////////////

# include <forty_two_sh.h>

/*
**	Sizes
*/
# define LE_DEFAULT_LINE_SIZE (8192U)
# define LE_KEY_BUFFER_SIZE (9U)

/*
**	Action Keys
*/
# define LE_NB_KEYS (25)
# define LE_ARROW_UP ((27) + (91 << 1) + (65 << 2))
# define LE_ARROW_DOWN ((27) + (91 << 1) + (66 << 2))
# define LE_ARROW_RIGHT ((27) + (91 << 1) + (67 << 2))
# define LE_ARROW_LEFT ((27) + (91 << 1) + (68 << 2))
# define LE_CTRL_A (1)
# define LE_HOME ((27) + (91 << 1) + (72 << 2))
# define LE_CTRL_E (5)
# define LE_END ((27) + (91 << 1) + (70 << 2))
# define LE_CTRL_UP ((27) + (91 << 1) + (49 << 2) + (59 << 3) + (53 << 4) + (65 << 5))
# define LE_CTRL_DOWN ((27) + (91 << 1) + (49 << 2) + (59 << 3) + (53 << 4) + (66 << 5))
# define LE_ALT_RIGHT ((27) + (27 << 1) + (91 << 2) + (67 << 3))
# define LE_ALT_LEFT  ((27) + (27 << 1) + (91 << 2) + (68 << 3))
# define LE_BACKSPACE (127)
# define LE_TAB (9)
# define LE_DELETE ((27) + (91 << 1) + (51 << 2) + (126 << 3))
# define LE_CTRL_B (2)
# define LE_CTRL_F (6)
# define LE_CTRL_R (18)
# define LE_CTRL_P (16)
# define LE_CTRL_U (21)
# define LE_CTRL_DASH (31)
# define LE_CTRL_OPEN_SQUARE_BRACKET (27)
# define LE_CTRL_CLOSE_SQUARE_BRACKET (29)
# define LE_CTRL_D (4)
# define LE_CTRL_L (12)

/*
	Heredoc
	AND
	OR
	Backslash
	Quotes simples, doubles et back quotes
*/

/*
**	Things
*/
# define LE_CHAR_DELIM_DEFAULT (t_kno)'\n'
# define RESIZE_IN_PROGRESS ((char *)(-1))
# define LE_DEFAULT_PROMPT (0)
# define LE_IFS (char []){'\t', '\n', ' ', '\0'}
# define LE_SPECIAL_CASE (42)

/*
**	Data type representing a key number
*/
typedef uint64_t t_kno;

/*
**	For read_key() function
*/
enum e_read_key
{
	INTR_BY_SIGINT = 0,
	ALL_IS_ALRIGHT = 1,
	INTR_BY_SIGWINCH = 2
};

/*
**	For set_term_attr() function
*/
enum e_set_term
{
	LE_SET_NEW,
	LE_SET_OLD
};
typedef enum e_set_term t_set_term;


/*
**	For cursor_crosses_screen() function
*/
enum e_cross_screen
{
	CROSS_TO_LEFT,
	CROSS_TO_RIGHT
};
typedef enum e_cross_screen t_cross_screen;

/*
**	opt_colosyn : true if the syntax highlighting feature is active.
**	le_is_init : true if the main datas structure is initialized,
**				it's useful to know which datas need to be freed.
**	prompt_type : type of the current prompt, useful to reprint it if necessary,
**				  and to handle EOF behavior depending of the prompt type.
*/

struct s_le_state
{
	t_kno	le_char_delim;
	bool	opt_colosyn;
	bool	le_is_init;
	int		prompt_type;
};

/*
**	nd : cursor moves one step right
**	le : cursor moves one step left
**	_do : (because 'do' is a standard keyword) cursor moves one line down
**	up : cursor moves one line up
**	dc : delete character under the cursor
**	cl : clear screen
**	md : begin bold mode
**	me : end bold mode
**	cr : cursor_pos 0
**	cd : clear all after cursor
**	dl : Delete a line
**	al : add a line
**	us : begin underline mode
**	mr : begin reverse mode
**	ue : end underline mode
*/

struct s_le_termcaps
{
	const char	*nd;
	const char	*le;
	const char	*_do;
	const char	*up;
	const char	*dc;
	const char	*cl;
	const char	*md;
	const char	*me;
	const char	*cr;
	const char	*cd;
	const char	*dl;
	const char	*al;
	const char	*us;
	const char	*mr;
	const char	*ue;
};

/*
**	Main datas structure for line_edition feature.
**
**	le_state : informations about the current line edition.
**	key_no : number representing the key(s) pressed by the user.
**	cmd : buffer containing the command line.
**	cmd_size : size in bytes of the buffer 'cmd'.
**	cmd_len : number of characters stored in the buffer 'cmd'.
**	cursor_index : position of the cursor for the buffer 'cmd'.
**	start_pos : starting position of the command line,
**				depending on the prompt's length.
**	cursor_pos : position of the cursor on its line (0 is the first position,
**				 'term_line_size - 1' is the last position on the line).
**	cursor_line : line where the cursor is for the current command line, (0 is
**				  the first line, 'nb_lines_written - 1' is the last one).
**	term_line_size : size of a line for the current window
**					 (same as the number of columns).
**	nb_lines_written : number of lines currently written for the
**					   current command line.
**	nb_char_on_last_line : number of characters currently written on the last
**						   line of the current command line.
**	clipboard : buffer containing the shell's clipboard.
**	clipboard_size : size in bytes of the buffer 'clipboard'.
**	clipboard_len : number of characters stored in the buffer 'clipboard'.
**	tcaps : termcaps strings
**	history : currently pointed element of the linked list representing
**			  the shell's history.
**	save_tmp_cmd : buffer useful to save the command line when navigating
**				   into the shell's history.
*/

struct s_line
{
	struct s_le_state		le_state;

	char					key_buffer[LE_KEY_BUFFER_SIZE];
	t_kno					key_no;
	char					*cmd;
	size_t					cmd_size;
	size_t					cmd_len;

	unsigned int			cursor_index;
	unsigned int			start_pos;
	unsigned int			cursor_pos;
	unsigned int			cursor_line;
	unsigned int			term_nb_lines;
	size_t					term_line_size;
	unsigned int			nb_lines_written;
	unsigned int			nb_char_on_last_line;

	char					*clipboard;
	size_t					clipboard_size;
	size_t					clipboard_len;

	struct s_le_termcaps	*tcaps;
	struct s_history		*history;
	char					*save_tmp_cmd;
};

/*
**	cmd : buffer containing a command line.
**	next : pointer to the next newest element.
**	prev : pointer to the next oldest elememt.
**
**	The most 'next' element is the newest of the list,
**	the most 'prev' element is the oldest.
*/

struct s_history
{
	char 				*cmd;
	struct s_history	*next;
	struct s_history	*prev;
};

/*
**	For the action_key() function.
**	key_no : number representing the key(s) pressed by the user.
**	func_ptr : function pointer in order to get the function
**			   corresponding to 'key_no'.
*/

struct s_action_key
{
	t_kno	key_no;
	void	(*func_ptr)(struct s_line *);
};

/*
**	Informations to help rewriting and replacing the cursor.
**	Use only in delete_char_into_cmdline_backspace_mode().
**	This structure use to be more useful with more datas, now that the code
**	is optimized, it's not so useful anymore ...
*/

struct s_infos_for_rewriting
{
	unsigned int	nb_line_to_go_up;
	unsigned int	pos_end_rewriting;
};


/*
**	actionk
*/
void	action_key(t_kno key, struct s_line *le_lettr);
void	actionk_clear_screen(struct s_line *le);
void    actionk_copy_all(struct s_line *le);
void    actionk_copy_to_end(struct s_line *le);
void    actionk_copy_to_start(struct s_line *le);
void	actionk_cursor_move_left(struct s_line *le);
void	actionk_cursor_move_right(struct s_line *le);
void	actionk_cut_all(struct s_line *le);
void	actionk_cut_to_end(struct s_line *le);
void	actionk_cut_to_start(struct s_line *le);
void	actionk_delete_character(struct s_line *le);
void	actionk_delete_current_input(struct s_line *le);
void	actionk_eof(struct s_line *le);
void    actionk_history_down(struct s_line *le);
void    actionk_history_up(struct s_line *le);
void    actionk_move_cursor_by_word_left(struct s_line *le);
void    actionk_move_cursor_by_word_right(struct s_line *le);
void	actionk_move_cursor_end(struct s_line *le);
void    actionk_move_cursor_line_down(struct s_line *le);
void    actionk_move_cursor_line_up(struct s_line *le);
void	actionk_move_cursor_start(struct s_line *le);
void	actionk_past_clipboard(struct s_line *le);
void	delete_char_into_cmdline_backspace_mode(struct s_line *le);
void	delete_char_into_cmdline_delete_mode(struct s_line *le);

/*
**	boolean_check
*/
bool	cursor_is_at_end_of_cmd(struct s_line *le);
bool	cursor_is_at_end_of_term_line(unsigned int cursorpos,
									  struct s_line *le);
bool    le_is_separator(char c);
bool 	possible_to_go_right(struct s_line *le);

/*
**	colosyn
*/
void	colosyn_add_char(struct s_line *le, t_kno key);
void	colosyn_cut_to_start(struct s_line *le);
void	colosyn_cut_to_end(struct s_line *le);
void	colosyn_delete_char(struct s_line *le);
void	colosyn_past_clipboard(struct s_line *le);
void	colosyn_print_history_elem(struct s_line *le);
void	refresh_colosyn(struct s_line *le, char *cmd);

/*
**	init_le
*/
void					init_line_edition_attributes(struct s_line *le,
													 int prompt_type);
struct s_le_termcaps	*init_termcaps_strings(void);
void					set_term_attr(t_set_term mode);

/*
**	print
*/
void			insert_and_print_character_into_cmdline(struct s_line *le, t_kno key);
void			print_history_cmd(struct s_line *le);
void			print_key_at_end(struct s_line *le, t_kno key);
void    		print_key(t_kno key);
unsigned int	print_str_on_term(const char *str,
								  unsigned int tmp_current_cursor_pos,
								  struct s_line *le, int foo);


/*
**	tools
*/
void			check_cmd_storage(struct s_line *le, unsigned int nb_char);
void			check_clipboard_storage(struct s_line *le, unsigned int nb_char);
void			cursor_crosses_screen(struct s_line *le, t_cross_screen direction);
unsigned int	get_terminal_nb_lines(void);
unsigned int	get_terminal_nb_col(void);
void			insert_char_into_array(struct s_line *le, t_kno key, unsigned int pos);
void    		reset_history_on_first_elem(struct s_line *le);
void			weird_trick_to_erase_char(struct s_line *le);
int				write_one_char(int c);

/*
**	SDF (sans dossier fixe)
*/
struct s_line	*access_le_main_datas(void);
void			add_history(const char *input, struct s_line *le);
void    		handle_window_resize(struct s_line *le);
void			le_free_datas(void);
void			le_free_history(struct s_line *le);
char			*line_edition(int prompt_type);
t_kno			get_key_number(const char *key);
void			process_key(struct s_line *le);




bool	still_enough_space_for_cmd(struct s_line *le);

#endif



/*
	mkdir test ; cd test ; ls -a ; ls | cat | wc -c > fifi ; cat fifi
	CETTE COMMANDE NE MARCHE PAS, ELLE PEUT MEME SEGFAULT ...
	FAUDRA VERIFIER CA ULTRA IMPORTANT

	TESTER  (echo line1; echo line2) | ./42sh

	BUILTINS BONUS A FAIRE (avec leurs options)
	{
		read
		export
		unset
	}


*/



/*

MINIMUM REQUIS

IL FAUDRA QUE JE CHECK MOI MEME SI LES BUILTINS ONT LES BONNES OPTIONS
ET LES BONS COMPORTEMENTS

cd
{
	options -p -L et - (dash tout court)
	http://pubs.opengroup.org/onlinepubs/9699919799/utilities/cd.html
	https://www.unix.com/man-page/posix/1posix/cd/
}

echo
{
	https://www.unix.com/man-page/posix/1posix/echo
	(oui le -n n'est pas obligatoire mais go le faire c'est facile et pratique non ?)
}

exit
{
	https://www.unix.com/man-page/posix/1posix/exit
}

env
{
	http://pubs.opengroup.org/onlinepubs/9699919799/utilities/env.html
	https://www.unix.com/man-page/posix/1posix/env
}

setenv
{
	pas posix ce bultin mdr, pas d'option donc go faire la meme syntaxe
	que csh qui a "invente" setenv : $ setenv variable value
	(le nombre de blancs entre variable et value peut etre plus que 1)

	si il y a 3 arguments ou plus, setenv: Too many arguments.
	si il y a un seul argument, sa valeur est une chaine vide
	exemple :
	$ setenv lol
	$ env
	var=value
	var=value
	...
	lol=


}

unsetenv
{
	si ya aucun argument, Too few arguments.
	tous les arguments sont a traiter, on peut unsetenv autant d'arguments au'on veut
}

history
{
	https://www.gnu.org/software/bash/manual/html_node/Bash-History-Builtins.html
}

*/
