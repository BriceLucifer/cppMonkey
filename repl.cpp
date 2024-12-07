//
// Created by 23766 on 2024/10/6.
//

#include "repl.h"
#include <iostream>
#include <memory>

Repl::Repl() = default;

void Repl::start() {
    std::cout << "Welcome to the simple Interpreter in C++. \nEnter expressions or commands.\n";
    while (true) {
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit")
            break;

        auto lexer = std::make_unique<Lexer>(input);
        // Token tok;
        Parser parser(std::move(lexer));
        auto p = parser.ParseProgram();
        // do {
            // tok = lexer.NextToken();
            //if (tok.type!= TokenType::EOF_) {
                // std::cout << "Token: " << tokenTypeToString(tok.type) << " Literal: " << tok.literal << std::endl;
            //}
        // } while (tok.type != TokenType::EOF_);
        if (!parser.Errors().empty()){
            std::cout << printParseErrors(parser.Errors()) ;
            continue;
        }
        std::cout << p->String();
        std::cout << "\n";
    }
}

std::string printParseErrors(std::vector<std::string> errors){
    std::ostringstream oss;
    for (auto &s:errors){
        oss << s << "\n";
    }
    return oss.str();
}
