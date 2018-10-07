# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/05 21:53:56 by jjaniec           #+#    #+#              #
#    Updated: 2018/10/02 14:26:50 by jjaniec          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###### EXEC ######
NAME = 42sh
TESTS_EXEC = $(addprefix $(NAME),_tests)

###### FILES ######
SRC_NAME = 	is_separator.c \
			lexer/lexer.c \
			lexer/get_lexeme_type.c \
			lexer/create_lexeme.c \
			lexer/is_operator.c \
			lexer/lexeme_type_word.c \
			lexer/handle_quotes_expansions.c \
			lexer/has_matching_quote.c \
			lexer/env_assigns_status.c \
			lexer/handle_escape_offset.c \
			lexer/free_lexemes.c \
			lexer/handle_char_expansion.c \
			lexer/get_expansion_end.c \
			ast/ast.c \
			ast/ast_explore.c \
			ast/ast_constructor.c \
			ast/ast_constructor_subast.c \
			ast/put_node.c \
			ast/node_placer_classic.c \
			ast/node_placer_if.c \
			ast/node_placer_while.c \
			ast/check_parsing.c \
			ast/lvl_lex.c \
			ast/is_op.c \
			ast/ast_utils_node.c \
			ast/ast_debug.c \
			ast/ast_free.c \
			ast/prepare_argv.c \
			ast/create_node.c \
			ast/access_ast_data.c \
			line_edition/access_le_main_datas.c \
			line_edition/add_history.c \
			line_edition/le_exit.c \
			line_edition/get_le_debug_status.c \
			line_edition/le_free_datas_and_history.c \
			line_edition/line_edition.c \
			line_edition/process_key.c \
			line_edition/actionk/action_key.c \
			line_edition/actionk/actionk_clear_screen.c \
			line_edition/actionk/actionk_copy_all.c \
			line_edition/actionk/actionk_copy_to_end.c \
			line_edition/actionk/actionk_copy_to_start.c \
			line_edition/actionk/actionk_cursor_move_left.c \
			line_edition/actionk/actionk_cursor_move_right.c \
			line_edition/actionk/actionk_cut_all.c \
			line_edition/actionk/actionk_cut_to_end.c \
			line_edition/actionk/actionk_cut_to_start.c \
			line_edition/actionk/actionk_delete_character.c \
			line_edition/actionk/actionk_delete_current_input.c \
			line_edition/actionk/actionk_eof.c \
			line_edition/actionk/actionk_history_down.c \
			line_edition/actionk/actionk_history_up.c \
			line_edition/actionk/actionk_move_cursor_by_word_left.c \
			line_edition/actionk/actionk_move_cursor_by_word_right.c \
			line_edition/actionk/actionk_move_cursor_end.c \
			line_edition/actionk/actionk_move_cursor_line_down.c \
			line_edition/actionk/actionk_move_cursor_line_up.c \
			line_edition/actionk/actionk_move_cursor_start.c \
			line_edition/actionk/actionk_past_clipboard.c \
			line_edition/tools/check_cmd_and_clipboard_storage.c \
			line_edition/colosyn/colosyn_add_char.c \
			line_edition/colosyn/colosyn_cut_to_start_or_end.c \
			line_edition/colosyn/colosyn_delete_char.c \
			line_edition/colosyn/colosyn_past_clipboard.c \
			line_edition/colosyn/colosyn_print_history_elem.c \
			line_edition/tools/cursor_crosses_screen.c \
			line_edition/boolean_check/cursor_is_at_end_of_cmd.c \
			line_edition/boolean_check/cursor_is_at_end_of_term_line.c \
			line_edition/actionk/delete_char_into_cmdline_backspace_mode.c \
			line_edition/actionk/delete_char_into_cmdline_delete_mode.c \
			line_edition/tools/get_terminal_nb_col.c \
			line_edition/init_le/init_line_edition_attributes.c \
			line_edition/signals/init_signals.c \
			line_edition/signals/handle_sigint.c \
			line_edition/signals/handle_sigwinch.c \
			line_edition/init_le/init_termcaps_strings.c \
			line_edition/print/insert_and_print_character_into_cmdline.c \
			line_edition/tools/insert_char_into_array.c \
			line_edition/boolean_check/is_separator.c \
			line_edition/boolean_check/possible_to_go_right.c \
			line_edition/print/print_history_cmd.c \
			line_edition/print/print_key_at_end.c \
			line_edition/print/print_key.c \
			line_edition/print/print_str_on_term.c \
			line_edition/colosyn/refresh_colosyn.c \
			line_edition/tools/reset_history_on_first_elem.c \
			line_edition/init_le/set_term_attr.c \
			line_edition/tools/weird_trick_to_erase_char.c \
			line_edition/tools/write_one_char.c \
			exec/exec.c \
			exec/exec_pre_in_post.c \
			exec/exec_thread.c \
			exec/io_manager.c \
			exec/manage_path.c \
			exec/handle_redirs.c \
			exec/handle_redir_fd.c \
			exec/handle_pipes.c \
			exec/handle_open_error.c \
			exec/print_error.c \
			exec/init_pipe_data.c \
			exec/get_last_pipe_node.c \
			exec/free_exec.c \
			builtin/builtin_history.c \
			builtin/builtin_cd.c \
			builtin/builtin_exit.c \
			builtin/builtin_setenv.c \
			builtin/builtin_toggle_syntax_highlighting.c \
			builtin/builtin_unsetenv.c \
			builtin/builtin_echo.c \
			builtin/builtin_env.c \
			builtin/builtin_return.c \
			builtin/builtin_test.c \
			builtin/is_builtin.c \
			script/script_lexer.c \
			script/good_start.c \
			script/script_ast_construct.c \
			script/find_end_lexeme.c \
			script/find_end_lexeme_solo.c \
			script/find_lexeme_compete.c \
			script/script_in_exec.c \
			script/is_script_complete.c \
			syntax_highlighting/print_colorized_input.c \
			syntax_highlighting/print_lexeme_colorized.c \
			autocomplete/autoc_check_path.c \
			log.c \
			ft_free_argv.c \
			sub_prompt.c \
			init_globals_config.c \
			parse_options.c \
			format_help.c \
			get_opt_elem.c \
			is_option_activated.c \
			syntax_highlighting/print_input_string_end.c \
			env/add_env_var.c \
			env/init_environ.c \
			env/init_environ_struct_ptrs.c \
			env/del_env_var.c \
			env/get_env_var.c \
			env/upd_env_var.c \
			free_all_shell_data.c \
			env/free_env_entries.c \
			history_file_checker.c \
			load_history_file.c \
			get_next_line.c \
			get_parsed_history_file_path.c \
			ft_strjoin_path.c \
			get_shell_vars.c \
			main.c

