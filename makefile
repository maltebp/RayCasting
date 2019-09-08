EXE = raycasting.exe
WDIR = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))#$(shell cd)
ODIR = $(WDIR)obj
BDIR = $(WDIR)bin
SDIR = $(WDIR)src

SRC = $(wildcard $(SDIR)/*.cpp)
#https://www.gnu.org/software/make/manual/html_node/Substitution-Refs.html
OBJ = $(SRC:$(SDIR)/%.cpp=$(ODIR)/%.o)

#For debugging
$(info Working Directory = $(WDIR))
$(info Object Directory = $(ODIR))
$(info Source = $(OBJ))
$(info Objects = $(OBJ))

# Signals that all and clean targets are NOT creating any files
.PHONY: all clean

# First target to be executed when running 'make'
all: $(BDIR)/$(EXE)

# EXE depends on all OBJ (all .o files)
# $^ points to all dependencies ($OBJ)
$(BDIR)/$(EXE): $(OBJ)
	g++ $^ -o $@

# $< is the first file in the dependency list (to the right of the :)
$(ODIR)/%.o: $(SDIR)/%.cpp
	g++ -c $< -o $@ -Iinclude

clean:
	$(RM) $(OBJ)


#https://stackoverflow.com/questions/30573481/path-include-and-src-directory-makefile/30602701
#http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/