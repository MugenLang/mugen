#include <iostream>
#include <fstream>
#include <lexer/lexer.hpp>
#include <lexer/token.hpp>
#include <parser/parser.hpp>

/*
🟦🟦🟦🟦🟦➖🟦🟦
⬜🔳⬜🟦🟦🟦🟦🟦
➖⬜⬜⬜⬜🟦🟦
➖➖⬜⬜⬜⬜🟦🟦
➖🟦🟦➖⬜⬜🟦🟦
➖➖➖➖➖⬜🟦🟦
➖➖➖➖⬜🟦🟦
➖➖➖🟦⬜🟦
➖➖➖🟦🟦
Blahaj Debugging <3*/

int main() {
    std::ifstream inputFile("exs/01-Functions.g");
    inputFile.rdbuf()->pubsetbuf(nullptr, 0);

    // Read the entire file content into a string
    std::string fileContent((std::istreambuf_iterator<char>(inputFile)),
                            std::istreambuf_iterator<char>());

    inputFile.close();
    std::vector<Token*> tokens=Tokenize(fileContent);
    Parse(tokens, fileContent);
    return 0;
}