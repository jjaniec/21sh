# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/05 21:53:56 by jjaniec           #+#    #+#              #
#    Updated: 2018/08/22 16:05:44 by jjaniec          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###### EXEC ######
NAME = 21sh
TESTS_EXEC = $(addprefix $(NAME),_tests)

###### FILES ######
SRC_NAME = 	is_separator.c \
			lexer/lexer.c \
			lexer/get_lexeme_type.c \
			lexer/create_lexeme.c \
			lexer/is_operator.c \
			lexer/lexeme_type_word.c \
			lexer/clean_word_lexeme.c \
			lexer/has_matching_quote.c \
			lexer/env_assigns_status.c \
			lexer/handle_backslash_escape.c \
			lexer/free_lexemes.c \
			ast/ast.c \
			ast/ast_explore.c \
			ast/ast_construct.c \
			ast/check_parsing.c \
			ast/lvl_lex.c \
			ast/is_op.c \
			ast/ast_utils_node.c \
			ast/ast_debug.c \
			line_edition/action_key.c \
			line_edition/actionk_cursor_move_left.c \
			line_edition/actionk_cursor_move_right.c \
			line_edition/actionk_delete_character.c \
			line_edition/actionk_move_cursor_by_word_left.c \
			line_edition/actionk_move_cursor_by_word_right.c \
			line_edition/actionk_move_cursor_end.c \
			line_edition/actionk_move_cursor_line_down.c \
			line_edition/actionk_move_cursor_line_up.c \
			line_edition/actionk_move_cursor_start.c \
			line_edition/cursor_is_at_end.c \
			line_edition/errors.c \
			line_edition/init_line_edition_attributes.c \
			line_edition/init_termcaps_strings.c \
			line_edition/insert_character_into_cmdline.c \
			line_edition/is_separator.c \
			line_edition/line_edition.c \
			line_edition/possible_to_go_right.c \
			line_edition/print_key_at_end.c \
			line_edition/process_key.c \
			line_edition/set_term_attr.c \
			line_edition/write_one_char.c \
			ast/ast_free.c \
            ast/prepare_argv.c \
			exec/exec.c \
			exec/exec_pre_in_post.c \
			exec/exec_thread.c \
			exec/io_manager.c \
			exec/manage_env.c \
			exec/manage_path.c \
			exec/handle_redirs.c \
			exec/handle_redir_fd.c \
			exec/handle_pipes.c \
			exec/handle_open_error.c \
			exec/print_error.c \
			exec/init_pipe_data.c \
			exec/get_last_pipe_node.c \
			exec/free_exec.c \
			builtin/builtin_cd.c \
			builtin/builtin_exit.c \
			builtin/builtin_setenv.c \
			builtin/builtin_unsetenv.c \
			builtin/builtin_echo.c \
			builtin/builtin_env.c \
			builtin/builtin_return.c \
			builtin/is_builtin.c \
			log.c \
			ft_free_argv.c \
			main.c

INCLUDES_NAME = lexer.h \
				ast.h \
				exec.h \
				twenty_one_sh.h \
				line_edition.h \
				log.h

TESTS_SRC_NAME =	lexer_tests.c \
					ast_tests.c \
					exec_tests.c \
					main.c


###### FOLDERS ######
SRC_DIR = ./srcs/
INCLUDES_DIR = ./includes/
TESTS_DIR = ./tests/
OBJ_DIR = ./objs/
OBJ_SUBDIRS = lexer/ ast/ exec/ builtin/ line_edition/
FT_PRINTF_DIR = ./ft_printf/
LIBTAP_DIR = libtap

###### SRC / OBJ ######
SRC = $(addprefix $(SRC_DIR), $(SRC_NAME))
OBJ = $(addprefix $(OBJ_DIR), $(SRC_NAME:.c=.o))
TESTS_OBJ = $(addprefix $(TESTS_DIR),$(TESTS_SRC_NAME:.c=.o))
TESTS_SRCS_OBJS_NAME = $(subst ./objs/main.o,,$(OBJ)) $(TESTS_OBJ) $(addprefix $(LIBTAP_DIR),"/tap.o")

###### COMPILATION ######
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -D_GNU_SOURCE

### FLAGS ###
VERBOSE_MODE = 0
VERBOSE_MODE_FLAGS = -DVERBOSE_MODE=$(VERBOSE_MODE) -DLOG_USE_COLOR
#DEV_FLAGS = -fsanitize=address -fno-omit-frame-pointer
CFLAGS += $(DEV_FLAGS)
COVERAGE_CFLAGS = -coverage -O0
IFLAGS = -I$(FT_PRINTF_DIR)includes -I$(INCLUDES_DIR)

### LIB ###
LFLAGS = -L$(FT_PRINTF_DIR) -lftprintf -lncurses
LIBTAP_FLAGS = -I$(LIBTAP_DIR) -L$(LIBTAP_DIR) -ltap
LIBFTPRINTF = $(addprefix $(FT_PRINTF_DIR),libftprintf.a)
### VERBOSE - COVERAGE ###
CFLAGS += $(VERBOSE_MODE_FLAGS)
### CROSS-COMPIL ###
UNAME_S := $(shell uname -s)
MAKEFILE_STATUS = $(addprefix $(addprefix $(FT_PRINTF_DIR),"libft/"),".makefile_status")

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
	rm -rf $(OBJ_DIR)
	if [ -d $(FT_PRINTF_DIR) ]; then make fclean -C $(FT_PRINTF_DIR); fi
	rm -rf $(addprefix $(TESTS_DIR),*.o)
	rm -rf *.gcov tests/*.{gcda,gcno} *.dSYM

fclean: clean
	rm -f $(NAME)

ffclean: fclean
	rm -rf ft_printf
	rm -rf libtap
