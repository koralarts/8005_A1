#
# Karl Castillo
# A00750878
#
# Assignments 1
# MAKEFILE
#

# GCC + Flags
GCC = gcc
FLAGS = -W -Wall -pedantic

# Directories
SDIR = ./src
BDIR = ./bin
DDIR = ./debug

# Release:
release: process thread

# Debug:
debug: process-d thread-d

# Process:
process:
	$(GCC) $(FLAGS) -o $(BDIR)/process -D process $(SDIR)/a1.c

# Process Debug:
process-d:
	$(GCC) $(FLAGS) -o $(DDIR)/process -ggdb -D process $(SDIR)/a1.c

# Thread:
thread:
	$(GCC) $(FLAGS) -o $(BDIR)/thread -D thread -pthread $(SDIR)/a1.c

# Thread Debug:
thread-d:
	$(GCC) $(FLAGS) -o $(DDIR)/thread -ggdb -D thread -pthread $(SDIR)/a1.c

# Clean:
clean:
	rm -f $(BDIR)/* $(DDIR)/*

# mkDir
dir:
	mkdir -p $(BDIR) && mkdir -p $(DDIR)
