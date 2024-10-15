# Compiler and flags
CC := c++
CFLAGS := -Wall -Wextra -Werror -g

# Source files
SRCS := ./parsing/ServerConfig.cpp ./parsing/LocationConfig.cpp ./parsing/parseConfFile.cpp main.cpp

# Object files
OBJS := $(SRCS:.c=.o)

# Target executable
TARGET := myprogram

# Default target
all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and the target executable
clean:
	rm -f $(OBJS) $(TARGET)
