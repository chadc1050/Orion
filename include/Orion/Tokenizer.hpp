#pragma once

#include <set>
#include <cctype>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class Tokenizer {
public:
    Tokenizer() = default;

    /// Tokenizes raw data.
    /// @param raw Raw sentences.
    /// @param lower Normalize to lower case.
    /// @return Tokens
    [[nodiscard]] virtual std::vector<std::string> tokenize(const std::vector<std::string>& raw, bool lower) const = 0;

    virtual ~Tokenizer() = default;
};

inline std::vector<std::vector<std::string>> normalize(const std::vector<std::string>& raw, const bool lower) {

    std::vector<std::vector<std::string>> res;
    res.reserve(raw.size());

    for (const std::string& sentence : raw) {
        std::vector<std::string> normalized;

        std::stringstream ss(sentence);
        std::string word;
        while (!ss.eof()) {
            getline(ss, word, ' ');

            std::string curr;

            for (char& c : word) {
                if (std::isalpha(c)) {
                    curr += lower ? std::tolower(c) : c;
                } else if (word.length() > 1) {
                    normalized.emplace_back(curr);
                    normalized.emplace_back(1, c);
                    curr = "";
                }
            }

            if (!curr.empty()) {
                normalized.emplace_back(curr);
            }
        }

        res.emplace_back(normalized);
    }

    return res;
}

inline std::set<std::string> get_vocab(const std::vector<std::vector<std::string>>& data) {
    std::set<std::string> vocab;

    for (const auto& sentence : data) {
        for (const auto& word : sentence) {
            vocab.insert(word);
        }
    }

    return vocab;
}

class WordPieceTokenizer final : public Tokenizer {
public:

    /// Tokenizes raw data via Word Piece algorithm.
    /// @param raw Raw sentences.
    /// @param lower Normalize to lower case.
    /// @return Tokens
    [[nodiscard]] std::vector<std::string> tokenize(const std::vector<std::string>& raw, const bool lower) const override {

        const std::vector<std::vector<std::string>> normalized = normalize(raw, lower);

        const std::set<std::string> vocab = get_vocab(normalized);

        std::set<std::string> tokens;
        std::set<std::string> special;

        std::unordered_map<std::string, unsigned int> freqs = {};

        for (const std::vector<std::string>& sentence : normalized) {
            for (const std::string& word : sentence) {
                for (int l = 0; l < word.size(); l++) {
                    // Special characters just get added
                    if (!std::isalpha(word[l])) {
                        special.insert(std::string(1, word[l]));
                        continue;
                    }

                    // Check alpha characters
                    if (l > 0 && word[l - 1] == ' ') {
                        tokens.insert(std::string(1, word[l]));
                        if (freqs.find(std::string(1, word[l])) == freqs.end()) {
                            freqs[std::string(1, word[l])] = 1;
                        } else {
                            freqs[std::string(1, word[l])] = freqs[std::string(1, word[l])] + 1;
                        }
                    } else {
                        tokens.insert(WORD_PIECE_DELIMITER + sentence[l]);
                    }
                }
            }
        }

        std::vector<std::string> res;

        return res;
    }
private:
    const std::string WORD_PIECE_DELIMITER = "##";
};

class BytePairTokenizer final : public Tokenizer {
public:
    /// Tokenizes raw data via Byte Pair algorithm.
    /// @param raw Raw sentences.
    /// @param lower Normalize to lower case.
    /// @return Tokens
    [[nodiscard]] std::vector<std::string> tokenize(const std::vector<std::string>& raw, const bool lower) const override {
        const std::vector<std::vector<std::string>> normalized = normalize(raw, lower);

        std::set<std::string> corpus = get_vocab(normalized);

        std::set<std::string> tokens;


    }
}


