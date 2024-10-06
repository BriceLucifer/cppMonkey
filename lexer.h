//
// Created by 23766 on 2024/10/5.
//

#ifndef CPPMONKEY_LEXER_H
#define CPPMONKEY_LEXER_H

#include "token.h"

class Lexer{
private:
    std::string input;
    int position;
    int readPosition;
    char ch;
public:
    Lexer(std::string input);
    ~Lexer();
    void readChar();
    Token NextToken();
    std::string readIdentifier();
    std::string readNumber();
    void skipWhitespace();
    char peekChar();
};

static bool isLetter(char ch);
static bool isDigit(char ch);

#endif //CPPMONKEY_LEXER_H
