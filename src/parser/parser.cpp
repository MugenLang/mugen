#include <lexer/token.hpp>
#include <regex>
#include <cstring>
#include <vector>


class Parser {
    public:
        Parser(const std::vector<Token*>& tokens, const std::string& input)
            : tokens_(tokens), input_(input), position_(0), line_(0), index_(0), braces_(0), currentToken(nullptr) {}

        void parseTokens() {
            while (index_ < tokens_.size()) {
                getNext();
                advance();
            }
        }

    private:
        std::string input_;
        std::vector<Token*> tokens_;
        Token* currentToken;
        std::size_t position_;
        std::size_t line_;
        std::size_t index_;
        std::size_t braces_;

        void advance() {
            index_++;
            if (index_ < tokens_.size()) {
                currentToken = tokens_[index_];
            }
        }

        void getNext() {
            if (index_ < tokens_.size()) {
                currentToken = tokens_[index_];
                if (currentToken->type == TokenType::TOKEN_IDENTIFIER && currentToken->value == "task") {
                    parseFunction();
                }
                else if(currentToken->type == TokenType::TOKEN_IDENTIFIER && next()->type==TokenType::TOKEN_LESS) {
                    parseVarDef();
                }
            }
        }

        void statements() {
            if (index_ < tokens_.size()) {
                currentToken = tokens_[index_];
                if (currentToken->type == TokenType::TOKEN_IDENTIFIER && currentToken->value == "return") {
                    parseReturn();
                }
                else if(currentToken->type == TokenType::TOKEN_IDENTIFIER && next()->type==TokenType::TOKEN_LESS) {
                    parseVarDef();
                }
                else if(currentToken->type == TokenType::TOKEN_IDENTIFIER && currentToken->value == "is") {
                    parseIf();
                }
            }
        }

        void parseFunction() {
            eat(TokenType::TOKEN_IDENTIFIER, "task");
            eat(TokenType::TOKEN_LBRACKET, "[");
            std::string ftype = eat(TokenType::TOKEN_IDENTIFIER, "", "Function_Type")->value;
            eat(TokenType::TOKEN_RBRACKET, "]");
            std::string fname = eat(TokenType::TOKEN_IDENTIFIER, "", "Function_Name")->value;
            eat(TokenType::TOKEN_LPAREN, "(");
            std::vector<std::vector<std::string>> args = {};

            while (currentToken->type != TokenType::TOKEN_RPAREN) {
                std::string argName = eat(TokenType::TOKEN_IDENTIFIER, "", "Function_Argument_Name")->value;
                eat(TokenType::TOKEN_COLON, ":");
                std::string argType = eat(TokenType::TOKEN_IDENTIFIER, "", "Function_Argument_Type")->value;
                std::vector<std::string> arg = {argType, argName};
                args.push_back(arg);
                if (currentToken->type == TokenType::TOKEN_COMMA) {
                    advance();
                    continue;
                }
            }
            eat(TokenType::TOKEN_RPAREN, ")");
            if (currentToken->type == TokenType::TOKEN_SEMICOLON) {}
            else if (currentToken->type == TokenType::TOKEN_LBRACE) {
                std::size_t pbraces = braces_;
                Token* brace=currentToken;
                braces_++;
                while (braces_ != pbraces) {
                    if ((index_) > tokens_.size()) {
                        std::cerr << "Unclosed Braces: " << brace->value << "\nLine: " << brace->line << "   Col: " << brace->col << std::endl;
                        exit(1);
                    }
                    else if (currentToken->type == TokenType::TOKEN_RBRACE) {
                        braces_--;
                        advance();
                    }
                    statements();
                    advance();
                }
            }
            else {
                std::cerr << "Unexpected token: " << currentToken->value << "\nExpected: " << "`;` or `{`\nLine: " << currentToken->line << "   Col: " << currentToken->col << std::endl;
                exit(1);
            }

        }


        void parseReturn() {
            eat(TokenType::TOKEN_IDENTIFIER, "return");
            std::string exp = parseExpression();
            eat(TokenType::TOKEN_SEMICOLON, ";");
        }

