//
// Created by 23766 on 2024/10/5.
//

#include "lexer.h"

#include <utility>

Lexer::Lexer(std::string input): input(std::move(input)),position(0),readPosition(0) {
    readChar();
}
Lexer::~Lexer() = default;
void Lexer::readChar() {
    if (readPosition >= input.length()){
        ch = 0;
    } else {
        ch = input[readPosition];
    }
    position = readPosition;
    readPosition += 1;
}
Token Lexer::NextToken() {
    Token tok;
    skipWhitespace();
    switch (ch) {
        case '=':
            if (peekChar() == '='){
                auto ch_t = ch;
                readChar();
                auto literal_t = std::string{ch_t} + std::string{ch};
                tok = {TokenType::EQ, literal_t};
            } else {
                tok = {TokenType::ASSIGN, std::string{ch}};
            }
            break;
        case '-':
            tok = {TokenType::MINUS, std::string{ch} };
            break;
        case '!':
            if (peekChar() == '=') {
                auto ch_t = ch;
                readChar();
                auto literal_t = std::string{ch_t} + std::string{ch};
                tok = {TokenType::NOT_EQ, literal_t};
            } else {
                tok = {TokenType::BANG, std::string{ch}};
            }
            break;
        case '/':
            tok = {TokenType::SLASH, std::string{ch}};
            break;
        case '*':
            tok = {TokenType::ASTERISK, std::string{ch}};
            break;
        case '<':
            tok = {TokenType::LT, std::string{ch}};
            break;
        case '>':
            tok = {TokenType::GT, std::string{ch}};
            break;
        case ';':
            tok = {TokenType::SEMICOLON, std::string{ch}};
            break;
        case '(':
            tok = {TokenType::LPAREN, std::string{ch}};
            break;
        case ')':
            tok = {TokenType::RPAREN, std::string{ch}};
            break;
        case ',':
            tok = {TokenType::COMMA, std::string{ch}};
            break;
        case '+':
            tok = {TokenType::PLUS, std::string{ch}};
            break;
        case '{':
            tok = {TokenType::LBRACE, std::string{ch}};
            break;
        case '}':
            tok = {TokenType::RBRACE, std::string{ch}};
            break;
        case 0:
            tok = {TokenType::EOF_, ""};
            break;
        default:
            if (isLetter(ch)){
                tok.literal = readIdentifier();
                tok.type = LookupIndent(tok.literal);
                return tok;
            }else if (isDigit(ch)) {
                tok.type = TokenType::INT;
                tok.literal = readNumber();
                return tok;
            }else {
                tok = {TokenType::ILLEGAL, std::string{ch}};
            }
    }
    readChar();
    return tok;
}

std::string Lexer::readIdentifier() {
    auto pos = position;
    while (isLetter(ch)){
        readChar();
    }
    return input.substr(pos,position-pos);
    // 开始位置 pos   count 长度
}

std::string Lexer::readNumber() {
    auto pos = position;
    while (isDigit(ch)){
        readChar();
    }
    return input.substr(pos, position-pos);
}

void Lexer::skipWhitespace() {
    while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r'){
        readChar();
    }
}

char Lexer::peekChar() {
    if (readPosition >= input.length()) {
        return 0;
    } else {
        return input[readPosition];
    }
}

// helper function
static bool isLetter(char ch){
    return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z' || ch == '_';
}
static bool isDigit(char ch){
    return '0' <= ch && ch <= '9';
}