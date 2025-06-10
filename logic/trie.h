#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include "wordList.h"


#define ALPHABET_SIZE 26

// Trie node structure
typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool is_word_end;
} TrieNode;

// Statistics structure for trie analysis
typedef struct {
    int total_nodes;
    int total_words;
    int max_depth;
} TrieStats;

// Core trie operations
TrieNode* trie_create(void);
void trie_insert(TrieNode* root, const char* word);
bool trie_search(TrieNode* root, const char* word);
bool trie_remove(TrieNode* root, const char* word);
void trie_destroy(TrieNode* root);

// Prefix operations
bool trie_has_prefix(TrieNode* root, const char* prefix);
void trie_get_words_with_prefix(TrieNode* root, const char* prefix, WordList* results);

// Utility operations
void trie_get_all_words(TrieNode* root, WordList* results);
bool trie_is_empty(TrieNode* root);

// Helper functions for WordList operations (should be implemented in main program)
WordList* wordlist_create(int initial_capacity);
void wordlist_add(WordList* list, const char* word);
void wordlist_free(WordList* list);

#endif