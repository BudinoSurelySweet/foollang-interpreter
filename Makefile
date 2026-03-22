CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -g -Iinclude
LDFLAGS  := 
BUILD    := ./obj
BIN      := ./bin
SRC      := ./src
INCLUDE  := ./include
UTILS    := ./utils

# --- Color configuration ---
# Available colors: RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN
COLOR ?= GREEN

ANSI_RED     := \033[0;31m
ANSI_GREEN   := \033[0;32m
ANSI_YELLOW  := \033[0;33m
ANSI_BLUE    := \033[0;34m
ANSI_MAGENTA := \033[0;35m
ANSI_CYAN    := \033[0;36m
ANSI_RESET   := \033[0m

C := $(ANSI_$(COLOR))
R := $(ANSI_RESET)
# -----------------------------

SRCS := $(shell find $(SRC) -name "*.cpp")
OBJS := $(patsubst %.cpp, $(BUILD)/%.o, $(notdir $(SRCS)))
TARGET := $(BIN)/output

# Use vpath to find source files in subdirectories
VPATH := $(SRC):$(shell find $(SRC) -type d | tr '\n' ':')


.PHONY: all clean directories run

all: directories $(TARGET)

$(TARGET): $(OBJS)
	@printf "$(C)Linking...$(R)\n"
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@printf "$(C)Build completed: $(TARGET)$(R)\n"

$(BUILD)/%.o: %.cpp
	@printf "$(C)Compiling SRC $<...$(R)\n"
	$(CXX) $(CXXFLAGS) -c $< -o $@

directories:
	@mkdir -p $(BIN)
	@mkdir -p $(BUILD)

clean:
	@rm -rf $(BUILD) $(BIN)

run: all
	@printf "$(C)Execution with arguments: $(ARGS)$(R)\n"
	@$(TARGET) $(ARGS)
