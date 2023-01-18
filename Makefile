# **************************************************************************** #
#                                     VARS                                     #
# **************************************************************************** #

# COLORS
NOCOL=\033[0m
RED=\033[31m
GRN=\033[32m
YEL=\033[33m
BLU=\033[34m
MAG=\033[35m
CYN=\033[36m
LBLU = \033[36m
LGRN = \033[0;90m

# OS
UNAME_S := $(shell uname -s)

# **************************************************************************** #
#                                   PROGRAM                                    #
# **************************************************************************** #

NAME = webserv

BIN_DIR ?= /usr/local/bin

# **************************************************************************** #
#                                   COMPILER                                   #
# **************************************************************************** #

MAKE = make

CXX = g++

CXXFLAGS += -std=c++98 -Wall -Wextra -Werror -MD

# **************************************************************************** #
#                                    PATHS                                     #
# **************************************************************************** #

SRC_PATH = src
OBJ_PATH = obj
INC_PATH = include

# **************************************************************************** #
#                                    FLAGS                                     #
# **************************************************************************** #

CXXFLAGS += -I ./$(INC_PATH)
CXXFLAGS += -I ./$(SRC_PATH)

# **************************************************************************** #
#                                   SOURCES                                    #
# **************************************************************************** #

SRC_DIR_CONFIG	= config
SRC_DIR_SERVER	= server
SRC_DIR_UTILS	= utils

OBJ_DIRS_NAME =	$(SRC_DIR_CONFIG)	$(SRC_DIR_SERVER)	$(SRC_DIR_UTILS)

#OBJ_DIRS_NAME =	$(SRC_DIR_MAIN)		$(SRC_DIR_BUILTIN)	$(SRC_DIR_EXEC)		\
				$(SRC_DIR_LEXER)	$(SRC_DIR_PARSER)	$(SRC_DIR_PROMPT)	\
				$(SRC_DIR_UTILS)

OBJ_DIRS = $(addprefix $(OBJ_PATH)/, $(OBJ_DIRS_NAME))

SRC_ROOT	=	main.cpp

SRC_CONFIG	=	Config.cpp

SRC_SERVER	=	ServerController.cpp	\
				Server.cpp

SRC_UTILS	=	log.cpp

#SRC_MAIN = 		handle_line.c
#
#SRC_BUILTIN =	cd.c				echo.c				env.c				\
#				exit.c				export.c			pwd.c				\
#				unset.c				cd_error.c			export_utils.c		\
#				cd_utils.c			unset_utils.c
#
#SRC_EXEC =		exec.c				exec_ast.c			exec_cmd.c			\
#				utils.c				exec_heredoc.c		heredoc.c			\
#				handle_zombies.c	redir.c				redir_types.c		\
#				path.c
#
#SRC_LEXER =		lexer.c				utils_1.c			utils_2.c			\
#				pc_gen_st.c			pc_other_st.c		process_tokens.c	\
#				token_utils.c		handle_expand.c		expand.c			\
#				wildcards.c			wc_match.c			wc_match_frag.c		\
#				wc_utils.c			wc_check.c
#
#SRC_PARSER =	parser.c			parser_utils.c		ast_utils_1.c		\
#				ast_utils_2.c		parse_cmd_line.c	parse_and_or_1.c	\
#				parse_and_or_2.c	parse_job.c			parse_cmd.c			\
#				parse_tok_lst.c		parse_redir.c		parse_redir_in.c	\
#				parse_redir_out.c
#
#SRC_PROMPT =	prompt.c

SRC_NAME =	$(SRC_ROOT)														\
			$(addprefix $(SRC_DIR_CONFIG)/, $(SRC_CONFIG))					\
			$(addprefix $(SRC_DIR_SERVER)/, $(SRC_SERVER))					\
			$(addprefix $(SRC_DIR_UTILS)/, $(SRC_UTILS))

#SRC_NAME =	$(SRC_ROOT)														\
#			$(addprefix $(SRC_DIR_MAIN)/, $(SRC_MAIN))						\
#			$(addprefix $(SRC_DIR_BUILTIN)/, $(SRC_BUILTIN))				\
#			$(addprefix $(SRC_DIR_EXEC)/, $(SRC_EXEC))						\
#			$(addprefix $(SRC_DIR_LEXER)/, $(SRC_LEXER))					\
#			$(addprefix $(SRC_DIR_PARSER)/, $(SRC_PARSER))					\
#			$(addprefix $(SRC_DIR_PROMPT)/, $(SRC_PROMPT))

OBJ_NAME = $(SRC_NAME:%.cpp=%.o)

SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

PHONY := all
all: $(NAME) ## default rule, compile web server

$(NAME): $(OBJ) $(LFT_NAME)
	@printf "\n${YEL}LINKING:${NOCOL}\n"
	@printf "${BLU}"
	$(CXX) $(CXXFLAGS) $(OBJ) $(LFT_NAME) -o $@ $(LDFLAGS) $(LDLIBS) $(CXXFLAGS)
	@printf "${NOCOL}"
	@printf "\n${GRN}SUCCESS!${NOCOL}\n"
	@printf "${CYN}type \"./${NAME}\" to start!${NOCOL}\n"

PHONY += install
install: $(NAME) ## install binary on the system
	install $(NAME) $(BIN_DIR)

PHONY += sanitize
ifeq ($(UNAME_S),Linux)
sanitize: CXXFLAGS += -pedantic -g3 -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=bounds -fsanitize=null
endif
ifeq ($(UNAME_S),Darwin)
sanitize: CXXFLAGS += -pedantic -g3 -fsanitize=address
endif
sanitize: $(NAME) ## compile with pedantic, debug symbol, and a bunch of sanitizes

PHONY += thread
thread: CXXFLAGS += -g3 -fsanitize=thread
thread: $(NAME) ## compile with thread sanitize

# OBJ
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp | $(OBJ_PATH) $(OBJ_DIRS)
	@printf "${BLU}"
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "${NOCOL}"

# OBJ DIRS
$(OBJ_DIRS): | $(OBJ_PATH)
	@printf "${MAG}"
	mkdir -p $(OBJ_DIRS)
	@printf "${NOCOL}"
$(OBJ_PATH):
	@printf "${MAG}"
	mkdir -p $(OBJ_PATH)
	@printf "${NOCOL}"

PHONY += clean
clean: ## clean objects and dependencies
	@printf "${RED}"
	rm -rf $(OBJ_PATH)
	@printf "${NOCOL}"

PHONY += fclean
fclean: clean ## clean everything
	@printf "${RED}"
	rm -rf $(NAME)
	@printf "${NOCOL}"

PHONY += re
re: fclean all ## redo all

PHONY += help
help: ## shows help
	@echo "\n$(LBLU)_______________________________ $(NAME) _______________________________$(NOCOL)"
	@echo "\n\tUsage: 'make $(LBLU)<command>$(NOCOL)'\n"
	@grep -E '^[a-z.A-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "${LBLU}%-20s${NOCOL} %s\n", $$1, $$2}'
#	@echo "\n\t$(LGRN)Using [Argument] $(LBLU)'V=1'$(LGRN) will show all the building output$(NOCOL)"
	@echo "\n$(LBLU)_______________________________________________________________________$(NOCOL)\n\n"

.PHONY: $(PHONY)
