#ifndef WORDLIST_H
#define WORDLIST_H

typedef struct {
    char **words;
    int    count;
    int    capacity;
} WordList;

WordList *wordlist_create(int initial_capacity);
void       wordlist_add  (WordList *list, const char *word);
void       wordlist_free (WordList *list);

#endif /* WORDLIST_H */
