//
// Created by 23766 on 2024/10/7.
//
#include "evaluator.h"

std::unique_ptr<Object> Eval(std::unique_ptr<Node> node, std::unique_ptr<Environment> env);
std::unique_ptr<Object> evalProgram(std::unique_ptr<Program> program, std::unique_ptr<Environment> env);
std::unique_ptr<Object> evalBlockStatements(std::unique_ptr<BlockStatement> blockStatement, std::unique_ptr<Environment> env);
std::unique_ptr<Boolean_obj> nativeBoolToBooleanObject(bool input);
std::unique_ptr<Object> evalPrefixExpression(std::string Operator, std::unique_ptr<Object> right);
std::unique_ptr<Object> evalBangOperatorExpression(std::unique_ptr<Object> right);
std::unique_ptr<Object> evalInfixExpression(std::string Operator, std::unique_ptr<Object> left,std::unique_ptr<Object> right);
std::unique_ptr<Object> evalIntegerInfixExpression(std::string Operator, std::unique_ptr<Object> left,std::unique_ptr<Object> right);
std::unique_ptr<Object> evalIfExpression(std::unique_ptr<IfExpression> ie, std::unique_ptr<Environment>);
std::unique_ptr<Object> evalIdentifier(std::unique_ptr<Identifier> node, std::unique_ptr<Environment> env);
std::vector<std::unique_ptr<Object>> evalExpressions(std::vector<std::unique_ptr<std::unique_ptr<Expression>>> exps, std::unique_ptr<Environment> env);
std::unique_ptr<Object> applyFunction(std::unique_ptr<Object> fn, std::vector<std::unique_ptr<Object>> args);
std::unique_ptr<Environment> extendFunctionEnv(std::unique_ptr<Function> fn, std::vector<std::unique_ptr<Object>>);

std::unique_ptr<Object> unwrapReturnValue(std::unique_ptr<Object> obj){
    // 转化为裸指针
    auto rv = dynamic_cast<ReturnValue*>(obj.release());
    if (rv){
        return std::move(rv->value);
    }
    return std::move(obj);
}

bool isTruthy(std::unique_ptr<Object> obj){
    if (obj == Null_t) {
        return false;
    }else if (obj == True) {
        return true;
    } else if (obj == False) {
        return false;
    } else {
        return true;
    }
}

std::unique_ptr<Error> newError(std::initializer_list<std::string> format){
    std::ostringstream oss;
    for(auto &item:format){
        oss << item;
    }
    return std::make_unique<Error>(oss.str());
}

bool isError(std::unique_ptr<Object> obj){
    if (obj != nullptr) {
        return obj->Type() == ERROR_OBJ;
    }
    return false;
}
