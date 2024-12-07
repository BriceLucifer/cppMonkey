//
// Created by 23766 on 2024/10/6.
//

#ifndef CPPMONKEY_REPL_H
#define CPPMONKEY_REPL_H

#include "lexer.h"
#include "parser.h"

class Repl {
public:
    Repl();
    static void start();
};

std::string printParseErrors(std::vector<std::string> errors);

#endif //CPPMONKEY_REPL_H