        void parseVarDef() {
            std::string vname = eat(TokenType::TOKEN_IDENTIFIER, "", "Var_Name")->value;
            eat(TokenType::TOKEN_LESS, "<");
            std::string vtype = eat(TokenType::TOKEN_IDENTIFIER, "", "Var_Type")->value;
            eat(TokenType::TOKEN_GREAT, ">");
            std::string vvalue = "";
            if (currentToken->type == TokenType::TOKEN_SEMICOLON) {advance();}
            else if (currentToken->type == TokenType::TOKEN_EQUAL) {
                advance();
                vvalue = parseExpression();
                if (currentToken->type == TokenType::TOKEN_SEMICOLON) {
                }
                else {
                    std::cerr << "Unexpected token: " << currentToken->value << "\nExpected: " << "`;`\nLine: " << currentToken->line << "   Col: " << currentToken->col << std::endl;
                    exit(1);
                }
            }
            else {
                std::cerr << "Unexpected token: " << currentToken->value << "\nExpected: " << "`;` or `=`\nLine: " << currentToken->line << "   Col: " << currentToken->col << std::endl;
                exit(1);
            }
        }


        void parseIf() {
            eat(TokenType::TOKEN_IDENTIFIER, "is");
            eat(TokenType::TOKEN_LPAREN, "(");
            std::string exp = parseExpression();
            std::cout << exp << std::endl;
        }

        std::string parseExpression() {
            std::string left = parseTerm();

            while (currentToken->type == TokenType::TOKEN_OP && isOperator(currentToken->value)) {
                std::string op = eat(TokenType::TOKEN_OP, "", "Operator")->value;

                std::string right = parseTerm();
                left = left + op + right;
            }

            return left;
        }

        std::string parseTerm() {
            std::string factor = parseFactor();

            while (currentToken->type == TokenType::TOKEN_OP && isOperator(currentToken->value)) {
                std::string op = eat(TokenType::TOKEN_OP, "", "Operator")->value;

                std::string nextFactor = parseFactor();
                factor = factor + op + nextFactor;
            }

            return factor;
        }

        std::string parseFactor() {
            if (currentToken->type == TokenType::TOKEN_INTEGER || currentToken->type == TokenType::TOKEN_FLOAT || currentToken->type == TokenType::TOKEN_IDENTIFIER) {
                std::string value = eat(currentToken->type, "", "Value")->value;
                return parsePostfix(value);
            } else if (currentToken->type == TokenType::TOKEN_LPAREN) {
                eat(TokenType::TOKEN_LPAREN, "(", "Left Parenthesis");
                std::string expr = parseExpression();
                eat(TokenType::TOKEN_RPAREN, ")", "Right Parenthesis");
                return parsePostfix(expr);
            } else {
                std::cerr << "Unexpected token: " << currentToken->value << "\nExpected: Value or Left Parenthesis\nLine: " << currentToken->line << "   Col: " << currentToken->col << std::endl;
                exit(1);
            }
        }

        std::string parsePostfix(const std::string& expr) {
            if (currentToken->type == TokenType::TOKEN_OP && isOperator(currentToken->value)) {
                std::string op = eat(TokenType::TOKEN_OP, "", "Operator")->value;
                std::string nextExpr = parseFactor();
                return parsePostfix(expr + op + nextExpr);
            } else if (isComparisonOperator(currentToken->value)) {
                std::string op = currentToken->value;
                advance();
                std::string nextExpr = parseExpression();
                return expr + op + nextExpr;
            } else {
                return expr; // Return the expression as it is without any postfix operators
            }
        }



        bool isOperator(const std::string& ope) {
            for (const char* op : OPERATORS) {
                if (std::string(op) == ope) {
                    return true;
                }
            }
            return false;
        }

        bool isComparisonOperator(const std::string& op) {
            for (const char* o : COMPARISON_OPERATORS) {
                if (std::string(o) == op) {
                    return true;
                }
            }
            return false;
        }


        Token* next() {
            return tokens_[index_+1];
        }

        void retro() {
            if (index_ > 0) {
                index_--;
                currentToken = tokens_[index_];
            }
        }

        Token* eat(TokenType ttype, const std::string& val = "", const std::string& type = "") {
            Token* mytok;
            if (index_ < tokens_.size()) {
                Token* nextToken = tokens_[index_];
                if (nextToken->type == ttype && (val.empty() || nextToken->value == val)) {
                    mytok = currentToken;
                } else {
                    std::cerr << "Unexpected token: " << nextToken->value << "\nExpected: \"" << val << "\"  \"" << type << "\"" << std::endl << "Line: " << nextToken->line << "  Col: " << nextToken->col << std::endl;
                    exit(1);
                }
            } else {
                std::cerr << "Unexpected end of tokens. Expected: " << val << " \"" << type << "\"" << std::endl;
                exit(1);
            }

            advance();
            return mytok;
        }
};


void Parse(std::vector<Token*>& tokens, std::string code) {
    Parser parser = Parser(tokens, code);
    parser.parseTokens();
}