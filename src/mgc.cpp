#include <iostream>
#include <fstream>
#include <lexer/lexer.hpp>

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
    std::ifstream inputFile("exs/Final.g");
    inputFile.rdbuf()->pubsetbuf(nullptr, 0);

    // Read the entire file content into a string
    std::string fileContent((std::istreambuf_iterator<char>(inputFile)),
                            std::istreambuf_iterator<char>());

    inputFile.close();
    Tokenize(fileContent);
    return 0;
}