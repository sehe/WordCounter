#include "WordCounter.h"
#include <cassert>
#include <cstring>

namespace {
    struct scan_window {
        scan_window(std::istream& istr) : it(istr), last() {
            consume();
            consume(); // seed buffer
        }

        bool eof() const {
            return it == last && !(_cur || _peek); // nothing buffered
        }

        void consume() {
            _prev = _cur;
            _cur  = _peek;
            _peek = rawget();
            //if (_cur) std::cout << "advanced to " << _cur << "\n"; 
        }
      private:
        char rawget() { return it!=last? *it++ : '\0'; }

        std::istreambuf_iterator<char> it, last;

      public:
        char _prev = '\0', _cur = '\0', _peek = '\0';
    };
}

/* This class accepts a file with at least 1 character in it
 * It keeps track of unique words and the number of times they appear
 */

bool WordCounter::Read(std::istream &istr) {
    // aid in type coercion and overload resolution
    auto isupper = [](uint8_t ch) { return std::isupper(ch); };
    auto isalpha = [](uint8_t ch) { return ch == '\'' || std::isalpha(ch); };
    auto ispunct = [](uint8_t ch) { return ch != '\'' && std::ispunct(ch); };
    auto isdigit = [](uint8_t ch) { return std::isdigit(ch); };

    // tally logic, handles ambiguous case and sentence starts
    auto& histo = this->words;
    auto tally = [=, &histo, new_sentence=true](std::string word) mutable {
        assert(!word.empty()); // otherwise, infinite loop ensues, unhandled character class
        //std::cout << "PROCESS: '" << word << "'\n";

        if ( new_sentence 
                && isupper(word.at(0)) 
                && none_of(word.begin()+1, word.end(), isupper) // Acronym (iPhone, NATO)
                && all_of(word.begin(), word.end(), isalpha)    // mixed (Windows10)
           ) {
            //word[0] = std::tolower(word[0]); // TODO more useful :)
            word.insert(0, "+");
        }

        new_sentence = all_of(word.begin(), word.end(), ispunct)
            && (word.find('!') != std::string::npos ||
                    word.find('?') != std::string::npos ||
                    word.find('.') != std::string::npos);

        //if (new_sentence) std::cout << "new_sentence after '" << word << "'\n";

        histo[word]++;
    };

    scan_window input(istr);

    auto is_atom = [&](bool startofword = false) {
        if (input.eof()) return false;
        return (input._cur == '.' && isdigit(input._peek))
            //|| (input._cur == '\'')
            || (startofword | isdigit(input._prev) && '.' == input._cur && isdigit(input._peek))
            || (              isdigit(input._prev) && ',' == input._cur && isdigit(input._peek))
            || (!isspace(input._cur) && !ispunct(input._cur));
    };

    std::string accum;
    auto append = [&] { accum += input._cur; input.consume(); };

    while (!input.eof()) {
        accum.clear();

        if (std::isspace(input._cur)) {
            input.consume();
            continue;
        }

        if (is_atom(true)) {
            append();
            while (is_atom()) { append(); }
        } else {
            while (ispunct(input._cur)) { append(); }
        }

        tally(std::move(accum));
    }

    // Check that istr hasn't failed but is done with the file
    if (istr.fail() && !istr.eof())
        return false;

    return true;
}

// Writes the output to the terminal
void WordCounter::Write() const {
    for (auto& w : words) {
        std::cout << w.first << " " << w.second << "\n";
    }
}
