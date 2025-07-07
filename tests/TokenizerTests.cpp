#include "catch2/catch_amalgamated.hpp"
#include "Orion/Tokenizer.hpp"

TEST_CASE("Tokenizer", "[Tokenizer]") {

    SECTION("Byte Pair Encoding") {

        const std::set<std::string> tokens = BytePairTokenizer().tokenize({ "This is a test." }, 13, true);

        REQUIRE(tokens.size() == 13);

        const std::set<std::string> tokens2 = BytePairTokenizer().tokenize({ "This is a test." }, 14, true);

        REQUIRE(tokens2.size() == 13);
    }
}