INCLUDES_NAME = lexer.h \
				ast.h \
				exec.h \
				forty_two_sh.h \
				line_edition.h \
				script.h \
				syntax_highlighting.h \
				log.h \
				struct.h \
				get_next_line.h

TESTS_SRC_NAME =	lexer_tests.c \
					syntax_highlighting_tests.c \
					ast_tests.c \
					exec_tests.c \
					builtins_tests.c \
					script_tests.c \
					builtin_test_tests.c \
					test_lexeme_list.c \
					compare_sh_42sh_outputs.c \
					compare_redirected_files_contents.c \
					compare_fds_with_strings.c \
					redirect_both_fds.c \
					main.c


###### FOLDERS ######
SRC_DIR = ./srcs/
INCLUDES_DIR = ./includes/
TESTS_DIR = ./tests/
OBJ_DIR = ./objs/
OBJ_SUBDIRS = lexer/ ast/ exec/ builtin/ line_edition/ line_edition/actionk/ \
			line_edition/colosyn/ line_edition/init_le line_edition/boolean_check \
			line_edition/print line_edition/signals line_edition/tools \
			syntax_highlighting/ script/ autocomplete/ env/
FT_PRINTF_DIR = ./ft_printf/
LIBTAP_DIR = ./libtap/

###### SRC / OBJ ######
SRC = $(addprefix $(SRC_DIR), $(SRC_NAME))
OBJ = $(addprefix $(OBJ_DIR), $(SRC_NAME:.c=.o))
TESTS_OBJ = $(addprefix $(TESTS_DIR),$(TESTS_SRC_NAME:.c=.o))
TESTS_SRCS_OBJS_NAME = $(subst ./objs/main.o,,$(OBJ)) $(TESTS_OBJ) $(addprefix $(LIBTAP_DIR),"/tap.o")

