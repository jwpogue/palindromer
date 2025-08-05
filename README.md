# Palindromer

**Palindromer** is a C program that finds multi-word palindromes using a dictionary of real words. It works by building palindromic phrases—like “step on no pets” or “evil live”—out of valid English words, using trie-based prefix matching and an iterative backtracking algorithm.

---

## Features

- **Multi-word palindrome detection** using real dictionary words
- **Trie-based prefix search** for fast candidate lookups (both forward and reverse)
- **Custom backtracking engine** with a configurable depth limit
- **Flexible dictionary support** with customizable minimum word length
- **Optional output file** for saving results

---

## How It Works

At a high level, the program constructs palindromes by recursively building out two sides—`left_part` and `right_part`—and maintaining an "overhang" of unmatched letters. It tries adding words to either side until the full expression resolves to a palindrome.

A successful match looks like: left_part + overhang + reversed(right_part)

The program checks that this final combined string is a true palindrome (ignoring spaces) before outputting it.

---

## Building

To compile, you'll need a C compiler like `gcc` and the following source files:

- `Cpalindromer.c` – main logic
- `trie.c` / `trie.h` – prefix tree implementation
- `wordList.c` / `wordList.h` – dynamic list of candidate words

Compile with:

```bash
gcc -o palindromer Cpalindromer.c trie.c wordList.c -Wall -O2
```

## Notes
I've also included the original version of this I wrote years ago in Python. The newer version is approximately 4000% faster, which is, an improvement to say the least. Here is my favorite one I've found:

_senile filler gnomes reverse mongrel lifelines_
