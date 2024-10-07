//
// Created by 23766 on 2024/10/7.
//

#ifndef CPPMONKEY_EVALUATOR_H
#define CPPMONKEY_EVALUATOR_H

#include "ast.h"
#include "object.h"

static std::unique_ptr<Null> N = std::make_unique<Null>();
static std::unique_ptr<Boolean_obj> True = std::make_unique<Boolean_obj>(true);
static std::unique_ptr<Boolean_obj> False = std::make_unique<Boolean_obj>(false);

std::unique_ptr<Object> Eval(std::unique_ptr<Node> node, std::unique_ptr<Environment> env);
std::unique_ptr<Object> evalProgram(std::unique_ptr<Program> program, std::unique_ptr<Environment> env);
std::unique_ptr<Object> evalBlockStatements(std::unique_ptr<BlockStatement> blockStatement, std::unique_ptr<Environment> env);
std::unique_ptr<Boolean_obj> nativeBoolToBooleanObject(bool input);
std::unique_ptr<Object> evalPrefixExpression(std::string Operator, std::unique_ptr<Object> right);


#endif //CPPMONKEY_EVALUATOR_H
