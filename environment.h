//
// Created by 23766 on 2024/10/6.
//

#ifndef CPPMONKEY_ENVIRONMENT_H
#define CPPMONKEY_ENVIRONMENT_H

#include <memory>
#include <map>
#include <string>
#include "object.h"

class Environment {
    std::map<std::string,std::unique_ptr<Object>> store;
    std::unique_ptr<Environment> outer;
    Environment();
    ~Environment();
    std::pair<std::unique_ptr<Object>,bool> Get(std::string name);
    std::unique_ptr<Object> Set(std::string name, std::unique_ptr<Object> val);
};

#endif //CPPMONKEY_ENVIRONMENT_H
