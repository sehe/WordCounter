#ifndef WORD_COUNTER_H
#define WORD_COUNTER_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/* This is a program which will count the number of unique words in a file
 * words can be any series of characters, where a word is:
 * a std::string of chars delimited on both sides by whitespace These are read into
 * a vector of strings that have counts associated with them indicating how
 * many exact duplicates were read into the file
 *
 * The file must have at least one word
 *
 * The words appear in lexicographic order.
 *
 * For example, if the input file contained "This is a test, a test it is!"
 * (quotes not included), then the output should be:
 *
 *     This 1
 *     a 2
 *     is 1
 *     is! 1
 *     it 1
 *     test 1
 *     test, 1
 */

class WordCounter {

  public:
    WordCounter() : words() {}

    // Accessors
    void Write() const;

    // Mutators
    bool Read(std::istream &istr);

  private:
    struct entry {
        std::string word;
        size_t frequency = 0;

        entry(std::string w, size_t f = 0) : word(std::move(w)), frequency(f) {}
    };

    struct unique_words {

        auto begin() const { return _container.begin(); }
        auto end() const   { return _container.end(); }

        entry& ensure(std::string const& key);

      private:
        auto begin()       { return _container.begin(); }
        auto end()         { return _container.end(); }
        // invariant: sorted
        struct by_name;
        std::vector<entry> _container;
    } words;
};
#endif
