CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -g -Iinclude -Iutils
LDFLAGS  := 
BUILD    := ./obj
BIN      := ./bin
SRC      := ./src
INCLUDE  := ./include
UTILS    := ./utils

SRCS := $(wildcard $(SRC)/*.cpp) $(wildcard $(UTILS)/*.cpp)
OBJS := $(patsubst %.cpp, $(BUILD)/%.o, $(notdir $(SRCS)))
TARGET := $(BIN)/output


.PHONY: all clean directories run

all: directories $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Build completata: $(TARGET)"

$(BUILD)/%.o: $(SRC)/%.cpp
	@echo "Compiling SRC $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/%.o: $(UTILS)/%.cpp
	@echo "Compiling UTILS $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

directories:
	@mkdir -p $(BIN)
	@mkdir -p $(BUILD)

clean:
	@rm -rf $(BUILD) $(BIN)

run: all
	@echo "Esecuzione con argomenti: $(ARGS)"
	@$(TARGET) $(ARGS)
