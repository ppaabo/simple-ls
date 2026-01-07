CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I$(INC_DIR) -O2
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TARGET := $(BUILD_DIR)/app

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/*.hpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

all: $(TARGET)

clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
