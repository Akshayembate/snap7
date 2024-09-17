# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++14

# Include paths
INCLUDES =

# Source files
SRCS = main.cpp snap7.cpp s7.cpp 

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = executable.exe

# Library paths
LIBS = -lsnap7

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Compile source files into object files
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean the project
clean:
	del /Q $(OBJS) $(TARGET)
