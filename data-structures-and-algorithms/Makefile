######################################################################
# Recipes.
######################################################################
include Makefile.in

#
# Phony 
# ----------------------------- #
# ----------------------------- #
.PHONY: all subdirs test-subdirs clean cleanall debug debug-shared shared help clean-subdirs

verbose = $(if $(or $(findstring yes,$(v)),$(findstring 1, $(v))),,@)

h := $(wildcard *.h) $(call deep_source_search,$(subdirs),$(h_ext))
hpp := $(wildcard *.hpp) $(call deep_source_search,$(subdirs),$(hpp_ext))
incs := $(h) $(hpp)

test_h := $(call deep_source_search,$(test_subdirs),$(h_ext))
test_hpp := $(call deep_source_search,$(test_subdirs),$(hpp_ext))
test_incs := $(test_h) $(test_hpp)

c_srcs := $(wildcard *.$(c_ext)) $(call deep_source_search,$(subdirs),$(c_ext))
cpp_srcs := $(wildcard *.$(cpp_ext)) $(call deep_source_search,$(subdirs),$(cpp_ext))
srcs := $(c_srcs) $(cpp_srcs)

test_c_srcs := $(call deep_source_search,$(test_subdirs),$(c_ext))
test_cpp_srcs := $(call deep_source_search,$(test_subdirs),$(cpp_ext))
test_srcs := $(test_c_srcs) $(test_cpp_srcs)

basenames := $(basename $(srcs))
test_basenames := $(basename $(test_srcs))
objs := $(addsuffix .o,$(basenames))
test_objs := $(addsuffix .o,$(test_basenames))
deps = $(objs:.o=.d)

#
# Compile shared libs
# -------------------------------------- #
# -------------------------------------- #
define compile_shared_lib_fn
$(CC) $(shared_flags) $(objs) $(LDFLAGS) $(LIBRARY_DIRS) $(LIBRARIES) -o $(shl_fullname)
ln -sf $(shl_fullname) $(shl_soname)
ln -sf $(shl_fullname) $(shl_linker_name)
endef

#
# Recipes
#
all: subdirs $(objs) # TODO library prerequisite
	@echo Linking executable.
	$(verbose)$(CC) $(objs) $(LDFLAGS) $(LIBRARY_DIRS) $(LIBRARIES) -o $(name)

subdirs:
	@echo Compiling application objects.
	$(verbose)for s in $(subdirs) ; do \
		echo "cd $$s/" ; \
		$(MAKE) $(PARALLEL) --no-print-directory -C $$s ; \
	done;

-include $(DEPS)

%.o: %.$(cpp_ext)
	@echo "    $< -> $@"
	$(verbose)$(CXX) $(dependency_files) $(CPPFLAGS) $(optimize_flags) $(defines) $(INCLUDES) -c $(realpath $<) -o $@

%.o: %.$(c_ext)
	@echo "    $< -> $@"
	$(verbose)$(CC) $(dependency_files) $(CFLAGS) $(optimize_flags) $(defines) $(INCLUDES) -c $(realpath $<) -o $@

shared: export CFLAGS += -fPIC
shared: subdirs
	@echo Linking shared lib.
	@echo $(CFLAGS)
	$(verbose)$(compile_shared_lib_fn)

test: subdirs test-subdirs $(objs)
	@echo Linking test executable.
	$(verbose)$(CXX) -Wl,--start-group $(objs) $(test_objs) $(TEST_LDFLAGS) $(TEST_LIBRARY_DIRS) $(TEST_LIBRARIES) -Wl,--end-group -o $(name)_test

test-subdirs: $(test_srcs) $(test_incs)
	@echo Compiling test objects.
	$(verbose)for s in $(test_subdirs) ; do \
		echo "cd $$s/" ; \
		$(MAKE) $(PARALLEL) --no-print-directory -C $$s ; \
	done;

clean: clean-subdirs
	rm -f *.o; rm -f *.so*;

test-clean:
	@for s in $(test_subdirs) ; do \
		echo "Cleaning $$s/" ; \
		$(MAKE) --no-print-directory -C $$s clean ; \
		echo "Exit $$s/" ; \
	done;

clean-subdirs:
	@for s in $(subdirs) ; do \
		echo "Cleaning $$s/" ; \
		$(MAKE) --no-print-directory -C $$s clean ; \
		echo "Exit $$s/" ; \
	done;


cleanall: 
	$(if $(findstring yes,$(recursive)),$(clean_subdirectories),)
	$(clean_main_fn)

debug: CPPFLAGS += -g
debug: all 

debug-shared: CPPFLAGS += -g
debug-shared: shared

help:
	@$(MAKE) --print-data-base --question all | \
	grep -v -e '^no-such-target' -e '^makefile' |       \
    awk '/^[^.%][-A-Za-z0-9_]*:/                        \
          { print substr($$1, 1, length($$1)-1) }' |      \
    sort 

# Alternatively:
# awk '/^[^.%][-A-Za-z0-9_]*:/ { print }' |      \

print-%: ; @echo $* = $($*)

print-all: ;
	 @echo ------------------------------ General
	 @echo SHELL                = $(SHELL)
	 @echo name                 = $(name) 
	 @echo ------------------------------------------ Shared library
	 @echo shl_name           = $(shl_name) 
	 @echo shl_version        = $(shl_version)
	 @echo shl_release_number = $(shl_release_number) 
	 @echo shl_minor_number   = $(shl_minor_number)
	 @echo shl_linker_name    = $(shl_linker_name)
	 @echo shl_soname         = $(shl_soname)
	 @echo shl_fullname       = $(shl_fullname)

	 @echo ------------------------------------------ Directories  
	 @echo subdirs              = $(subdirs) 
	 @echo CURR_DIR             = $($CURR_DIR)
	 @echo include_dirs = $(include_dirs) 
	 @echo library_dirs = $(library_dirs)
	 @echo libraries    = $(libraries)

	 @echo ---------------------------- Compiler 
	 @echo CC                   = $(CC) 
	 @echo CXX                  = $(CXX) 

	 @echo ---------------------------- Flags
	 @echo shared               = $(shared)
	 @echo CFLAGS               = $(CFLAGS)
	 @echo CPPFLAGS             = $(CPPFLAGS)
	 @echo LDFLAGS              = $(LDFLAGS)

	 @echo ---------------------------- Sources 
	 @echo c_srcs       = $(c_srcs)
	 @echo c_srcs       = $(c_srcs)
	 @echo cxx_srcs     = $(cxx_srcs)
	 @echo ---------------------------- Objects 
	 @echo cxx_objs     = $(cxx_objs)
	 @echo c_objs       = $(c_objs)
	 @echo ---------------------------- Headers 
	 @echo h            = $(h)
	 @echo hpp          = $(hpp)
	 @echo cap_h        = $(cap_h)
