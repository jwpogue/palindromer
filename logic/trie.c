#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "trie.h"

// Create a new trie node
TrieNode* trie_node_create(void) {
    TrieNode* node = malloc(sizeof(TrieNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for trie node\n");
        exit(1);
    }
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    node->is_word_end = false;

    return node;
}

// Create a new trie (returns root node)
TrieNode* trie_create(void) {
    return trie_node_create();
}

// Get index for a character (a-z maps to 0-25)
static int c2i(char c) {
        return c - 'a';
}

// Insert a word into the trie
void trie_insert(TrieNode* root, const char* word) {
    if (!root || !word) return;
    
    TrieNode* current = root;
    int len = strlen(word);
    
    for (int i = 0; i < len; i++) {
        int index = c2i(word[i]);
        
        // Skip invalid characters
        if (index == -1) continue;
        
        // Create new node if it doesn't exist
        if (!current->children[index]) {
            current->children[index] = trie_node_create();
        }
        
        current = current->children[index];
    }
    
    // Mark end of word
    if (!current->is_word_end) {
        current->is_word_end = true;
    }
}

// Search for a word in the trie
bool trie_search(TrieNode* root, const char* word) {
    if (!root || !word) return false;
    
    TrieNode* current = root;
    int len = strlen(word);
    
    for (int i = 0; i < len; i++) {
        int index = c2i(word[i]);
        
        if (index == -1 || !current->children[index]) {
            return false;
        }
        
        current = current->children[index];
    }
    
    return current->is_word_end;
}

// Check if a prefix exists in the trie
bool trie_has_prefix(TrieNode* root, const char* prefix) {
    if (!root || !prefix) return false;
    
    TrieNode* current = root;
    int len = strlen(prefix);
    
    for (int i = 0; i < len; i++) {
        int index = c2i(prefix[i]);
        
        if (index == -1 || !current->children[index]) {
            return false;
        }
        
        current = current->children[index];
    }
    
    return true;
}

// Get the node corresponding to a prefix
static TrieNode* trie_get_prefix_node(TrieNode* root, const char* prefix) {
    if (!root || !prefix) return NULL;
    
    TrieNode* current = root;
    int len = strlen(prefix);
    
    for (int i = 0; i < len; i++) {
        int index = c2i(prefix[i]);
        
        if (index == -1 || !current->children[index]) {
            return NULL;
        }
        
        current = current->children[index];
    }
    
    return current;
}

// Recursive helper for collecting words with a given prefix
static void trie_collect_words_helper(TrieNode* node, char* current_word, 
                                     int depth, const char* prefix, 
                                     WordList* results) {
    if (!node) return;
    
    // If this node marks the end of a word, add it to results
    if (node->is_word_end) {
        current_word[depth] = '\0';
        char* full_word = malloc(strlen(prefix) + strlen(current_word) + 1);
        strcpy(full_word, prefix);
        strcat(full_word, current_word);
        wordlist_add(results, full_word);
        free(full_word);
    }
    
    // Recursively traverse all children
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            current_word[depth] = 'a' + i;
            trie_collect_words_helper(node->children[i], current_word, 
                                    depth + 1, prefix, results);
        }
    }
}

// Get all words that start with a given prefix
void trie_get_words_with_prefix(TrieNode* root, const char* prefix, WordList* results) {
    if (!root || !prefix || !results) return;
    
    // Find the node corresponding to the prefix
    TrieNode* prefix_node = trie_get_prefix_node(root, prefix);
    if (!prefix_node) return;
    
    // Collect all words from this node
    char suffix[1000];  // Buffer for building suffixes
    trie_collect_words_helper(prefix_node, suffix, 0, prefix, results);
}

// Recursive helper for collecting all words in the trie
static void trie_collect_all_words_helper(TrieNode* node, char* current_word, 
                                        int depth, WordList* results) {
    if (!node) return;
    
    // If this node marks the end of a word, add it to results
    if (node->is_word_end) {
        current_word[depth] = '\0';
        wordlist_add(results, current_word);
    }
    
    // Recursively traverse all children
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            current_word[depth] = 'a' + i;
            trie_collect_all_words_helper(node->children[i], current_word, depth + 1, results);
        }
    }
}

// Get all words in the trie
void trie_get_all_words(TrieNode* root, WordList* results) {
    if (!root || !results) return;
    
    char word_buffer[1000];
    trie_collect_all_words_helper(root, word_buffer, 0, results);
}


// Check if trie is empty
bool trie_is_empty(TrieNode* root) {
    if (!root) return true;
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            return false;
        }
    }
    
    return !root->is_word_end;
}


// Print all words in the trie (for debugging)
void trie_print_all_words(TrieNode* root) {
    if (!root) return;
    
    WordList* words = wordlist_create(1000);
    trie_get_all_words(root, words);
    
    printf("Trie contains %d words:\n", words->count);
    for (int i = 0; i < words->count; i++) {
        printf("  %s\n", words->words[i]);
    }
    
    wordlist_free(words);
}

// Destroy the trie and free all memory
void trie_destroy(TrieNode* root) {
    if (!root) return;
    
    // Recursively destroy all children
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            trie_destroy(root->children[i]);
        }
    }
    
    // Free the current node
    free(root);
}
