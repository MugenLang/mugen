#include <iostream>

#ifndef TOKEN_HPP
#define TOKEN_HPP

extern const char* OPERATORS[6];

enum TokenType {
  // TYPES
  TOKEN_INTEGER,
  TOKEN_FLOAT,
  TOKEN_IDENTIFIER,

  // SYMBOLS
  TOKEN_COLON,
  TOKEN_DCOLON,
  TOKEN_SEMICOLON,
  TOKEN_STRING,
  TOKEN_CHAR,

  TOKEN_LBRACKET,
  TOKEN_RBRACKET,

  TOKEN_COMMA,

  TOKEN_LPAREN,
  TOKEN_RPAREN,

  TOKEN_LBRACE,
  TOKEN_RBRACE,

  TOKEN_QMARK,
  TOKEN_LESS,
  TOKEN_GREAT,
  

  // LOGICAL
  TOKEN_EQUAL,
  TOKEN_DEQUAL,
  TOKEN_NEQUAL,
  TOKEN_OR,
  TOKEN_DLR,


  // ARITMETHIC
  TOKEN_OP,
  
  
  TOKEN_EOF,
  TOKEN_DEFAULT,
};

struct Token {
  TokenType type;
  std::string value;
  std::string typestr;
  unsigned long long pos;
  unsigned long long line;
  unsigned long long col;
};


#endif