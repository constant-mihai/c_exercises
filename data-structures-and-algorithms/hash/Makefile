# executable name and path
BIN_NAME = hash
BIN_PATH = ./ 

CC ?= gcc
CXX ?= g++

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
		   -I/usr/local/include/ \
		   -I../../

CC_COMPILE_FLAGS = -Wall -Wextra -Wcast-align -Werror=return-type -g -O0 -Ddebug -fPIC
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

shl_linker_name := lib$(BIN_NAME).so
shl_version := 1
shl_release_number := 0
shl_minor_number := 0
shl_soname := $(shl_linker_name).$(shl_version)
shared_flags := -shared -Wl,-soname,$(shl_soname)
shl_fullname := $(shl_soname).$(shl_minor_number).$(shl_release_number)

$(OBJECTS): $(C_SOURCES)
	$(VERBOSE)$(CC) $(CCFLAGS) $(CC_COMPILE_FLAGS) $(INCLUDES) -c $(realpath $<) -o $@

sharedlib: $(OBJECTS) $(DEFS_FILE)
	$(VERBOSE)$(CC) $(shared_flags) $(OBJECTS) -o $(shl_fullname) $(LDFLAGS)
	ln -sf $(shl_fullname) $(shl_soname)
	ln -sf $(shl_fullname) $(shl_linker_name)

# Link the application
$(BIN_NAME): $(OBJECTS) $(DEFS_FILE)
	@echo "Linking: $@"
	$(VERBOSE)$(CC) $(OBJECTS) $(LIBS) $(LDFLAGS) -o $@

