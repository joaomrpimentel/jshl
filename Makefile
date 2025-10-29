# JSHL Compiler Makefile
# Professional build system with organized directory structure

# ============================================================================
# Configuration
# ============================================================================

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lm -lmp3lame -lFLAC

# ============================================================================
# Directories
# ============================================================================

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# ============================================================================
# Files
# ============================================================================

TARGET = $(BIN_DIR)/jshl

SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/core/note_list.c \
          $(SRC_DIR)/core/note_table.c \
          $(SRC_DIR)/parser/parser.c \
          $(SRC_DIR)/audio/synth.c \
          $(SRC_DIR)/audio/wav_writer.c \
          $(SRC_DIR)/audio/raw_writer.c \
          $(SRC_DIR)/audio/mp3_writer.c \
          $(SRC_DIR)/audio/flac_writer.c \
          $(SRC_DIR)/cli/cli.c

OBJECTS = $(BUILD_DIR)/main.o \
          $(BUILD_DIR)/note_list.o \
          $(BUILD_DIR)/note_table.o \
          $(BUILD_DIR)/parser.o \
          $(BUILD_DIR)/synth.o \
          $(BUILD_DIR)/wav_writer.o \
          $(BUILD_DIR)/raw_writer.o \
          $(BUILD_DIR)/mp3_writer.o \
          $(BUILD_DIR)/flac_writer.o \
          $(BUILD_DIR)/cli.o

INCLUDES = -I$(INC_DIR) \
           -I$(INC_DIR)/core \
           -I$(INC_DIR)/parser \
           -I$(INC_DIR)/audio \
           -I$(INC_DIR)/cli

# ============================================================================
# Build Rules
# ============================================================================

.PHONY: all clean rebuild install dirs help

# Default target
all: dirs $(TARGET)
	@echo "✓ Build complete: $(TARGET)"

# Create necessary directories
dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Link final executable
$(TARGET): $(OBJECTS)
	@echo "Linking $@..."
	@$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile main.c
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile core modules
$(BUILD_DIR)/note_list.o: $(SRC_DIR)/core/note_list.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/note_table.o: $(SRC_DIR)/core/note_table.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile parser module
$(BUILD_DIR)/parser.o: $(SRC_DIR)/parser/parser.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile audio modules
$(BUILD_DIR)/synth.o: $(SRC_DIR)/audio/synth.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/wav_writer.o: $(SRC_DIR)/audio/wav_writer.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/raw_writer.o: $(SRC_DIR)/audio/raw_writer.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/mp3_writer.o: $(SRC_DIR)/audio/mp3_writer.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/flac_writer.o: $(SRC_DIR)/audio/flac_writer.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compile CLI module
$(BUILD_DIR)/cli.o: $(SRC_DIR)/cli/cli.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ============================================================================
# Utility Targets
# ============================================================================

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)/*.o $(TARGET)
	@echo "✓ Clean complete"

# Full rebuild
rebuild: clean all

# Install to system (requires sudo)
install: $(TARGET)
	@echo "Installing to /usr/local/bin..."
	@install -m 0755 $(TARGET) /usr/local/bin/jshl
	@echo "✓ Installation complete"

# Show available targets
help:
	@echo "JSHL Compiler - Available targets:"
	@echo "  make all      - Build the compiler (default)"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make rebuild  - Clean and build"
	@echo "  make install  - Install to /usr/local/bin"
	@echo "  make help     - Show this help message"