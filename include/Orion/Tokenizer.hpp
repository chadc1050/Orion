#pragma once

#include <cctype>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Dast/collections/LinkedList.hpp"

class Tokenizer {
public:
    Tokenizer() = default;

    /// Tokenizes raw data.
    /// @param raw Raw sentences.
    /// @param lower Normalize to lower case.
    /// @return Tokens
    [[nodiscard]] virtual std::set<std::string> tokenize(const std::vector<std::string>& raw, unsigned int vocab, bool lower) const = 0;

    virtual ~Tokenizer() = default;
};

inline std::vector<std::vector<Dast::LinkedList<int>>> normalize(const std::vector<std::string>& raw, const bool lower) {

    // Sentence -> word -> letters (as linked string chars)
    std::vector<std::vector<Dast::LinkedList<int>>> res;
    res.reserve(raw.size());

    for (const std::string& sentence : raw) {
        std::vector<Dast::LinkedList<int>> normalized;

        std::stringstream ss(sentence);
        std::string word;
        while (getline(ss, word, ' ')) {

            Dast::LinkedList<int> curr;

            for (const char& c : word) {
                if (std::isalpha(c)) {
                    curr.push_back(lower ? std::tolower(c) : static_cast<int>(c));
                } else if (word.length() > 1) {
                    if (!curr.empty()) {
                        normalized.emplace_back(std::move(curr));
                        curr.clear();
                    }
                    normalized.emplace_back(Dast::LinkedList{static_cast<int>(c)});
                }
            }

            if (!curr.empty()) {
                normalized.emplace_back(std::move(curr));
            }
        }

        res.emplace_back(normalized);
    }

    return res;
}

struct pair_hash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second) << 1;
    }
};

class BytePairTokenizer final : public Tokenizer {
public:
    /// Tokenizes raw data via Byte Pair algorithm.
    /// @param raw Raw sentences.
    /// @param n_vocab Number of tokens
    /// @param lower Normalize to lower case.
    /// @return Tokens
    [[nodiscard]] std::set<std::string> tokenize(const std::vector<std::string>& raw, const unsigned int n_vocab, const bool lower) const override {

        std::vector<std::vector<Dast::LinkedList<int>>> normalized = normalize(raw, lower);

        std::cout << "Normalized data" << std::endl;

        // Use ordered map for better decoding
        std::map<int, std::pair<int, std::optional<int>>> defs = {};

        // Add characters to vocab
        int max = 0;
        for (const auto& sentence : normalized) {
            for (const auto& word : sentence) {
                for (const auto& letter : word) {
                    max = std::max(max, letter);
                    defs[letter] = std::make_pair(letter, std::optional<int>{});
                }
            }
        }

        while (defs.size() < n_vocab) {

            std::unordered_map<std::pair<int, int>, int, pair_hash> freqs = {};

            // Get the frequencies of all pairings of characters
            for (std::vector<Dast::LinkedList<int>>& sentence: normalized) {
                for (Dast::LinkedList<int>& word : sentence) {
                    Dast::Node<int>* node = word.get_ptr();
                    while (node->next != nullptr) {
                        freqs[std::pair(node->data, node->next->data)]++;
                        node = node->next;
                    }
                }
            }

            // If we do not get a frequency distribution it would indicate merging is complete.
            if (freqs.empty()) {
                break;
            }

            // Add mode to defs
            int mode = 0;
            std::pair<int, int> key;
            for (const auto&[freq_key, count] : freqs) {
                if (count > mode) {
                    mode = count;
                    key = {freq_key.first, freq_key.second};
                }
            }

            max = max + 1;
            defs[max] = key;

            // Replace occurrences in normalized with new
            for (std::vector<Dast::LinkedList<int>>& sentence: normalized) {
                for (Dast::LinkedList<int>& word : sentence) {
                    auto node = word.get_ptr();

                    while (node != nullptr && node->next != nullptr) {
                        if (node->data == key.first && node->next->data == key.second) {
                            Dast::Node<int>* next = node->next->next;
                            node->data = max;
                            delete node->next;
                            node->next = next;
                        }
                        node = node->next;
                    }
                }
            }
        }

        // Decode in the order from least to greatest key
        std::set<std::string> tokens = {};
        for (const auto&[encoding, comps] : defs) {
            tokens.emplace(decode(defs, comps));
        }

        return tokens;
    }

    static std::string decode(const std::map<int, std::pair<int, std::optional<int>>>& defs, const std::pair<int, std::optional<int>>& encoded) {

        std::string val;
        if (encoded.second.has_value()) {
            val += decode(defs, defs.at(encoded.first));
            val += decode(defs, defs.at(encoded.second.value()));
        } else {
            val += static_cast<char>(encoded.first);
        }

        return val;
    }
};


