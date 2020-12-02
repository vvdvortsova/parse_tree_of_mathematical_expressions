cmake_minimum_required(VERSION 3.15)
project(parse_tree_of_mathematical_expressions)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Wall -Wextra -pedantic -Wfloat-equal -fno-stack-protector)


add_executable(parse_tree_of_mathematical_expressions
        main.cpp
        src/math_tree.cpp
        src/math_tree.h
        )

add_executable(treeExpr
        src/treeExpr.cpp
        src/math_tree.cpp
        src/math_tree.h)