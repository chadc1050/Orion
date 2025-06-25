#pragma once
#include <fstream>
#include <string>
#include <vector>

using Translation = std::pair<std::string, std::string>;

inline std::vector<Translation> read_file(const std::string& path) {
    std::ifstream file;

    file.open(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    std::string line;

    std::vector<Translation> translations;

    bool first = true;

    while (std::getline(file, line, '\n')) {

        // Skip header
        if (first) {
            first = false;
            continue;
        }

        std::vector<std::string> fields;
        std::string field;
        bool in_quotes = false;

        for (size_t i = 0; i < line.size(); i++) {
            if (char c = line[i]; c == '"') {
                // Check for escaped quotes
                if (i + 1 == line.size() && line[i + 1] == '"') {
                    field += '"';
                    // Skip the next quote
                    i++;
                } else {
                    in_quotes = !in_quotes;
                }
            } else if (c == ',' && !in_quotes) {
                fields.push_back(field);
                field.clear();
            } else {
                field += c;
            }
        }

        fields.push_back(field);

        if (fields.size() != 2) {
            throw std::runtime_error("Parse error!");
        }

        translations.emplace_back(fields[1], fields[0]);
    }

    file.close();

    return translations;
}