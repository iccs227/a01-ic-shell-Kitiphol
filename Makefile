# CC=gcc
# CFLAGS=-Wall -g 
# BINARY=icsh

# all: icsh

# icsh: src/icsh.cpp
# 	$(CC) -o $(BINARY) $(CFLAGS) $<

# .PHONY: clean

# clean:
# 	rm -f $(BINARY)




# # Use the C++ compiler
# CXX = g++
# CXXFLAGS = -Wall -g -std=c++17

# # Directory for source files
# SRCDIR = src

# # Name of the final binary
# BINARY = icsh

# # List of source files
# SOURCES = $(SRCDIR)/icsh.cpp $(SRCDIR)/parser.cpp $(SRCDIR)/executor.cpp $(SRCDIR)/history.cpp $(SRCDIR)/jobs.cpp

# # Convert .cpp file names to .o (object files)
# OBJECTS = $(SOURCES:.cpp=.o)

# # Default target to build the binary
# all: $(BINARY)

# # Link object files into the final binary
# $(BINARY): $(OBJECTS)
# 	$(CXX) $(CXXFLAGS) -o $(BINARY) $(OBJECTS)

# # Pattern rule: compile each .cpp file into a .o file
# $(SRCDIR)/%.o: $(SRCDIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Clean up generated object files and binary
# clean:
# 	rm -f $(SRCDIR)/*.o $(BINARY)

# .PHONY: all clean





# Makefile for icsh project
CXX = g++
CXXFLAGS = -Wall -g -std=c++17

BINARY = icsh
SRCDIR = src

SOURCES = $(SRCDIR)/icsh.cpp \
          $(SRCDIR)/parser.cpp \
          $(SRCDIR)/executor.cpp \
          $(SRCDIR)/history.cpp \
          $(SRCDIR)/signals.cpp \
		  $(SRCDIR)/jobs.cpp 

OBJECTS = $(SOURCES:.cpp=.o)

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(BINARY)


