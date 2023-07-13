# Compiler setup
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra

# Directories
SRCDIR := src
INCDIR := include
BUILDDIR := build
BINDIR := bin
TARGET := $(BINDIR)/lars

# Source files
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compilation
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Create directories if they don't exist
$(shell mkdir -p $(BUILDDIR))
$(shell mkdir -p $(BINDIR))

# Cleanup
clean:
	rm -rf $(BUILDDIR) $(BINDIR)
