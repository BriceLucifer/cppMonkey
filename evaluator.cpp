//
// Created by 23766 on 2024/10/7.
//
#include "evaluator.h"

std::unique_ptr<Object> Eval(std::unique_ptr<Node> node, std::unique_ptr<Environment> env){;
    // get the pointer type
    const std::type_info &item_type = typeid(*node);
    // switch structure
    if (typeid(Program) == item_type) {
        return evalProgram(std::unique_ptr<Program>(dynamic_cast<Program*>(node.release())),std::move(env));
    } else if (typeid(ExpressionStatement) == item_type) {
        return Eval(std::move(node), std::move(env));
    } else if (typeid(IntegerLiteral) == item_type) {
        auto tp = dynamic_cast<IntegerLiteral*>(node.release());
        return std::make_unique<Integer>(tp->value);
    } else if (typeid(Boolean) == item_type) {
        return nativeBoolToBooleanObject(dynamic_cast<Boolean*>(node.release())->value);
    } else if (typeid(PrefixExpression) == item_type) {
        auto right = Eval(std::move(dynamic_cast<PrefixExpression*>(node.release())->Right),std::move(env));
        if (isError(std::move(right))) {
            return right;
        }
        return evalPrefixExpression(std::move(dynamic_cast<PrefixExpression*>(node.release())->Operator), std::move(right));
    } else if (typeid(InfixExpression) == item_type ) {
        auto left = Eval(std::move(dynamic_cast<InfixExpression*>(node.release())->left), std::move(env));
        if (isError(std::move(left))) {
            return left;
        }

        auto right = Eval(std::move(dynamic_cast<PrefixExpression*>(node.release())->Right),std::move(env));
        if (isError(std::move(right))) {
            return right;
        }

        return evalInfixExpression(std::move(dynamic_cast<PrefixExpression*>(node.release())->Operator),std::move(left), std::move(right));
    } else if (typeid(BlockStatement) == item_type) {
        return evalBlockStatements(std::move(std::unique_ptr<BlockStatement>(dynamic_cast<BlockStatement*>(node.release()))), std::move(env));
    } else if (typeid(IfExpression) == item_type ) {
        return evalIfExpression(std::move(std::unique_ptr<IfExpression>(dynamic_cast<IfExpression*>(node.release()))), std::move(env));
    } else if (typeid(ReturnStatement) == item_type ) {
        auto val = Eval(std::move(dynamic_cast<ReturnStatement*>(node.release())->return_value), std::move(env));
        if (isError(std::move(val))) {
            return val;
        }
        return std::make_unique<ReturnValue>(std::move(val));
    }

}
std::unique_ptr<Object> evalProgram(std::unique_ptr<Program> program, std::unique_ptr<Environment> env);
std::unique_ptr<Object> evalBlockStatements(std::unique_ptr<BlockStatement> blockStatement, std::unique_ptr<Environment> env);
std::unique_ptr<Boolean_obj> nativeBoolToBooleanObject(bool input);
std::unique_ptr<Object> evalPrefixExpression(std::string Operator, std::unique_ptr<Object> right);
std::unique_ptr<Object> evalBangOperatorExpression(std::unique_ptr<Object> right){
    // 只需要比较一下False and null -> true
    // 其余全部为 false
}
std::unique_ptr<Object> evalInfixExpression(std::string Operator, std::unique_ptr<Object> left,std::unique_ptr<Object> right){
    auto leftVal = dynamic_cast<Integer*>(left.release())->value;
    auto rightVal = dynamic_cast<Integer*>(right.release())->value;

    if (Operator == "+") {
        return std::make_unique<Integer>(leftVal + rightVal);
    } else if (Operator == "-") {
        return std::make_unique<Integer>(leftVal - rightVal);
    } else if (Operator == "*") {
        return std::make_unique<Integer>(leftVal * rightVal);
    } else if (Operator == "/") {
        return std::make_unique<Integer>( leftVal / rightVal);

    } else if (Operator == "<") {
        return nativeBoolToBooleanObject(leftVal < rightVal);
    } else if (Operator == ">") {
        return nativeBoolToBooleanObject(leftVal > rightVal);
    } else if (Operator == "==") {
        return nativeBoolToBooleanObject(leftVal == rightVal);
    } else if (Operator == "!=") {
        return nativeBoolToBooleanObject(leftVal != rightVal);
    } else {
        return newError({"Unknown operator: ",left->Type(), Operator, right->Type()});
    }
}

std::unique_ptr<Object> evalMinusPrefixOperatorExpression(std::unique_ptr<Object> right) {
    auto Type_name = right->Type();
    if (Type_name != INTEGER_OBJ) {
        return newError({"unknown operator: -",Type_name});
    }

    // 尝试转换为 Integer 类型
    auto* int_ptr = dynamic_cast<Integer*>(right.get());
    if (!int_ptr) {
        return newError({"type assertion failed: expected Integer but got ", Type_name});
    }

    // 获取值并创建新对象
    auto value = int_ptr->value;
    return std::make_unique<Integer>(-value);
}

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
