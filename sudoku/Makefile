# Compiler to use
CC = g++

# Compiler flags
CFLAGS = -Wall -g

# Target executable name
TARGET = sudoku

# Source files
SOURCES = main.cpp sudoku.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files
HEADERS = sudoku.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: all clean
