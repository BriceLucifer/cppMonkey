//
// Created by 23766 on 2024/10/6.
//

#include "parser.h"
#include <memory>
#include <typeinfo>
#include <utility>

Parser::Parser(std::unique_ptr<Lexer> l)
        : L(std::move(l)),
          errors(std::vector<std::string>()),
          prefixParserFns(std::map<TokenType, PrefixParseFn>()),
          infixParserFns(std::map<TokenType, InfixParseFn>()) {
    nextToken();
    nextToken();

    // 注册前缀解析函数
    registerPrefix(TokenType::IDENT, [this] { return parseIdentifier(); });
    registerPrefix(TokenType::INT, [this] { return parseIntegerLiteral(); });  // 修改为 parseIntegerLiteral
    registerPrefix(TokenType::BANG, [this] { return parsePrefixExpression(); });
    registerPrefix(TokenType::MINUS, [this] { return parsePrefixExpression(); });
    registerPrefix(TokenType::TRUE, [this] { return parseBoolean(); });
    registerPrefix(TokenType::FALSE, [this] { return parseBoolean(); });
    registerPrefix(TokenType::LPAREN, [this] { return parseGroupExpression(); });
    registerPrefix(TokenType::IF, [this] { return parseIfExpression(); });
    registerPrefix(TokenType::FUNCTION, [this] { return parseFunctionLiteal(); });  // 修改为 lambda 函数

    // 注册中缀解析函数
    registerInfix(TokenType::PLUS, [this](std::unique_ptr<Expression> left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::MINUS, [this](std::unique_ptr<Expression> left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::SLASH, [this](std::unique_ptr<Expression> left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::ASTERISK, [this](std::unique_ptr<Expression> left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::EQ, [this](std::unique_ptr<Expression> left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::NOT_EQ, [this](std::unique_ptr<Expression> left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::LT, [this](std::unique_ptr<Expression> left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::GT, [this](std::unique_ptr<Expression> left) { return parseInfixExpression(std::move(left)); });
}

Parser::~Parser() = default;

void Parser::nextToken() {
    curToken = peekToken;
    peekToken = L->NextToken();
}

std::unique_ptr<Program> Parser::ParseProgram() {
    std::unique_ptr<Program> program = std::make_unique<Program>();
    while (curToken.type != TokenType::EOF_){
        auto stmt = parseStatement();
        if (stmt != nullptr){
            program->Statements.push_back(std::move(stmt));
        }
        nextToken();
    }
    return program;
}

std::unique_ptr<Statement> Parser::parseStatement() {
    switch (curToken.type) {
        case TokenType::LET:
            return parseLetStatement();
        case TokenType::RETURN:
            return parseReturnStatement();
        default:
            return parseExpressionStatement();
    }
}

std::unique_ptr<LetStatement> Parser::parseLetStatement() {
    auto stmt = std::make_unique<LetStatement>(curToken);
    if (!expectPeek(TokenType::IDENT)) {
        return nullptr;
    }

    stmt->name = std::make_unique<Identifier>(curToken, curToken.literal);

    if (!expectPeek(TokenType::ASSIGN)) {
        return nullptr;
    }

    nextToken();

    stmt->value = parseExpression(Precedence::LOWEST);

    if (peekTokenIs(TokenType::SEMICOLON)) {
        nextToken();
    }

    return stmt;
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement() {
    std::unique_ptr<ReturnStatement> stmt(new ReturnStatement(curToken));

    nextToken();

    stmt->return_value = parseExpression(Precedence::LOWEST);

    if (peekTokenIs(TokenType::SEMICOLON)){
        nextToken();
    }

    return stmt;
}

std::unique_ptr<ExpressionStatement> Parser::parseExpressionStatement() {
    auto stmt = std::make_unique<ExpressionStatement>(curToken);
    stmt->expression = parseExpression(Precedence::LOWEST);

    if (peekTokenIs(TokenType::SEMICOLON)){
        nextToken();
    }

    return stmt;
}

std::unique_ptr<Expression> Parser::parseExpression(Precedence precedence) {
    auto prefix = prefixParserFns[curToken.type];
    if (prefix == nullptr){
        noPrefixParseFnError(curToken.type);
        return nullptr;
    }

    auto leftexp = prefix();

    while (!peekTokenIs(TokenType::SEMICOLON) && precedence < peekPrecedence()){
        auto infix = infixParserFns[peekToken.type];
        if (infix == nullptr){
            return leftexp;
        }

        nextToken();

        leftexp = infix(std::move(leftexp)); // 消耗一个然后返回一个 挺好
    }

    return leftexp;
}

std::unique_ptr<Expression> Parser::parseIdentifier() {
    auto i = std::make_unique<Identifier>(curToken,curToken.literal);
    // c++ 多一步转换
    return i;
}

std::unique_ptr<Expression> Parser::parseIntegerLiteral() {
    auto lit = std::make_unique<IntegerLiteral>(curToken);
    int64_t value;
    try {
        value = std::stoll(curToken.literal);
    }catch (const std::exception& e){
        std::ostringstream msg;
        msg << "could not parse " << curToken.literal << " as integer";
        errors.push_back(msg.str());
        return nullptr;
    }

    lit->value = value;
    return std::unique_ptr<Expression>(lit.release());
}

std::unique_ptr<Expression> Parser::parsePrefixExpression() {
    auto expression = std::make_unique<PrefixExpression>(curToken,curToken.literal);

    nextToken();

    expression->Right = parseExpression(Precedence::PREFIX);

    return std::unique_ptr<Expression>(expression.release());
}

std::unique_ptr<Expression> Parser::parseInfixExpression(std::unique_ptr<Expression> left) {
    auto expression = std::make_unique<InfixExpression>(curToken, curToken.literal, std::move(left));

    auto precedence = curPrecedence();
    nextToken();
    expression->right = parseExpression(precedence);

    return std::unique_ptr<Expression>(expression.release());
}

std::unique_ptr<Expression> Parser::parseGroupExpression() {
    nextToken();

    auto exp = parseExpression(Precedence::LOWEST);

    if (!expectPeek(TokenType::RPAREN)){
        return nullptr;
    }

    return exp;
}

std::unique_ptr<Expression> Parser::parseBoolean() {
    auto b = std::make_unique<Boolean>(curToken, curTokenIs(TokenType::TRUE));
    return std::unique_ptr<Expression>(b.release());
}

std::unique_ptr<Expression> Parser::parseIfExpression() {
    auto expression = std::make_unique<IfExpression>(curToken);

    if (!expectPeek(TokenType::LPAREN)){
        return nullptr;
    }

    nextToken();
    expression->condition = parseExpression(Precedence::LOWEST);

    if (!expectPeek(TokenType::RPAREN)){
        return nullptr;
    }

    if (!expectPeek(TokenType::LBRACE)){
        return nullptr;
    }

    expression->consequence = parseBlockStatement();

    if (peekTokenIs(TokenType::ELSE)){
        nextToken();

        if (!expectPeek(TokenType::LBRACE)){
            return nullptr;
        }

        expression->alternative = parseBlockStatement();
    }

    return expression;
}

std::unique_ptr<BlockStatement> Parser::parseBlockStatement() {
    auto block = std::make_unique<BlockStatement>(curToken);

    nextToken();

    while(!curTokenIs(TokenType::RBRACE) && !curTokenIs(TokenType::EOF_)){
        auto stmt = parseStatement();
        if (stmt != nullptr){
            block->statements.push_back(std::move(stmt));
        }
        nextToken();
    }

    return block;
}

std::unique_ptr<Expression> Parser::parseFunctionLiteal() {
    auto lit = std::make_unique<FunctionLiteral>(curToken);

    if (!expectPeek(TokenType::LPAREN)){
        return nullptr;
    }

    lit->parameters = parseFunctionParameters();


    if (!expectPeek(TokenType::LBRACE)){
        return nullptr;
    }

    lit->body = parseBlockStatement();

    return lit;
}

std::vector<std::unique_ptr<Identifier>> Parser::parseFunctionParameters() {
    auto identifiers = std::vector<std::unique_ptr<Identifier>>();

    if (peekTokenIs(TokenType::RPAREN)){
        nextToken();
        return identifiers;
    }

    nextToken();

    auto ident = std::make_unique<Identifier>(curToken,curToken.literal);
    identifiers.push_back(std::move(ident));

    while (peekTokenIs(TokenType::COMMA)){
        nextToken();
        nextToken();
        ident = std::make_unique<Identifier>(curToken,curToken.literal);
        identifiers.push_back(std::move(ident));
    }

    if (!expectPeek(TokenType::RPAREN)){
        return {};
    }

    return identifiers;
}

std::unique_ptr<Expression> Parser::parseCallExpression(std::unique_ptr<Expression> function) {
    auto exp = std::make_unique<CallExpression>(curToken,std::move(function));
    exp->arguments = parseCallArguments();
    return exp;
}

std::vector<std::unique_ptr<Expression>> Parser::parseCallArguments() {
    auto args = std::vector<std::unique_ptr<Expression>>();

    if (peekTokenIs(TokenType::RPAREN)){
        nextToken();
        return args;
    }

    nextToken();
    args.push_back(parseExpression(Precedence::LOWEST));

    while (peekTokenIs(TokenType::COMMA)) {
        nextToken();
        nextToken();
        args.push_back(parseExpression(Precedence::LOWEST));
    }

    if (!expectPeek(TokenType::RPAREN)){
        return {};
    }

    return args;
}

bool Parser::curTokenIs(TokenType t) const {
    return t == curToken.type;
}

bool Parser::peekTokenIs(TokenType t) const {
    return t == peekToken.type;
}

bool Parser::expectPeek(TokenType t) {
    if (peekTokenIs(t)) {
        nextToken();
        return true;
    } else{
        peekError(t);
        return false;
    }
}

std::vector<std::string> Parser::Errors() const {
    return errors;
}

void Parser::peekError(TokenType t) {
    std::ostringstream msg;
    msg << "Expected next token to be "
        << tokenTypeToString(t)
        << ", got "
        << tokenTypeToString(peekToken.type)
        << " instead";

    errors.push_back(msg.str());
}

void Parser::registerPrefix(TokenType tokentype, PrefixParseFn fn) {
    prefixParserFns[tokentype] = std::move(fn);
}

void Parser::registerInfix(TokenType tokentype, InfixParseFn fn) {
    infixParserFns[tokentype] = std::move(fn);
}

void Parser::noPrefixParseFnError(TokenType t) {
    std::ostringstream msg;
    msg << "no prefix parse function for " << tokenTypeToString(t) << " found";
    errors.push_back(msg.str());
}

Precedence Parser::peekPrecedence() const {
    auto p = precedences.find(peekToken.type);
    if (p != precedences.end()){
        return p->second;
    }
    return Precedence::LOWEST;
}

Precedence Parser::curPrecedence() const {
    auto p = precedences.find(curToken.type);
    if (p != precedences.end()) {
        return p->second;
    }
    return Precedence::LOWEST;
}