#include <iostream>
#include <cstring>
#include "src/math_tree.h"
void printtreenode(Node *root);

int main() {
    std::cout << "Hello, World!" << std::endl;
    int size = 0;
    char* expr = getExpressionFromFile("../test1.txt", &size);
    std::cout << expr << "\n";

    Node* node = (Node*)calloc(1, sizeof(node));

    auto a = getTokens(expr, size);
    auto tree = buildTree(a);
    printtreenode(tree);
//    printf( " %s\n", token ); //printing the token
    return 0;
}

void printtreenode(Node *root)
{
    if(root == NULL)
        return;

    printtreenode(root -> leftChild);
    printf("%d\n",root -> type);
    printtreenode(root ->rightChild);
}