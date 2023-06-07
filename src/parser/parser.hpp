#include <lexer/token.hpp>
#include <regex>
#include <cstring>
#include <vector>


class Parser {
    public:
        Parser(const std::vector<Token*> tokens, std::string input) : tokens_(tokens), input_(input), position_(0), line_(0), index_(0) {}

        void parseTokens();

        void getNext();

    private:
        std::string input_;
        std::vector<Token*> tokens_;
        Token* currentToken;
        std::size_t position_;
        std::size_t line_;
        std::size_t index_;
        std::size_t braces_;


        void advance();
};

void Parse(std::vector<Token*>& tokens, std::string code);