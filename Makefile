# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -Werror -g
CXXFLAGS = -Wall -Wextra -Werror -g
AR = ar rcs

# Directory paths
SRCDIR = .
OBJDIR = obj
INCDIR = .
LIBDIR = lib

# Library name
LIB = libgbxc.a
SOLIB = libgbxc.so

# Source files using wildcards
SRCS = $(wildcard $(SRCDIR)/errorm/*.c) \
       $(wildcard $(SRCDIR)/func_api/*.c) \
       $(wildcard $(SRCDIR)/gc/*.c)

# Object files
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
SHARED_OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%_pic.o,$(SRCS))

# VPATH to search for source files
VPATH = $(SRCDIR)/errorm:$(SRCDIR)/func_api:$(SRCDIR)/gc

# Main executable
MAIN = main
MAIN_CPP = main_cpp
PYTHON_SCRIPT = main.py

# Targets
all: directories $(LIBDIR)/$(LIB)

# Create necessary directories
directories:
	@mkdir -p $(OBJDIR)/errorm
	@mkdir -p $(OBJDIR)/func_api
	@mkdir -p $(OBJDIR)/gc
	@mkdir -p $(LIBDIR)

# Build the static library
$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $@ $^
	@echo "Library created: $@"

$(LIBDIR)/$(SOLIB): $(SHARED_OBJS)
	$(CC) -shared -o $@ $^
	@echo "Shared library created: $@"

# Compile source files to object files
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(OBJDIR)/%_pic.o: %.c
	$(CC) $(CFLAGS) -fPIC -I$(INCDIR) -c $< -o $@

# Compile and link the main program separately
main: main.c $(LIBDIR)/$(LIB)
	$(CC) $(CFLAGS) -I$(INCDIR) $< -L$(LIBDIR) -lgbxc -o $@

cpp: $(MAIN_CPP)

$(MAIN_CPP): main.cpp $(LIBDIR)/$(LIB)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) $< -L$(LIBDIR) -lgbxc -o $@
	@echo "C++ executable created: $@"
	@echo "Running C++ test..."
	@LD_LIBRARY_PATH=$(LIBDIR) ./$(MAIN_CPP)

# Run Python script with FFI
py3: $(LIBDIR)/$(SOLIB) $(PYTHON_SCRIPT)
	@echo "Running Python FFI test..."
	@chmod +x $(PYTHON_SCRIPT)
	@python3 $(PYTHON_SCRIPT)




# Clean object files
clean:
	rm -rf $(OBJDIR)

# Clean object files and library
fclean: clean
	rm -rf $(LIBDIR)
	rm -f $(MAIN)
	rm -f $(MAIN_CPP)

# Clean and rebuild
re: fclean all

.PHONY: all clean fclean re directories main cpp py3