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

CXXFLAGS += -std=c++98 -Wno-c++0x-compat
CXXFLAGS += -MMD

# warning flags
WFLAGS :=	-Wall -Wextra -Werror -Wpedantic -Wshadow

# extended warning flags
# some of this warnings are enabled in -Wall or -Wextra
# but I like to explicitly put the ones that are related either way
LINUX_EWFLAGS :=	-Wuninitialized -Wmaybe-uninitialized \
					-Wdouble-promotion \
					-Wformat -Wformat-overflow -Wformat-truncation -Wformat-security \
					-Wnull-dereference \
					-Winit-self \
					-Wmissing-include-dirs \
					-Wunused -Wunused-const-variable=1 \
					-Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wmissing-noreturn -Wsuggest-attribute=malloc \
					-Wsuggest-attribute=format -Wmissing-format-attribute -Wsuggest-attribute=cold \
					-Walloc-zero \
					-Warray-bounds \
					-Wbool-compare -Wbool-operation \
					-Wduplicated-branches -Wduplicated-cond \
					-Wcast-qual -Wcast-align \
					-Wparentheses -Wdangling-else \
					-Wconversion -Wfloat-conversion -Wsign-conversion -Wsign-compare \
					-Waddress \
					-Wlogical-op -Wlogical-not-parentheses \
					-Wmissing-field-initializers \
					-Wredundant-decls \
					-Winline \
					-Wvla

OSX_EWFLAGS :=		-Wuninitialized \
					-Wdouble-promotion \
					-Wformat -Wformat-security \
					-Wnull-dereference \
					-Winit-self \
					-Wmissing-include-dirs \
					-Wunused -Wunused-const-variable \
					-Warray-bounds \
					-Wcast-qual -Wcast-align \
					-Wparentheses -Wdangling-else \
					-Wconversion -Wfloat-conversion -Wsign-conversion -Wsign-compare \
					-Waddress \
					-Wlogical-not-parentheses \
					-Wmissing-field-initializers \
					-Wredundant-decls \
					-Winline \
					-Wvla

LINUX_ASAN :=	-fsanitize=address \
				-fsanitize=pointer-compare \
				-fsanitize=pointer-subtract \
				-fsanitize=leak \
				-fsanitize=undefined \
				-fsanitize=shift \
				-fsanitize=shift-exponent \
				-fsanitize=shift-base \
				-fsanitize=integer-divide-by-zero \
				-fsanitize=vla-bound \
				-fsanitize=null \
				-fsanitize=return \
				-fsanitize=signed-integer-overflow \
				-fsanitize=bounds \
				-fsanitize=bounds-strict \
				-fsanitize=alignment \
				-fsanitize=object-size \
				-fsanitize=float-divide-by-zero \
				-fsanitize=float-cast-overflow \
				-fsanitize=nonnull-attribute \
				-fsanitize=returns-nonnull-attribute \
				-fsanitize=bool \
				-fsanitize=enum \
				-fsanitize=vptr \
				-fsanitize=pointer-overflow \
				-fsanitize=builtin

OSX_ASAN :=		-fsanitize=address \
				-fsanitize=pointer-compare \
				-fsanitize=pointer-subtract \
				-fsanitize=undefined \
				-fsanitize=shift \
				-fsanitize=shift-exponent \
				-fsanitize=shift-base \
				-fsanitize=integer-divide-by-zero \
				-fsanitize=vla-bound \
				-fsanitize=null \
				-fsanitize=return \
				-fsanitize=signed-integer-overflow \
				-fsanitize=bounds \
				-fsanitize=alignment \
				-fsanitize=float-divide-by-zero \
				-fsanitize=float-cast-overflow \
				-fsanitize=nonnull-attribute \
				-fsanitize=returns-nonnull-attribute \
				-fsanitize=bool \
				-fsanitize=enum \
				-fsanitize=vptr \
				-fsanitize=pointer-overflow \
				-fsanitize=builtin

# **************************************************************************** #
#                                    PATHS                                     #
# **************************************************************************** #

SRC_PATH = src
OBJ_PATH = obj
INC_PATH = include
LIB_PATH = lib

