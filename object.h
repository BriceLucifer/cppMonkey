//
// Created by 23766 on 2024/10/6.
//

#ifndef CPPMONKEY_OBJECT_H
#define CPPMONKEY_OBJECT_H

#include <string>
#include <memory>
#include <vector>
#include "ast.h"

#define INTEGER_OBJ "INTEGER"
#define BOOLEAN_OBJ "BOOLEAN"
#define NULL_OBJ "NULL"
#define RETURN_VALUE_OBJ "RETURN_VALUE"
#define ERROR_OBJ "ERROR"
#define FUNCTION_OBJ "FUNCTION"

using ObjectType = std::string;

class Object {
public:
    virtual ObjectType Type() const = 0;
    virtual std::string Inspect() const = 0;
    Object() = default;
    ~Object() = default;
};
class Integer : public Object {
public:
    int64_t value;
    Integer(int64_t val);
    ~Integer();
    ObjectType Type() const override {
        return INTEGER_OBJ;
    }
    std::string Inspect() const override {
        return std::to_string(value);
    }
};

class Boolean_obj : public Object{
public:
    bool value;
    Boolean_obj(bool value);
    ~Boolean_obj();
    ObjectType Type() const override {
        return BOOLEAN_OBJ;
    }
    std::string Inspect() const override {
        return std::to_string(value);
    }
};

class Null : public Object {
public:
    Null() = default ;
    ~Null() = default ;
    ObjectType Type() const override {
        return NULL_OBJ;
    }
    std::string Inspect() const override {
        return "null";
    }
};

class ReturnValue : public Object {
public:
    std::unique_ptr<Object> value;
    ReturnValue(std::unique_ptr<Object> parm);
    ~ReturnValue();
    ObjectType Type() const override {
        return RETURN_VALUE_OBJ;
    }
    std::string Inspect() const override {
        return value->Inspect();
    }
};

class Error : Object {
public:
    explicit Error(std::string msg);
    ~Error();
    std::string message;
    ObjectType Type() const override {
        return ERROR_OBJ;
    }
    std::string Inspect() const override {
        return "ERROR: " + message;
    }
};

class Environment {
public:
    std::map<std::string,std::unique_ptr<Object>> store;
    std::unique_ptr<Environment> outer;
    explicit Environment(std::unique_ptr<Environment> new_env);
    ~Environment();
    // get获取
    std::pair<std::unique_ptr<Object>,bool> Get(std::string name);
    // set设置
    void Set(std::string name, std::unique_ptr<Object> val);
};


class Function : public Object {
public:
    std::vector<std::unique_ptr<Identifier>> parameters;
    std::unique_ptr<BlockStatement> body;
    std::unique_ptr<Environment> env;
    Function();
    ~Function();
    ObjectType Type() const override {
        return FUNCTION_OBJ;
    }
    std::string Inspect() const override{
        std::ostringstream oss;
        std::vector<std::string> params;
        for(auto &item:parameters){
            params.push_back(item->String());
        }
        oss << "fn";
        oss << "(";
        std::string result;
        for (const auto& str : params) {
            result += str;
        }
        oss << result;
        oss << ") {\n";
        oss << body->String();
        oss << "\n}";
        return oss.str();
    }
};
#endif //CPPMONKEY_OBJECT_H
