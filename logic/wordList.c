#include <string.h>
#include <stdlib.h>
#include "wordList.h"

// Initialize word list
WordList* wordlist_create(int initial_capacity) {
    WordList* list = malloc(sizeof(WordList));
    list->words = malloc(sizeof(char*) * initial_capacity);
    list->count = 0;
    list->capacity = initial_capacity;
    return list;
}

void wordlist_add(WordList* list, const char* word) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->words = realloc(list->words, sizeof(char*) * list->capacity);
    }
    list->words[list->count] = malloc(strlen(word) + 1);
    strcpy(list->words[list->count], word);
    list->count++;
}

void wordlist_free(WordList* list) {
    for (int i = 0; i < list->count; i++) {
        free(list->words[i]);
    }
    free(list->words);
    free(list);
}