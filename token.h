//
// Created by 23766 on 2024/10/5.
//

#ifndef CPPMONKEY_TOKEN_H
#define CPPMONKEY_TOKEN_H

#include <string>
#include <map>

enum class TokenType {
    ILLEGAL,
    EOF_,
    IDENT,
    INT,
    ASSIGN,
    PLUS,
    MINUS,
    BANG,
    ASTERISK,
    SLASH,
    LT,
    GT,
    COMMA,
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    FUNCTION,
    LET,
    TRUE,
    FALSE,
    IF,
    ELSE,
    RETURN,
    EQ,
    NOT_EQ
};

// 替换go的const ()
constexpr std::string_view tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::ILLEGAL: return "ILLEGAL";
        case TokenType::EOF_: return "EOF";
        case TokenType::IDENT: return "IDENT";
        case TokenType::INT: return "INT";
        case TokenType::ASSIGN: return "=";
        case TokenType::PLUS: return "+";
        case TokenType::MINUS: return "-";
        case TokenType::BANG: return "!";
        case TokenType::ASTERISK: return "*";
        case TokenType::SLASH: return "/";
        case TokenType::LT: return "<";
        case TokenType::GT: return ">";
        case TokenType::COMMA: return ",";
        case TokenType::SEMICOLON: return ";";
        case TokenType::LPAREN: return "(";
        case TokenType::RPAREN: return ")";
        case TokenType::LBRACE: return "{";
        case TokenType::RBRACE: return "}";
        case TokenType::FUNCTION: return "FUNCTION";
        case TokenType::LET: return "LET";
        case TokenType::TRUE: return "TRUE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::IF: return "IF";
        case TokenType::ELSE: return "ELSE";
        case TokenType::RETURN: return "RETURN";
        case TokenType::EQ: return "==";
        case TokenType::NOT_EQ: return "!=";
    }
    return "UNKNOWN";
}

// 替换了const

class Token{
public:
    TokenType type;
    std::string  literal;
};

// LookupIndent
TokenType LookupIndent(std::string_view ident);

#endif //CPPMONKEY_TOKEN_H
