# Compiler setup
CC := gcc
CFLAGS := -std=c99 -Wall -Wextra
LDFLAGS :=
LDLIBS := -lcunit

# Platform detection
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_S),Darwin)        # MacOS
    ifeq ($(UNAME_M),x86_64)
        CFLAGS += -I/usr/local/include
        LDFLAGS += -L/usr/local/lib
    else ifeq ($(UNAME_M),arm64)
        CFLAGS += -I/opt/homebrew/include
        LDFLAGS += -L/opt/homebrew/lib
    endif
endif
ifeq ($(UNAME_S),Linux)         # Linux
    CFLAGS += -I/usr/include
    LDFLAGS += -L/usr/lib
endif
ifneq ($(filter MSYS%,$(UNAME_S)),)     # MSYS/MinGW (Windows)
    CFLAGS += -IC:/msys64/mingw64/include
    LDFLAGS += -LC:/msys64/mingw64/lib
endif

# Directories
SRCDIR := src
INCDIR := include
BUILDDIR := build
BINDIR := bin
TESTDIR := tests
TARGET := $(BINDIR)/lars
TESTTARGET := $(BINDIR)/test

# Source files
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

# Test files
TEST_SRCS := $(wildcard $(TESTDIR)/*.c)
TEST_OBJS := $(patsubst $(TESTDIR)/%.c,$(BUILDDIR)/%.o,$(TEST_SRCS))

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compilation
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# Create directories if they don't exist
$(shell mkdir -p $(BUILDDIR))
$(shell mkdir -p $(BINDIR))

# Cleanup
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

# Test target
test: $(TESTTARGET)
	./$(TESTTARGET)

$(TESTTARGET): $(filter-out build/main.o, $(OBJS)) $(TEST_OBJS)
	$(CC) -DTEST $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

# Generate compile_commands.json using bear
bear: clean all
	bear -- make

.PHONY: all clean test
