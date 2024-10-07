//
// Created by 23766 on 2024/10/7.
//

#include "object.h"

Integer::Integer(int64_t val) : value(val) {}
Integer::~Integer()  = default;

Boolean_obj::Boolean_obj(bool value) : value(value) {}
Boolean_obj::~Boolean_obj() = default ;

ReturnValue::ReturnValue(std::unique_ptr<Object> parm) : value(std::move(parm)){}
ReturnValue::~ReturnValue() = default;

Error::Error(std::string msg) : message(msg){}
Error::~Error() = default;

Environment::Environment(std::unique_ptr<Environment> new_env):store(std::map<std::string,std::unique_ptr<Object>>()),outer(std::move(new_env)) {}
Environment::~Environment() = default;
std::pair<std::unique_ptr<Object>, bool> Environment::Get(std::string name) {
    auto item = store.find(name);
    if (item != store.end()){
        return {std::move(item->second), true};
    }
    if ( outer != nullptr ) {
        return outer->Get(name);
    }
    return {nullptr, false};
}

void Environment::Set(std::string name, std::unique_ptr<Object> val) {
    store[name] = std::move(val);
}

Function::Function() = default;
Function::~Function() = default;