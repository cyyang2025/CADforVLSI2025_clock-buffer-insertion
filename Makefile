# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Wno-array-bounds -Wno-stringop-overflow

# Executable name
TARGET = cbi

# Source files
SRCS = main.cpp Parser.cpp Utils.cpp Overlap.cpp DME.cpp Output.cpp Globals.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Linking rule
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compilation rule for .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
