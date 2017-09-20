/*! \file main.cpp: main function for a word counter */
#include "WordCounter.h"
#include <fstream>
#include <iostream>
#include <string>

/// Print the correct usage in case of user syntax error.
int Usage(char *arg0) {
    std::cout << "Usage: " << arg0 << " filename" << std::endl;
    std::cout << "where file contains at least 1" << std::endl;
    std::cout << "non-whitespace character" << std::endl;
    return -1;
}

int main(int argc, char *argv[]) {
    // check for correct number of args
    if (argc != 2)
        return Usage(argv[0]);

    // create an input stream and check it doesn't fail (empty file)
    std::ifstream istr(argv[1]);
    if (istr.fail())
        return Usage(argv[0]);

    // Create an IntCounter class
    WordCounter wc1;
    if (!wc1.Read(istr))
        return Usage(argv[0]);

    // write output to terminal
    wc1.Sort();
    wc1.Write();
}
