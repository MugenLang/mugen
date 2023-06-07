#include <lexer/token.hpp>
#include <regex>

class Lexer {
    public:
        Lexer(const std::string& input) : input_(input), position_(0) {}

        Token& getNextToken();

    private:
        std::string input_;
        std::size_t position_;

        bool isOperator();

        Token parseNum();
        Token parseId();

        char next();
        char beef();
        void retro();
        void advance();
        void setPos(int pos);

};

void Tokenize(std::string code);