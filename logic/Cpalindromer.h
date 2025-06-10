#ifndef CPALINDROMER_H
#define CPALINDROMER_H

#include <stdbool.h>
#include "wordList.h"
#include "trie.h"

#define MAX_PALINDROME_LEN 6
#define MAX_WORD_LEN 100
#define MAX_LINE_LEN 1000
#define MAX_RESULTS 100000
#define STACK_SIZE MAX_PALINDROME_LEN * 10000

// Search state for iterative backtracking
typedef struct {
    char* settled;      // accumulated left side
    char* overhang;       // current unmatched portion
    int depth;
    bool is_forwards;     // direction of current search
} SearchState;

// Core palindrome finder functions
bool palindrome_init(void);
void palindrome_cleanup(void);
bool palindrome_load_dictionary(const char* filename);
bool palindrome_set_output_file(const char* filename);
void palindrome_set_min_word_length(int min_len);
int palindrome_find_all(const char* starting_word);

// Utility functions
char* string_reverse(const char* str);
bool is_palindrome(const char* str);
void get_prefixes(const char* word, WordList* result);
void generate_candidates(const char* prefix, bool is_forwards, WordList* candidates);

// Internal search functions
bool find_palindromes(const char* starting_prefix);
void output_palindrome(const char* settled, const char* middle, bool was_forward);

#endif // CPALINDROMER_H