//
// Created by 23766 on 2024/10/5.
//
#include "token.h"

TokenType LookupIndent(std::string_view ident) {
    std::map<std::string_view, TokenType> KEYWORDS{
            {"fn",     TokenType::FUNCTION},
            {"let",    TokenType::LET},
            {"true",   TokenType::TRUE},
            {"false",  TokenType::FALSE},
            {"if",     TokenType::IF},
            {"else",   TokenType::ELSE},
            {"return", TokenType::RETURN},
    };
    auto tok = KEYWORDS.find(ident);
    if (tok != KEYWORDS.end()) {
        return tok->second;
    } else {
        return TokenType::IDENT;
    }
}