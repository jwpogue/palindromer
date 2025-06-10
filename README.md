# palindromer
generate sentential palindromes with the power of C

Hello! This project is version two of a project I made in high school for a college app. It intends to make strings of words that form perfect palindromes. It doesn't yet follow english grammar, but this is a feature I intend to implement.

In order to use, use _make clean_, _make_, _./palindrome_ 
Set your minimum word length, your starting prefix, or use '0' to start with no prefix. Enjoy!
Future ideas:

Make generate_start allow any length of starts 
1. make a function which generates all possible splits of a string, and return just the list of final strings

Make the generator stop only when it needs to:
    1. this basically entails making it cached in order to make it interesting

make the generator more sentence like
1. use n-grams
    1. in generate options, remove options that don't appear in the ngram list above a certain frequency

2. use sentence structure
    1. have a seperate trie for each p.o.s
    2. change generate_candidates to use depth to generate only from the correct trie.
