CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g

TARGET = palindrome
SRCDIR = logic
SOURCES = main.c $(SRCDIR)/Cpalindromer.c $(SRCDIR)/trie.c $(SRCDIR)/wordList.c
OBJECTS = $(SOURCES:.c=.o)

# Build rule
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Rule for compiling .o files from logic/*.c
logic/%.o: logic/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean
