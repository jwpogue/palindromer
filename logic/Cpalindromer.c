#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "Cpalindromer.h"

// Global variables (static for encapsulation)
static TrieNode* dictionary_root = NULL;
static TrieNode* reverse_dictionary_root = NULL;
static int MIN_WORD_LEN = 6;
static FILE* output_file = NULL;
static int palindrome_count = 0;

// Initialize the palindrome finder
bool palindrome_init(void) {
    dictionary_root = trie_create();
    reverse_dictionary_root = trie_create();
    palindrome_count = 0;
    
    if (!dictionary_root || !reverse_dictionary_root) {
        palindrome_cleanup();
        return false;
    }
    
    return true;
}

void cleanUp(char* str) {
    if (!str) return;
    
    int len = strlen(str);
    int half = len / 2;
    int writePos = 0;
    
    for (int pos = 0; pos < len; pos++) {
        char ch = str[pos];
        
        if (pos < half) {
            // First half: '1' -> space, '2' -> skip, others -> keep
            if (ch == '1') {
                str[writePos++] = ' ';
            } else if (ch != '2') {
                str[writePos++] = ch;
            }
        } else {
            // Second half: '1' -> skip, '2' -> space, others -> keep
            if (ch == '2') {
                str[writePos++] = ' ';
            } else if (ch != '1') {
                str[writePos++] = ch;
            }
        }
    }
    
    str[writePos] = '\0';  // Null terminate at new length
}


// Cleanup resources
void palindrome_cleanup(void) {
    if (dictionary_root) {
        trie_destroy(dictionary_root);
        dictionary_root = NULL;
    }
    if (reverse_dictionary_root) {
        trie_destroy(reverse_dictionary_root);
        reverse_dictionary_root = NULL;
    }
    if (output_file) {
        fclose(output_file);
        output_file = NULL;
    }
}

// Set minimum word length
void palindrome_set_min_word_length(int min_len) {
    MIN_WORD_LEN = min_len;
}

// Set output file
bool palindrome_set_output_file(const char* filename) {
    if (output_file) {
        fclose(output_file);
    }
    
    output_file = fopen(filename, "w");
    return output_file != NULL;
}

// Utility function: reverse a string
char* string_reverse(const char* str) {
    int len = strlen(str);
    char* reversed = malloc(len + 1);
    if (!reversed) return NULL;
    
    for (int i = 0; i < len; i++) {
        reversed[i] = str[len - 1 - i];
    }
    reversed[len] = '\0';
    return reversed;
}

// Check if a string is a palindrome
bool is_palindrome(const char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return false;
        }
    }
    return true;
}

// Get all prefixes of a word that are also valid words
void get_prefixes(const char* word, WordList* result) {
    int len = strlen(word);
    char prefix[MAX_WORD_LEN];
    
    for (int i = 1; i < len; i++) {
        strncpy(prefix, word, i);
        prefix[i] = '\0';
        wordlist_add(result, prefix);
    }
}

// Generate candidate words for given prefix and direction
void generate_candidates(const char* prefix, bool is_forwards, WordList* candidates) {
    candidates->count = 0;  // Reset list
    
    if (is_forwards) {
        // Get words starting with prefix from normal dictionary
        trie_get_words_with_prefix(dictionary_root, prefix, candidates);
        
        // Also add prefixes of current prefix that are valid reverse words
        WordList* prefixes = wordlist_create(strlen(prefix));
        get_prefixes(prefix, prefixes);
        
        for (int i = 0; i < prefixes->count; i++) {
            // char* reversed = string_reverse(prefixes->words[i]);
            if (trie_search(dictionary_root, prefixes->words[i])) {
                wordlist_add(candidates, prefixes->words[i]);
            }
        }
        wordlist_free(prefixes);

    } else {

        trie_get_words_with_prefix(reverse_dictionary_root, prefix, candidates);
        WordList* prefixes = wordlist_create(strlen(prefix));
        get_prefixes(prefix, prefixes);
        for (int i = 0; i < prefixes->count; i++) {
            if (trie_search(reverse_dictionary_root, prefixes->words[i])) {
                wordlist_add(candidates, prefixes->words[i]);
            }
        }
        wordlist_free(prefixes);
    }
}

