cmake_minimum_required(VERSION 3.27)
project(cppMonkey)

set(CMAKE_CXX_STANDARD 17)

add_executable(cppMonkey main.cpp
        token.h
        token.cpp
        lexer.h
        lexer.cpp
        repl.h
        repl.cpp
        ast.h
        object.h
        environment.cpp
        environment.h
        parser.h
        parser.cpp
        ast.cpp
        evaluator.h
        evaluator.cpp)

