# **************************************************************************** #
#                                     VARS                                     #
# **************************************************************************** #

# COLORS
NOCOL	= \033[0m
RED		= \033[31m
GRN		= \033[32m
YEL		= \033[33m
BLU		= \033[34m
MAG		= \033[35m
CYN		= \033[36m
LBLU	= \033[36m
LGRN	= \033[0;90m

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
INC_PATH = inc

# **************************************************************************** #
#                                    FLAGS                                     #
# **************************************************************************** #

CXXFLAGS += -I ./$(INC_PATH)
CXXFLAGS += -I ./$(SRC_PATH)

# **************************************************************************** #
#                                   SOURCES                                    #
# **************************************************************************** #

SRC_DIR_CONFIG		= config
SRC_DIR_SERVER		= server
SRC_DIR_REQUEST		= request
SRC_DIR_RESPONSE	= response
SRC_DIR_CGI			= cgi
SRC_DIR_UTILS		= utils

OBJ_DIRS_NAME =	$(SRC_DIR_CONFIG)	$(SRC_DIR_SERVER)	$(SRC_DIR_REQUEST)	\
				$(SRC_DIR_RESPONSE)	$(SRC_DIR_CGI)		$(SRC_DIR_UTILS)

OBJ_DIRS = $(addprefix $(OBJ_PATH)/, $(OBJ_DIRS_NAME))

SRC_ROOT		=	main.cpp

SRC_CONFIG		=	constants.cpp		Config.cpp			ServerConfig.cpp

SRC_SERVER		=	Server.cpp			Client.cpp

SRC_REQUEST		=	Request.cpp			RequestData.cpp

SRC_RESPONSE	=	Response.cpp		ResponseData.cpp

SRC_CGI			=	Cgi.cpp

SRC_UTILS		=	utils.cpp			signals.cpp

SRC_NAME =	$(SRC_ROOT)														\
			$(addprefix $(SRC_DIR_CONFIG)/,		$(SRC_CONFIG))				\
			$(addprefix $(SRC_DIR_SERVER)/,		$(SRC_SERVER))				\
			$(addprefix $(SRC_DIR_REQUEST)/,	$(SRC_REQUEST))				\
			$(addprefix $(SRC_DIR_RESPONSE)/,	$(SRC_RESPONSE))			\
			$(addprefix $(SRC_DIR_CGI)/,		$(SRC_CGI))					\
			$(addprefix $(SRC_DIR_UTILS)/,		$(SRC_UTILS))

OBJ_NAME = $(SRC_NAME:%.cpp=%.o)
DEP_NAME = $(SRC_NAME:%.cpp=%.d)

SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))
DEP = $(addprefix $(OBJ_PATH)/, $(DEP_NAME))

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

PHONY := all
all: $(NAME) ## default rule, compile web server

$(NAME): $(OBJ)
	@printf "\n${YEL}LINKING:${NOCOL}\n"
	@printf "${BLU}"
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@
	@printf "${NOCOL}"
	@printf "\n${GRN}SUCCESS!${NOCOL}\n"
	@printf "${CYN}type \"./${NAME}\" to start!${NOCOL}\n"

PHONY += install
install: $(NAME)
	install $(NAME) $(BIN_DIR)

PHONY += sanitize
ifeq ($(UNAME_S),Linux)
sanitize: CXXFLAGS += -Wpedantic -Wconversion -g3 -fsanitize=address -fsanitize=leak -fsanitize=undefined -fsanitize=bounds -fsanitize=null
endif
ifeq ($(UNAME_S),Darwin)
sanitize: CXXFLAGS += -Wpedantic -Wconversion -g3 -fsanitize=address
endif
sanitize: $(NAME)

PHONY += valgrind
valgrind: CXXFLAGS += -ggdb3
valgrind: $(NAME)

PHONY += thread
thread: CXXFLAGS += -g3 -fsanitize=thread
thread: $(NAME)

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
clean:
	@printf "${RED}"
	rm -rf $(OBJ_PATH)
	@printf "${NOCOL}"

PHONY += fclean
fclean: clean
	@printf "${RED}"
	rm -rf $(NAME)
	@printf "${NOCOL}"

PHONY += re
re: fclean all

-include $(DEP)

.PHONY: $(PHONY)
