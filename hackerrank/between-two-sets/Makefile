# executable name and path
BIN_NAME = between-two-sets
BIN_PATH = ./ 

CC ?= gcc

# source file extensions
C_EXT = c
CPP_EXT = cpp

CCFLAGS = -pthread
CXXFLAGS = -pthread
LDFLAGS = -pthread

LAST_MAKEFILE := $(abspath $(lastword $(MAKEFILE_LIST)))
SRC_DIR := $(patsubst %/,%,$(dir $(LAST_MAKEFILE))) 
ROOT_PATH := $(patsubst %/,%,$(dir $(SRC_DIR)))

# include subdires
INCLUDES = -I. \
		   -I$(ROOT_PATH) \
		   -I/usr/local/include/

CC_COMPILE_FLAGS = -Wall -Wextra -Wcast-align -Werror=return-type -g -O0 -Ddebug
CXX_COMPILE_FLAGS = -std=c++17 -Wall -Wextra -Wcast-align -Werror=return-type -g -O0 -Ddebug

# source files
C_SOURCES = $(wildcard *.$(C_EXT)) 
CPP_SOURCES = $(wildcard *.$(CPP_EXT))
# object files
OBJECTS = $(C_SOURCES:%.$(C_EXT)=%.o) $(CPP_SOURCES:%.$(CPP_EXT)=%.o)
# dependency files
DEPS = $(OBJECTS:.o=.d)


.PHONY: default
default: all;

.PHONY: all
all: $(BIN_NAME)

$(OBJECTS): $(C_SOURCES)
	$(VERBOSE)$(CC) $(CCFLAGS) $(CC_COMPILE_FLAGS) $(INCLUDES) -c $(realpath $<) -o $@

# Link the application
$(BIN_NAME): $(OBJECTS) $(DEFS_FILE)
	@echo "Linking: $@"
	$(VERBOSE)$(CC) $(OBJECTS) $(LIBS) $(LDFLAGS) -o $@
