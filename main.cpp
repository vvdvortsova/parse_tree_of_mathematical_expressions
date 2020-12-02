#include <iostream>
#include "src/math_tree.h"

int main() {
    int size = 0;
    //TODO fix empty file
    //TODO add checking on arg and improve token parsing
    char* expr = getExpressionFromFile("gr1.txt", &size);
    std::cout << expr << "\n";

    auto a = getTokens(expr);
    auto tree = buildTree(&a);
    gravizDump("gr1.dot", tree);

    std::cout << "res = " << calculate(tree);
    latexDump("gr1.tex", tree);
    return 0;
}