void generate_starts(const char* starting_prefix, WordList* candidates) {
    candidates->count = 0;
    
    // Get words that directly match the prefix
    trie_get_words_with_prefix(dictionary_root, starting_prefix, candidates);
    
    // Pre-calculate starting_prefix length to avoid repeated strlen calls
    const size_t prefix_len = strlen(starting_prefix);
    
    // Get all prefixes once
    WordList* prefixes = wordlist_create(prefix_len);
    get_prefixes(starting_prefix, prefixes);
    
    // Pre-allocate buffer for combined words outside the loop
    char combined[MAX_WORD_LEN * 2];
    
    // Create completion_words once and reuse it
    WordList* completion_words = wordlist_create(1000);
    
    for (int i = 0; i < prefixes->count; i++) {
        const char* prefix = prefixes->words[i];
        if (trie_search(dictionary_root, prefix)) {
            const size_t curr_prefix_len = strlen(prefix);
            
            
            // Calculate remaining string pointer once
            const char* remaining = starting_prefix + curr_prefix_len;
            
            // Clear the reused WordList instead of creating new one
            completion_words->count = 0;
            trie_get_words_with_prefix(dictionary_root, remaining, completion_words);
            
            // Batch process combinations
            for (int j = 0; j < completion_words->count; j++) {
                const char* completion = completion_words->words[j];
                
                // Use memcpy for potentially better performance than snprintf
                memcpy(combined, prefix, curr_prefix_len);
                strcpy(combined + curr_prefix_len, completion);
                
                wordlist_add(candidates, combined);
            }
        }
    }
    
    // Clean up
    wordlist_free(completion_words);
    wordlist_free(prefixes);
}

// Output a found palindrome
void output_palindrome(const char* settled, const char* middle, bool was_forwards) {
    // Create the full palindrome


    char palindrome[MAX_WORD_LEN * MAX_PALINDROME_LEN];

    char* reversed = string_reverse(settled);

    if (was_forwards) {
        snprintf(palindrome, sizeof(palindrome), "%s %s%s", settled, middle, reversed);
    } else {
        snprintf(palindrome, sizeof(palindrome), "%s%s %s", settled, middle, reversed);
    }

    cleanUp(palindrome);
    
    // Write to file if output file is set
    if (output_file) {
        fprintf(output_file, "%s\n", palindrome);
        fflush(output_file);
    }
    
    palindrome_count++;
    printf("Found: %s\n", palindrome);
}