# **************************************************************************** #
#                                    FLAGS                                     #
# **************************************************************************** #

CXXFLAGS += -I ./$(INC_PATH)
CXXFLAGS += -I ./$(SRC_PATH)

# **************************************************************************** #
#                                     LIBS                                     #
# **************************************************************************** #

CXXFLAGS += -I ./$(LIB_PATH)/nstd/include

# **************************************************************************** #
#                                      OS                                      #
# **************************************************************************** #

ifeq ($(UNAME_S),Linux)
	CXXFLAGS += -D LINUX
endif
ifeq ($(UNAME_S),Darwin)
	CXXFLAGS += -D OSX
endif

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

SRC_ROOT		=	main.cc

SRC_CONFIG		=	constants.cc		config.cc			server_config.cc

SRC_SERVER		=	server.cc			client.cc

SRC_REQUEST		=	request.cc			request_data.cc

SRC_RESPONSE	=	response.cc			response_data.cc

SRC_CGI			=	cgi.cc

SRC_UTILS		=	utils.cc			signals.cc

SRC_NAME =	$(SRC_ROOT)														\
			$(addprefix $(SRC_DIR_CONFIG)/,		$(SRC_CONFIG))				\
			$(addprefix $(SRC_DIR_SERVER)/,		$(SRC_SERVER))				\
			$(addprefix $(SRC_DIR_REQUEST)/,	$(SRC_REQUEST))				\
			$(addprefix $(SRC_DIR_RESPONSE)/,	$(SRC_RESPONSE))			\
			$(addprefix $(SRC_DIR_CGI)/,		$(SRC_CGI))					\
			$(addprefix $(SRC_DIR_UTILS)/,		$(SRC_UTILS))

OBJ_NAME = $(SRC_NAME:%.cc=%.o)
DEP_NAME = $(SRC_NAME:%.cc=%.d)

SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_NAME))
DEP = $(addprefix $(OBJ_PATH)/, $(DEP_NAME))

# **************************************************************************** #
#                                    RULES                                     #
# **************************************************************************** #

PHONY := all
all: $(NAME)

$(NAME): CXXFLAGS += $(WFLAGS) $(EWFLAGS)
$(NAME): $(OBJ)
	@printf "\n${YEL}LINKING:${NOCOL}\n"
	@printf "${BLU}"
	$(CXX) $(CXXFLAGS) $(DEBUG) $(OBJ) -o $@
	@printf "${NOCOL}"
	@printf "\n${GRN}SUCCESS!${NOCOL}\n"
	@printf "${CYN}type \"./${NAME}\" to start!${NOCOL}\n"

PHONY += debug
ifeq ($(UNAME_S),Linux)
debug: EWFLAGS := $(LINUX_EWFLAGS)
endif
ifeq ($(UNAME_S),Darwin)
debug: EWFLAGS := $(OSX_EWFLAGS)
endif
debug: $(NAME)

PHONY += install
install: $(NAME)
	install $(NAME) $(BIN_DIR)

PHONY += sanitize
ifeq ($(UNAME_S),Linux)
sanitize: DEBUG := -g3 $(LINUX_ASAN)
endif
ifeq ($(UNAME_S),Darwin)
sanitize: DEBUG := -g3 $(OSX_ASAN)
endif
sanitize: debug

PHONY += valgrind
valgrind: DEBUG := -ggdb3
valgrind: debug

PHONY += thread
thread: DEBUG := -g3 -fsanitize=thread
thread: debug

# OBJ
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cc | $(OBJ_PATH) $(OBJ_DIRS)
	@printf "${BLU}"
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@
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

# RUN
PHONY += run
ifeq ($(UNAME_S),Darwin)
export ASAN_OPTIONS = detect_leaks=1
endif
run: CONF ?= ./conf/all.conf
run: $(NAME)
	@printf "\n${YEL}RUNNING...${NOCOL}\n"
	./$(NAME) $(CONF)
	@printf "\n${GRN}SUCCESS!${NOCOL}\n"

-include $(DEP)

.PHONY: $(PHONY)
