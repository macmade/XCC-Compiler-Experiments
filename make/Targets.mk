#-------------------------------------------------------------------------------
# The MIT License (MIT)
# 
# Copyright (c) 2020 Jean-David Gadina - www.xs-labs.com
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# @author           Jean-David Gadina
# @copyright        (c) 2020, Jean-David Gadina - www.xs-labs.com
#-------------------------------------------------------------------------------

# Clear any existing search path
VPATH =
vpath

# Add search paths for source files
vpath %$(EXT_C) $(DIR_SRC)

# Clears any existing suffix
.SUFFIXES:

# Phony targets
.PHONY: all clean

# Precious targets
.PRECIOUS: $(DIR_BUILD_OBJ)%$(EXT_O) $(DIR_BUILD_OBJ)%$(EXT_C)$(EXT_O)

#-------------------------------------------------------------------------------
# Targets with second expansion
#-------------------------------------------------------------------------------

.SECONDEXPANSION:
	
# Clean all build files
clean:
	
	$(call PRINT,,Cleaning all build files)
	@rm -rf $(DIR_BUILD_OBJ)*
	@rm -rf $(DIR_BUILD_BIN)*

# Project
%: _DIRS = $(foreach _F,$(wildcard $(DIR_PROJECTS)$*/*),$(subst /,_,$(_F)))
%: $$(_DIRS)
	
	@:

# Project part
projects_%: _DIR     = $(patsubst %,$(DIR_PROJECTS)%/,$(subst _,/,$*))
projects_%: _PROJECT = $(firstword $(subst _, ,$*))
projects_%: _PART    = $(lastword $(subst _, ,$*))
projects_%: _SOURCE  = $(foreach _F,$(wildcard $(_DIR)*$(EXT_C)),$(_F))
projects_%: _OBJ     = $(foreach _F,$(_SOURCE),$(patsubst %,$(DIR_BUILD_OBJ)$(_PROJECT)_$(_PART)_%$(EXT_O),$(notdir $(_F))))
projects_%: _LIBS    = $(foreach _L,$(LIBS),$(addprefix -l,$(_L)))
projects_%: _TOOL    = $(subst .,-,$(_PROJECT)-$(_PART))
projects_%: $$(_OBJ)
	
	$(call PRINT_FILE,$(_PROJECT) $(_PART),$(HOST_ARCH),$(COLOR_CYAN)Linking executable$(COLOR_NONE),$(COLOR_GRAY)$(_TOOL)$(COLOR_NONE))
	@$(CC) $(ARGS_CC) $(_LIBS) -o $(DIR_BUILD_BIN)$(_TOOL) $(_OBJ)
	
# Avoids stupid search rules...
%$(EXT_C):

# Compiles a C file
$(DIR_BUILD_OBJ)%$(EXT_C)$(EXT_O): _PROJECT = $(firstword $(subst _, ,$*))
$(DIR_BUILD_OBJ)%$(EXT_C)$(EXT_O): _PART    = $(firstword $(patsubst $(_PROJECT)%,%,$(subst _, ,$*)))
$(DIR_BUILD_OBJ)%$(EXT_C)$(EXT_O): _FILE    = $(patsubst %,$(DIR_PROJECTS)%$(EXT_C),$(subst _,/,$*))
$(DIR_BUILD_OBJ)%$(EXT_C)$(EXT_O): $$(_FILE)
	
	$(call PRINT_FILE,$(_PROJECT) $(_PART),$(HOST_ARCH),Compiling C file,$(COLOR_YELLOW)$(_FILE)$(COLOR_NONE) "->" $(COLOR_GRAY)$(notdir $@)$(COLOR_NONE))
	@$(CC) $(ARGS_CC) $(ARGS_C) -o $@ -c $(abspath $<)

# Empty target to force special targets to be built
FORCE:

	@:
