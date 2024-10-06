//
// Created by 23766 on 2024/10/6.
//

#ifndef CPPMONKEY_PARSER_H
#define CPPMONKEY_PARSER_H

#include "ast.h"
#include "lexer.h"
#include <functional>

enum class Precedence{
    LOWEST,
    EQUALS,
    LESSGREATER,
    SUM,
    PRODUCT,
    PREFIX,
    CALL,
};

using PrefixParseFn = std::function<std::unique_ptr<Expression>()>;
using InfixParseFn = std::function<std::unique_ptr<Expression>(std::unique_ptr<Expression>)>;

class Parser {
public:
    std::unique_ptr<Lexer> L;
    std::vector<std::string> errors;

    Token curToken;
    Token peekToken;

    std::map<TokenType,PrefixParseFn> prefixParserFns;
    std::map<TokenType,InfixParseFn> infixParserFns;

    explicit Parser(std::unique_ptr<Lexer> l);
    ~Parser();
// methods
    void nextToken();
    std::unique_ptr<Program> ParseProgram();
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<LetStatement> parseLetStatement();
    std::unique_ptr<ReturnStatement> parseReturnStatement();
    std::unique_ptr<ExpressionStatement> parseExpressionStatement();
    std::unique_ptr<Expression> parseExpression(Precedence precedence);
    std::unique_ptr<Expression> parseIdentifier();
    std::unique_ptr<Expression> parseIntegerLiteral();
    std::unique_ptr<Expression> parsePrefixExpression();
    std::unique_ptr<Expression> parseInfixExpression(std::unique_ptr<Expression> left);
    std::unique_ptr<Expression> parseGroupExpression();
    std::unique_ptr<Expression> parseBoolean();
    std::unique_ptr<Expression> parseIfExpression();
    std::unique_ptr<BlockStatement> parseBlockStatement();
    std::unique_ptr<Expression> parseFunctionLiteal();
    std::vector<std::unique_ptr<Identifier>> parseFunctionParameters();
    std::unique_ptr<Expression> parseCallExpression(std::unique_ptr<Expression> function);
    std::vector<std::unique_ptr<Expression>> parseCallArguments();
    bool curTokenIs(TokenType t) const;
    bool peekTokenIs(TokenType t) const;
    bool expectPeek(TokenType t);
    std::vector<std::string> Errors() const;
    void peekError(TokenType t);
    void registerPrefix(TokenType tokentype,PrefixParseFn fn);
    void registerInfix(TokenType tokentype, InfixParseFn fn);
    void noPrefixParseFnError(TokenType t);
    Precedence peekPrecedence() const;
    Precedence curPrecedence() const;
};

static std::map<TokenType, Precedence> precedences = {
        {TokenType::EQ, Precedence::EQUALS},
        {TokenType::NOT_EQ, Precedence::EQUALS},
        {TokenType::LT, Precedence::LESSGREATER},
        {TokenType::GT, Precedence::LESSGREATER},
        {TokenType::PLUS, Precedence::SUM},
        {TokenType::MINUS, Precedence::SUM},
        {TokenType::SLASH, Precedence::PRODUCT},
        {TokenType::ASTERISK, Precedence::PRODUCT},
        {TokenType::LPAREN, Precedence::CALL},
};

#endif //CPPMONKEY_PARSER_H
