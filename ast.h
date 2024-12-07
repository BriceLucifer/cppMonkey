//
// Created by 23766 on 2024/10/6.
//

#ifndef CPPMONKEY_AST_H
#define CPPMONKEY_AST_H

#include "token.h"
#include <vector>
#include <memory>
#include <sstream>

class Node{
public:
    virtual std::string TokenLiteral() const = 0;
    virtual std::string String() const = 0;
    Node() = default;
    ~Node() = default;
};

// statementNode
class Statement : public Node {
public:
    Statement() = default;
    ~Statement() = default;
    virtual void statementNode() const = 0;
};

// expression
class Expression : public Node {
public:
    Expression() = default;
    ~Expression() = default;
    virtual void expressionNode() const = 0;
};


class Program : Node{
public:
    std::vector<std::unique_ptr<Statement>> Statements;
    Program() ;
    ~Program();
    std::string TokenLiteral() const override {
        if(!Statements.empty()){
            return Statements[0]->TokenLiteral();
        } else {
            return "";
        }
    }
    std::string String() const override {
        std::ostringstream out;
        for (const auto& s : Statements) {
            out << s->String();
        }
        return out.str();
    }
};

class Identifier : public Expression{
public:
    Token token;
    std::string value;
    explicit Identifier(Token tok,std::string val);
    ~Identifier();
    void expressionNode() const override { }
    std::string TokenLiteral() const override{
        return token.literal;
    }
    std::string String() const override {
        return value;
    }
};

// let statement
class LetStatement : public Statement {
public:
    Token token;
    std::unique_ptr<Identifier> name;
    std::unique_ptr<Expression> value;
    explicit LetStatement(Token tok);
    ~LetStatement();
    void statementNode() const override {}
    std::string TokenLiteral() const override {
        return token.literal;
    }
    std::string String() const override {
        std::ostringstream oss;
        oss << TokenLiteral() << " ";
        oss << name->String();
        oss << " = ";
        if (value != nullptr) {
            oss << value->String() << " ";
        }
        oss << ";";
        return oss.str();
    }
};

// return statement
class ReturnStatement : public Statement {
public:
    Token token;
    std::unique_ptr<Expression> return_value;
    explicit ReturnStatement(Token tok);
    ~ReturnStatement();
    void statementNode() const override {}
    std::string TokenLiteral() const override{
        return token.literal;
    }
    std::string String() const override {
        std::ostringstream oss;
        oss << token.literal + " ";
        if (return_value != nullptr) {
            oss << return_value->String();
        }
        oss << ";";
        return oss.str();
    }
};

// expression statement
class ExpressionStatement : public Statement {
public:
    Token token;
    std::unique_ptr<Expression> expression;
    explicit ExpressionStatement(Token tok);
    ~ExpressionStatement();
    void statementNode() const override {}
    std::string TokenLiteral() const override {
        return token.literal;
    }
    std::string String() const override {
        if (expression != nullptr){
            return expression->String();
        }
        return "";
    }
};

// integer literal
class IntegerLiteral : public Expression {
public:
    Token token;
    int64_t value;
    explicit IntegerLiteral(Token tok);
    ~IntegerLiteral();
    void expressionNode() const override{ }
    std::string TokenLiteral() const override {
        return token.literal;
    }
    std::string String() const override {
        return token.literal;
    }
};

// prefix expression
class PrefixExpression : public Expression {
public:
    Token token;
    std::string Operator;
    std::unique_ptr<Expression> Right;
    explicit PrefixExpression(Token tok,std::string op);
    ~PrefixExpression();
    void expressionNode() const override{ }
    std::string TokenLiteral() const override {
        return token.literal;
    }
    std::string String() const override {
        std::ostringstream oss;
        oss << "(";
        oss << Operator;
        oss << Right->String();
        oss << ")";
        return oss.str();
    }
};

// infix expression
class InfixExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Expression> left;
    std::string Operator;
    std::unique_ptr<Expression> right;
    explicit InfixExpression(Token tok,std::string op,std::unique_ptr<Expression> left);
    ~InfixExpression();
    void expressionNode() const override {}
    std::string TokenLiteral() const override {
        return token.literal;
    }
    std::string String() const override {
        std::ostringstream oss;
        oss << "(";
        oss << left->String();
        oss << " " + Operator + " ";
        oss << right->String();
        oss << ")";
        return oss.str();
    }
};

// boolean literals
class Boolean : public Expression {
public:
    Token token;
    bool value;
    explicit Boolean(Token tok, bool boolean);
    ~Boolean();
    void expressionNode() const override {}
    std::string TokenLiteral() const override {
        return token.literal;
    }
    std::string String() const override {
        return token.literal;
    }
};

// block statement
class BlockStatement : Statement {
public:
    Token token;
    std::vector<std::unique_ptr<Statement>> statements;
    explicit BlockStatement(Token tok);
    ~BlockStatement();
    void statementNode() const override { }
    std::string TokenLiteral() const override {
        return token.literal;
    }
    std::string String() const override {
        std::ostringstream oss;
        for (auto &s: statements){
            oss << s->String();
        }
        return oss.str();
    }
};

class IfExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> consequence;
    std::unique_ptr<BlockStatement> alternative;
    explicit IfExpression(Token tok);
    ~IfExpression();
    void expressionNode() const override { }
    std::string TokenLiteral() const override {
        return token.literal;
    }
    std::string String() const override {
        std::ostringstream oss;
        oss << "if ";
        oss << "[condition (";
        oss << condition->String();
        oss << ")] ";
        oss << consequence->String();

        if (alternative != nullptr) {
            oss << " else ";
            oss << alternative->String();
        }

        return oss.str();
    }
};

class FunctionLiteral : public Expression {
public:
    Token token;
    std::vector<std::unique_ptr<Identifier>> parameters;
    std::unique_ptr<BlockStatement> body;
    explicit FunctionLiteral(Token tok);
    ~FunctionLiteral();
    void expressionNode() const override {}
    std::string TokenLiteral() const override {
        return token.literal;
    }
    std::string String() const override {
        std::ostringstream oss;
        oss << TokenLiteral();
        oss << "( ";
        for(auto &s: parameters){
            oss << s->String() << ", ";
        }
        oss << ") ";
        oss << body->String();
        return oss.str();
    }
};

class CallExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Expression> function;
    std::vector<std::unique_ptr<Expression>> arguments;
    explicit CallExpression(Token tok, std::unique_ptr<Expression> func);
    ~CallExpression();
    void expressionNode() const override { }
    std::string TokenLiteral() const override {
        return token.literal;
    }
    std::string String() const override {
        std::ostringstream oss;
        oss << function->String();
        oss << "(";
        for (auto &s : arguments){
            oss << s->String() << ", ";
        }
        oss << ")";
        return oss.str();
    }
};

#endif //CPPMONKEY_AST_H
