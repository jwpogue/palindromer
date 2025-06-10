#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "logic/trie.h"
#include "logic/wordList.h"
#include "logic/Cpalindromer.h"

int main() {
    // Initialize palindrome finder
    if (!palindrome_init()) {
        printf("Error: Failed to initialize palindrome finder\n");
        return 1;
    }
    
    // Get minimum word length
    int min_word_len;
    printf("min length: ");
    scanf("%d", &min_word_len);
    palindrome_set_min_word_length(min_word_len);
    
    // Load dictionary
    printf("Loading dictionary...\n");
    if (!palindrome_load_dictionary("lexicons/cel.txt")) {
        printf("Error: Failed to load dictionary\n");
        palindrome_cleanup();
        return 1;
    }
    
    // Open output file
    if (!palindrome_set_output_file("output.txt")) {
        printf("Error: Cannot create output file\n");
        palindrome_cleanup();
        return 1;
    }
    
    // Get starting word
    char attempt[MAX_WORD_LEN];
    printf("start: ");
    scanf("%s", attempt);
    
    // Convert to lowercase
    for (int i = 0; attempt[i]; i++) {
        attempt[i] = tolower(attempt[i]);
    }
    if (attempt[0] == '0') {
        attempt[0] = '\0';
    }
    // Find palindromes
    printf("Searching for palindromes starting with '%s'...\n", attempt);
    int count = palindrome_find_all(attempt);
    
    printf("\nFound %d palindromes total\n", count);
    
    // Cleanup
    palindrome_cleanup();
    
    return 0;
}