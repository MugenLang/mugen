#include <lexer/token.hpp>
#include <regex>

class Lexer {
    public:
        Lexer(const std::string& input) : input_(input), position_(0) {}

        Token& getNextToken();

    private:
        std::string input_;
        char currentChar;
        std::size_t position_;
        std::size_t line_;
        std::size_t col_;

        bool isOperator();

        Token parseNum();
        Token parseId();

        char next();
        char beef();
        void retro();
        void advance();
        void setPos(int pos);

};

std::vector<Token*> Tokenize(std::string code);