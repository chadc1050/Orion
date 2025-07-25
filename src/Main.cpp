#include <fstream>
#include <iostream>
#include <vector>

#include "Orion/Reader.hpp"
#include "Orion/Tokenizer.hpp"

using Translation = std::pair<std::string, std::string>;

int main() {

    std::cout << "Parsing Raw Data..." << std::endl;
    const std::vector<Translation> translations = read_file("../data/wmt14_translate_de-en_train.csv");

    std::cout << "Raw Data: " << translations.size() << std::endl;

    std::vector<std::string> en;
    en.reserve(translations.size());

    std::vector<std::string> de;
    de.reserve(translations.size());

    std::vector<std::string> shared;
    shared.reserve(2 * translations.size());

    for (const auto&[english, german] : translations) {
        en.push_back(english);
        de.push_back(german);

        shared.push_back(english);
        shared.push_back(german);
    }

    const BytePairTokenizer tokenizer;

    std::cout << "Tokenizing Data..." << std::endl;
    std::set<std::string> tokens = tokenizer.tokenize(shared, 37000, true);

    return 0;
}