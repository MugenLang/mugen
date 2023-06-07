#include <lexer/token.hpp>
#include <regex>
#include <cstring>
#include <vector>


class Lexer {
    public:
        Lexer(const std::string& input) : input_(input), position_(0), line_(0) {}

        Token* getNextToken() {
            if (position_ >= input_.size()) {
                return new Token{TOKEN_EOF, "", "EOF", position_, line_};
            }

            advance();

            while (isspace(currentChar)) {
                if (currentChar == '\n') {line_++;}
                advance();}

            if (currentChar == '\0') {return new Token{TOKEN_EOF, "", "EOF", position_, line_};}

            if (isdigit(currentChar)) {
                return parseNum();
            }
            else if (isalnum(currentChar)) {
                return parseId();
            }

            else if (
                (currentChar == ':') ||
                (currentChar == ';') ||
                (currentChar == '(') ||
                (currentChar == ')') ||
                (currentChar == ',') ||
                (currentChar == '[') ||
                (currentChar == ']') ||
                (currentChar == '[') ||
                (currentChar == '{') ||
                (currentChar == '}') ||
                (currentChar == '?') ||
                (currentChar == '<') ||
                (currentChar == '>') ||
                (currentChar == '=') || 
                (currentChar == '"') ||
                (currentChar == '\'') ||
                (currentChar == '|') ||
                (currentChar == '$')
            ) {
                if ((std::string(1,currentChar)+std::string(1,next())) == "::") {
                    advance();
                    return new Token{TokenType::TOKEN_DCOLON, "::", "DColon", position_, line_};
                }
                else if ((std::string(1,currentChar)+std::string(1,next())) == "==") {
                    advance();
                    return new Token{TokenType::TOKEN_DEQUAL, "==", "Dequal", position_, line_};
                }
                else if ((std::string(1,currentChar)+std::string(1,next())) == "!=") {
                    advance();
                    return new Token{TokenType::TOKEN_NEQUAL, "!=", "Nequal", position_, line_};
                }
                else if ((std::string(1,currentChar)+std::string(1,next())) == ";;") {
                    parseComment();
                }
                switch (currentChar) {
                    case ':': return new Token{TokenType::TOKEN_COLON, ":", "Colon", position_, line_};
                    case ';': return new Token{TokenType::TOKEN_SEMICOLON, ";", "Semicolon", position_, line_};
                    case '(': return new Token{TokenType::TOKEN_LPAREN, "(", "LParen", position_, line_};
                    case ')': return new Token{TokenType::TOKEN_RPAREN, ")", "RParen", position_, line_};
                    case ',': return new Token{TokenType::TOKEN_COMMA, ",", "Comma", position_};
                    case '[': return new Token{TokenType::TOKEN_LBRACKET, "[", "LBracket", position_, line_};
                    case ']': return new Token{TokenType::TOKEN_RBRACKET, "]", "RBracket", position_, line_};
                    case '{': return new Token{TokenType::TOKEN_LBRACE, "{", "LBrace", position_, line_};
                    case '}': return new Token{TokenType::TOKEN_RBRACE, "}", "RBrace", position_, line_};
                    case '?': return new Token{TokenType::TOKEN_QMARK, "?", "QeMark", position_, line_};
                    case '>': return new Token{TokenType::TOKEN_GREAT, ">", "Great", position_, line_};
                    case '<': return new Token{TokenType::TOKEN_LESS, "<", "Less", position_, line_};
                    case '=': return new Token{TokenType::TOKEN_EQUAL, "=", "Eq", position_, line_};
                    case '|': return new Token{TokenType::TOKEN_OR, "|", "Or", position_, line_};
                    case '$': return new Token{TokenType::TOKEN_DLR, "$", "Dollar", position_, line_};
                    case '"': return parseString();
                    case '\'': return parseChar();

                }
            }

            else if (isOperator() || dOperator()) {
                if (currentChar == '-' || currentChar == '+') {
                    if (!isalnum(beef())) {
                        return parseNum();
                    }
                    else {
                        if (dOperator()) {
                            return new Token{TokenType::TOKEN_OP, (std::string(1,currentChar)+std::string(1,next())), "Operator", position_, line_};
                        }
                        else {
                            return new Token{TokenType::TOKEN_OP, std::string(1, currentChar), "Operator", position_, line_};
                        }
                    }
                }
            }

            else {
                std::cerr << "Unknown Char: " << currentChar << std::endl;
                exit(1);
            }

            return new Token{TOKEN_DEFAULT, std::string(1, currentChar), "DEFAULT", position_, line_};
        }

    private:
        std::string input_;
        char currentChar;
        std::size_t position_;
        std::size_t line_;

        bool isOperator() {
            for (const char* op : OPERATORS) {
                if (std::string(op) == std::string(1, currentChar)) {
                    return true;
                }
            }
            return false;
        }

        bool dOperator() {
            for (const char* op : OPERATORS) {
                if (std::string(op) == (std::string(1,currentChar)+std::string(1,next()))) {
                    return true;
                }
            }
            return false;
        }

        Token* parseNum() {
            std::string value="";
            bool isInt=true;

            if (currentChar == '-' || currentChar == '+') {
                value+=currentChar;
                advance();
            }

            while (isdigit(currentChar) || (currentChar == '.')) {
                if (currentChar == '.') {isInt=false;}
                value+=currentChar;
                advance();
            }

            retro();

            if (isInt) {
                return new Token{TokenType::TOKEN_INTEGER, value, "Integer", position_, line_};
            }
            else {
                return new Token{TokenType::TOKEN_FLOAT, value, "Float", position_, line_};
            }
        }

        Token* parseId() {
            std::string value="";
            bool isInt=true;
            while (isalnum(currentChar) || currentChar == '.') {
                value+=currentChar;
                advance();
            }

            retro();

            return new Token{TokenType::TOKEN_IDENTIFIER, value, "Identifier", position_, line_};
        }

        Token* parseString() {
            std::string value="";
            while (currentChar != '\0') {
                value+=currentChar;
                if (currentChar == '"') {
                    break;
                }
                advance();
            }
            return new Token{TokenType::TOKEN_STRING, value, "String", position_, line_};
        }

        Token* parseChar() {
            std::string value="";
            while (currentChar != '\0') {
                value+=currentChar;
                if (currentChar == '\'') {
                    break;
                }
                advance();
            }
            return new Token{TokenType::TOKEN_CHAR, value, "Char", position_, line_};
        }

        void parseComment() {
            while (currentChar != '\n') {
                advance();
            }
        }

        char next() {
            return input_[position_+1];
        }

        char beef() {
            return input_[position_-1];
        }
        
        void retro() {
            position_--;
            currentChar=input_[position_];
        }

        void advance() {
            position_++;
            currentChar=input_[position_];
        }

        void setPos(int pos) {
            position_=pos;
            currentChar=input_[position_];
        }

};

std::vector<Token*> Tokenize(std::string code) {
    Lexer lexer(std::string("!") + code);
    Token* tokenPtr = lexer.getNextToken();
    std::vector<Token*> tokens;

    while (tokenPtr->type != TOKEN_EOF) {
        if (tokenPtr->type != TokenType::TOKEN_DEFAULT) {
            tokens.push_back(tokenPtr);
        }
        tokenPtr = lexer.getNextToken();
    }
    delete tokenPtr;
    
    return tokens;
}