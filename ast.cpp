//
// Created by 23766 on 2024/10/6.
//

#include "ast.h"

Program::Program() : Statements(std::vector<std::unique_ptr<Statement>>()){}
Program::~Program() = default ;

Identifier::Identifier(Token tok,std::string val): token(tok),value(val){ }
Identifier::~Identifier() = default;

LetStatement::LetStatement(Token tok): token(tok), name(nullptr), value(nullptr){}
LetStatement::~LetStatement() = default;

ReturnStatement::ReturnStatement(Token tok): token(tok), return_value(nullptr) {}
ReturnStatement::~ReturnStatement() = default;

ExpressionStatement::ExpressionStatement(Token tok) : token(tok), expression(nullptr){}
ExpressionStatement::~ExpressionStatement() = default ;

IntegerLiteral::IntegerLiteral(Token tok) : token(tok),value(static_cast<int64_t>(0)){}
IntegerLiteral::~IntegerLiteral() = default;

PrefixExpression::PrefixExpression(Token tok, std::string op) : token(tok), Operator(op), Right(nullptr){}
PrefixExpression::~PrefixExpression() = default;

InfixExpression::InfixExpression(Token tok, std::string op, std::unique_ptr<Expression> left):token(tok), Operator(op), left(std::move(left)), right(nullptr){}
InfixExpression::~InfixExpression() = default;

Boolean::Boolean(Token tok, bool boolean) : token(tok), value(boolean) {}
Boolean::~Boolean() = default;

BlockStatement::BlockStatement(Token tok): token(tok),statements(std::vector<std::unique_ptr<Statement>>()) {}
BlockStatement::~BlockStatement() = default ;

IfExpression::IfExpression(Token tok) : token(tok),condition(nullptr),alternative(nullptr),consequence(nullptr){}
IfExpression::~IfExpression() = default ;

FunctionLiteral::FunctionLiteral(Token tok) :
token(tok), parameters(std::vector<std::unique_ptr<Identifier>>()),body(nullptr){}
FunctionLiteral::~FunctionLiteral() = default ;

CallExpression::CallExpression(Token tok,std::unique_ptr<Expression> func): token(tok), function(std::move(func)), arguments(std::vector<std::unique_ptr<Expression>>()){}
CallExpression::~CallExpression() = default ;