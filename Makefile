# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jjaniec <jjaniec@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/03/05 21:53:56 by jjaniec           #+#    #+#              #
#    Updated: 2018/07/04 12:46:12 by jjaniec          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 21sh
TESTS_EXEC = $(addprefix $(NAME),_tests)

UNAME_S := $(shell uname -s)

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
			ast/ast_construct.c \
			ast/check_parsing.c \
			ast/ast_utils.c \
			ast/ast_utils_node.c \
			ast/ast_debug.c \
			log.c \
			main.c

INCLUDES_NAME = lexer.h \
				ast.h \
				twenty_one_sh.h \
				log.h

TESTS_SRC_NAME =	lexer_tests.c \
					ast_tests.c \
					main.c

SRC_DIR = ./srcs/
INCLUDES_DIR = ./includes/
TESTS_DIR = ./tests/
OBJ_DIR = ./objs/
OBJ_SUBDIRS = lexer/ ast/
FT_PRINTF_DIR = ./ft_printf/
LIBTAP_DIR = libtap

SRC = $(addprefix $(SRC_DIR), $(SRC_NAME))
OBJ = $(addprefix $(OBJ_DIR), $(SRC_NAME:.c=.o))
TESTS_OBJ = $(addprefix $(TESTS_DIR),$(TESTS_SRC_NAME:.c=.o))
TESTS_SRCS_OBJS_NAME = $(subst ./objs/main.o,,$(OBJ)) $(TESTS_OBJ) $(addprefix $(LIBTAP_DIR),"/tap.o")

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -D_GNU_SOURCE
DEBUG?=0
DEBUG_FLAGS = -D DEBUG=$(DEBUG) -DLOG_USE_COLOR
#DEV_FLAGS = -fsanitize=address -fno-omit-frame-pointer
COVERAGE_FLAGS = -coverage -O0
IFLAGS = -I$(FT_PRINTF_DIR)/includes -I$(INCLUDES_DIR)
LFLAGS = -L$(FT_PRINTF_DIR) -lftprintf

LIBTAP_DIR = libtap
LIBTAP_FLAGS = -I$(LIBTAP_DIR) -L$(LIBTAP_DIR) -ltap

CFLAGS += $(DEV_FLAGS)
LIBFTPRINTF = $(addprefix $(FT_PRINTF_DIR),"libftprintf.a")

MAKEFILE_STATUS = $(addprefix $(addprefix $(FT_PRINTF_DIR),"libft/"),".makefile_status")

UNAME_S := $(shell uname -s)

define ui_line
	$(MAKEFILE_STATUS) $(1) $(2) || true
endef

CFLAGS += $(DEBUG_FLAGS)

all : $(NAME)

verbose: DEBUG=1
verbose: $(NAME)

$(NAME) : $(LIBFTPRINTF) $(OBJ)
ifeq ($(UNAME_S),Linux)
	@$(CC) $(CFLAGS) $(LFLAGS) $(OBJ) $(LIBFTPRINTF) -o $(NAME)
endif
ifeq ($(UNAME_S),Darwin)
	@$(CC) $(CFLAGS) $(LFLAGS) $(OBJ) -o $(NAME)
endif

$(OBJ_DIR)%.o : $(SRC_DIR)%.c $(addprefix $(INCLUDES_DIR), $(INCLUDES_NAME))
	@mkdir -p $(OBJ_DIR) $(addprefix $(OBJ_DIR), $(OBJ_SUBDIRS))
	@$(CC) $(CFLAGS) -c $(IFLAGS) $< -o $@ && $(call ui_line, $@, $(NAME))

$(FT_PRINTF_DIR):
	git clone https://github.com/jjaniec/ft_printf $(FT_PRINTF_DIR) || true

$(LIBFTPRINTF): $(FT_PRINTF_DIR)
	@make -C $(FT_PRINTF_DIR)

$(LIBTAP_DIR):
	git clone https://github.com/zorgnax/libtap.git $(LIBTAP_DIR) || true

$(TESTS_DIR)%.o: $(TESTS_DIR)%.c $(addprefix $(TESTS_DIR),/tests.h)
	$(CC) $(CFLAGS) -c $(IFLAGS) $< -o $@

$(TESTS_EXEC): $(LIBFTPRINTF) $(OBJ) $(TESTS_OBJ)
	$(CC) -c $(IFLAGS) $(addprefix $(LIBTAP_DIR),"/tap.c") -o $(addprefix $(LIBTAP_DIR),"/tap.o")
	$(CC) $(CFLAGS) $(LFLAGS) $(TESTS_SRCS_OBJS_NAME) $(LIBFTPRINTF) -o $(TESTS_EXEC)

tests: CFLAGS += $(COVERAGE_FLAGS)
tests: $(LIBTAP_DIR) $(TESTS_EXEC)

coverage: tests
	gcov $(subst ./objs/log.o,,$(TESTS_SRCS_OBJS_NAME)) 2> /dev/null

clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(FT_PRINTF_DIR)
	rm -rf $(addprefix $(TESTS_DIR),*.o)

fclean: clean
	rm -f $(NAME)

ffclean: fclean
	rm -rf ft_printf
	rm -rf libtap

re: fclean all

.PHONY: fclean re all verbose