###### COMPILATION ######
CC = gcc
CFLAGS = -Wall -Wextra -g -D_GNU_SOURCE -std=c11 # -Werror

### FLAGS ###
VERBOSE_MODE = 0
VERBOSE_MODE_FLAGS = -DVERBOSE_MODE=$(VERBOSE_MODE) -DLOG_USE_COLOR
#DEV_FLAGS = -fsanitize=address -fno-omit-frame-pointer
CFLAGS += $(DEV_FLAGS)
COVERAGE_CFLAGS = -coverage -O0
IFLAGS = -I$(FT_PRINTF_DIR)/includes -I$(INCLUDES_DIR)

### LIB ###
LFLAGS = -L$(FT_PRINTF_DIR) -lftprintf -lncurses
LIBTAP_FLAGS = -I$(LIBTAP_DIR) -L$(LIBTAP_DIR) -ltap
LIBFTPRINTF = $(addprefix $(FT_PRINTF_DIR),libftprintf.a)
### VERBOSE - COVERAGE ###
CFLAGS += $(VERBOSE_MODE_FLAGS)
### CROSS-COMPIL ###
UNAME_S := $(shell uname -s)
MAKEFILE_STATUS = $(addprefix $(addprefix $(FT_PRINTF_DIR),"libft/"),".makefile_status.sh")

define ui_line
	$(MAKEFILE_STATUS) $(1) $(2) || true
endef

###### RULES ######
.PHONY: fclean re all verbose
all : $(NAME)

verbose: VERBOSE_MODE=1
verbose: $(NAME)

$(NAME) : $(LIBFTPRINTF) $(OBJ)
ifeq ($(UNAME_S),Linux)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFTPRINTF) -o $(NAME) $(LFLAGS)
endif
ifeq ($(UNAME_S),Darwin)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LFLAGS)
endif

$(TESTS_EXEC): $(LIBFTPRINTF) $(OBJ) $(TESTS_OBJ)
	@$(CC) -c $(IFLAGS) $(addprefix $(LIBTAP_DIR),"/tap.c") -o $(addprefix $(LIBTAP_DIR),"/tap.o")
	@$(CC) $(CFLAGS) $(TESTS_SRCS_OBJS_NAME) $(LIBFTPRINTF) -o $(TESTS_EXEC) $(LFLAGS)

tests: CFLAGS += $(COVERAGE_CFLAGS)
tests: all $(LIBTAP_DIR) $(TESTS_EXEC)

coverage: tests
	gcov $(subst ./objs/log.o,,$(TESTS_SRCS_OBJS_NAME))

re: fclean all


###### OBJ RULES ######
$(OBJ_DIR)%.o : $(SRC_DIR)%.c $(addprefix $(INCLUDES_DIR), $(INCLUDES_NAME))
	@mkdir -p $(OBJ_DIR) $(addprefix $(OBJ_DIR), $(OBJ_SUBDIRS))
	@$(CC) $(CFLAGS) -c $(IFLAGS) $< -o $@ && $(call ui_line, $@, $(NAME))

$(TESTS_DIR)%.o: $(TESTS_DIR)%.c $(addprefix $(TESTS_DIR),/tests.h)
	$(CC) $(CFLAGS) -c $(IFLAGS) $< -o $@


###### LIB RULES ######
$(FT_PRINTF_DIR):
	git clone https://github.com/jjaniec/ft_printf $(FT_PRINTF_DIR) || true

$(LIBFTPRINTF): $(FT_PRINTF_DIR)
	@make -C $(FT_PRINTF_DIR)

$(LIBTAP_DIR):
	git clone https://github.com/zorgnax/libtap.git $(LIBTAP_DIR) || true


###### CLEAN RULES ######
clean:
	-rm -rf $(OBJ_DIR)
	-rm -rf $(addprefix $(TESTS_DIR),*.o)
	-rm -rf *.gcov tests/*.{gcda,gcno} *.dSYM
	-make clean -C $(FT_PRINTF_DIR)

fclean: clean
	-rm -f $(NAME)
	-make fclean -C $(FT_PRINTF_DIR)

ffclean: fclean
	-rm -rf $(FT_PRINTF_DIR)
	-rm -rf $(LIBTAP_DIR)
