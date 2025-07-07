#include <fstream>
#include <iostream>
#include <vector>

#include "Orion/Reader.hpp"
#include "Orion/Tokenizer.hpp"

using Translation = std::pair<std::string, std::string>;

int main() {

    std::cout << "Parsing Raw Data..." << std::endl;
    const std::vector<Translation> translations = read_file("../data/wmt14_translate_de-en_test.csv");

    std::cout << "Raw Data: " << translations.size() << std::endl;

    std::vector<std::string> en;
    en.reserve(translations.size());

    std::vector<std::string> de;
    de.reserve(translations.size());

    for (const auto&[english, german] : translations) {
        en.push_back(english);
        de.push_back(german);
    }

    const BytePairTokenizer tokenizer;

    std::cout << "Tokenizing English Data..." << std::endl;
    std::set<std::string> en_tokens = tokenizer.tokenize(en, 1000, true);

    std::cout << "Tokenizing German Data..." << std::endl;
    std::set<std::string> de_tokens = tokenizer.tokenize(de, 1000, false);

    return 0;
}