// Main palindrome search - iterative with explicit stack
bool find_palindromes(const char* starting_prefix) {
    // Stack for iterative backtracking
    SearchState stack[STACK_SIZE];
    int stack_top = 0;
    
    // Initialize with starting state
    WordList* initial_candidates = wordlist_create(1000);

    if (starting_prefix[0] == '\0') {
        // If no prefix, start from *all words* in the dictionary
        trie_get_all_words(dictionary_root, initial_candidates);

    } else {
        generate_starts(starting_prefix, initial_candidates);
    }
    
    
    if (initial_candidates->count == 0) {
        printf("No words found starting with '%s'\n", starting_prefix);
        wordlist_free(initial_candidates);
        return false;
    }
    
    // Push initial states for each candidate word
    for (int i = 0; i < initial_candidates->count; i++) {
        if ((size_t) stack_top >= STACK_SIZE) break;
        
        SearchState* state = &stack[stack_top++];
        state->settled = malloc(MAX_WORD_LEN * MAX_PALINDROME_LEN);
        state->overhang = malloc(MAX_WORD_LEN * MAX_PALINDROME_LEN);
        
        if (!state->settled || !state->overhang) {
            // Handle allocation failure
            free(state->settled);
            free(state->overhang);
            stack_top--;
            continue;
        }
        
        strcpy(state->settled, "");
        strcpy(state->overhang, initial_candidates->words[i]);
        state->depth = 0;
        state->is_forwards = false;
    }
    
    wordlist_free(initial_candidates);
    
    // Main search loop
    while (stack_top > 0 && stack_top <= STACK_SIZE) {
        SearchState current = stack[--stack_top];
        
        // Check depth limit
        if (current.depth > MAX_PALINDROME_LEN) {
            free(current.settled);
            free(current.overhang);
            continue;
        }
        
        // Check if overhang is a palindrome
        if (is_palindrome(current.overhang) && current.depth > 0 && current.overhang) {
            // Found a palindrome!
            output_palindrome(current.settled, current.overhang, current.is_forwards);
            // if (strlen(current.overhang) > 0) {
                free(current.settled);
                free(current.overhang); 
                continue;
            // }
        }
        
        // Generate next candidates
        WordList* candidates = wordlist_create(1000);
        generate_candidates(current.overhang, current.is_forwards, candidates);
        
        // Try each candidate
        for (int i = 0; i < candidates->count && (size_t) stack_top < STACK_SIZE; i++) {

            SearchState* new_state = &stack[stack_top++];
            new_state->settled = malloc(MAX_WORD_LEN * MAX_PALINDROME_LEN);
            new_state->overhang = malloc(MAX_WORD_LEN);
            if (!new_state->settled || !new_state->overhang) {
                // Handle allocation failure
                free(new_state->settled);
                free(new_state->overhang);
                stack_top--;
                continue;
            }

            const char* word = candidates->words[i];

            const char* shorterPrefix;
            const char* longerPrefix;

            // Determine which is shorter/longer
            if (strlen(word) >= strlen(current.overhang)) {
                shorterPrefix = current.overhang;
                longerPrefix = word;
                new_state->is_forwards = !current.is_forwards;
            } else {
                shorterPrefix = word;
                longerPrefix = current.overhang;
                new_state->is_forwards = current.is_forwards;
            }
            strcpy(new_state->settled, current.settled);      // Copy previous state

            char marker[2];
            if (current.is_forwards) {
                marker[0] = '1';
            } else {
                marker[0] = '2'; 
            }
            marker[1] = '\0';
            strcat(new_state->settled, marker);

            strcat(new_state->settled, shorterPrefix);
            strcpy(new_state->overhang, longerPrefix + strlen(shorterPrefix));
            new_state->depth = current.depth + 1;
            }
        
        wordlist_free(candidates);
        free(current.settled);
        free(current.overhang);
    }

    return palindrome_count > 0;
}

// Load dictionary from file
bool palindrome_load_dictionary(const char* filename) {
    if (!dictionary_root || !reverse_dictionary_root) {
        return false;
    }
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open dictionary file %s\n", filename);
        return false;
    }
    
    char line[MAX_LINE_LEN];
    int words_loaded = 0;
    
    while (fgets(line, sizeof(line), file)) {
        // Remove newline and convert to lowercase
        line[strcspn(line, "\n\r")] = 0;
        
        for (int i = 0; line[i]; i++) {
            line[i] = tolower(line[i]);
        }
        
        if (strlen(line) >= (size_t) MIN_WORD_LEN) {
            trie_insert(dictionary_root, line);
            
            char* reversed = string_reverse(line);
            if (reversed) {
                trie_insert(reverse_dictionary_root, reversed);
                free(reversed);
            }
            
            words_loaded++;
        }
    }
    
    fclose(file);
    printf("Loaded %d words\n", words_loaded);
    return true;
}

// Main entry point for finding palindromes
int palindrome_find_all(const char* starting_word) {
    if (!dictionary_root || !reverse_dictionary_root) {
        return 0;
    }
    
    // Reset counter
    palindrome_count = 0;
    
    // Add starting word to dictionaries
    if (strlen(starting_word) >= (size_t) MIN_WORD_LEN) {
        trie_insert(dictionary_root, starting_word);
        char* reversed_attempt = string_reverse(starting_word);
        if (reversed_attempt) {
            trie_insert(reverse_dictionary_root, reversed_attempt);
            free(reversed_attempt);
        }
    }
        
    // Find palindromes
    find_palindromes(starting_word);
    
    return palindrome_count;
}
