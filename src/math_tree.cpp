/**
* @file         math_tree.cpp
* @brief        Definitions of methods for ast dumps
* @author       Dvortsova Varvara BSE182 HSE
*/
#include "math_tree.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <cmath>
#include <iostream>
#define  EQUATION_EPS 1e-9

/**
* @brief      Compare a double variable with zero
* @param[in]  a  double variable
* @return     0 if a == 0 else 1
*/
static bool isZero(double a) {
    return fabs(a) < EQUATION_EPS;
}

const char* getNameOfOp(OP_TYPE type) {
    switch(type) {
        case ADD:
            return "+";
        case SUB:
            return "-";
        case DIV:
            return "/";
        case MUL:
            return "*";
        case NUM:
            return nullptr;
    }
    return nullptr;
}

char* getExpressionFromFile(char* fName, int* size) {
    assert(fName != NULL);

    FILE* file = NULL;
    file = fopen(fName, "r");
    if(!file) {
        fprintf(stderr,"Do not open = %s\n", fName);
        exit(EXIT_FAILURE);
    }

    // define size of file
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* arr = static_cast<char *>(calloc(*size, *size * sizeof(char)));
    if (arr) {
        int res = fread(arr, 1, *size, file);
        if(res != *size) {
            fprintf(stderr,"Do not read file to buffer = %s\n", fName);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
    return arr;
}

bool getDoubleNumber(char* mnemonicStart, double* number) {
    char* endPtr = NULL;
    //cast string to double
    *number = strtod(mnemonicStart, &endPtr);
    return endPtr != mnemonicStart;
}


/**
 * get tokens from string
 * @param expr
 * @param size
 * @return
 */
std::vector<Token*> getTokens(char* expr) {
    assert(expr != nullptr);
    int countOP = 0;
    std::vector<Token*> tokens;
    char* token = strtok(expr, " ");
    while( token != nullptr ) {
        printf( " %s\n", token );
        if(strncmp(token,"(",1) == 0) {
            auto elem = new Pair(OPEN);
            elem->setNumber(countOP);
            tokens.push_back(elem);
            countOP++;
        }
        else if(strcmp(token,"+") == 0) {
            auto elem = new Add();
            elem->setNumber(countOP);
            tokens.push_back(elem);
            countOP++;
        }
        else if(strcmp(token,"-") == 0) {
            auto elem = new Sub();
            elem->setNumber(countOP);
            tokens.push_back(elem);
            countOP++;
        }
        else if(strcmp(token,"/") == 0) {
            auto elem = new Div();
            elem->setNumber(countOP);
            tokens.push_back(elem);
            countOP++;
        }
        else if(strcmp(token,"*") == 0) {
            auto elem = new Mul();
            elem->setNumber(countOP);
            tokens.push_back(elem);
            countOP++;
        }
        else if(strncmp(token,")", 1) == 0) {
            auto elem = new Pair(CLOSE);
            elem->setNumber(countOP);
            tokens.push_back(elem);
            countOP++;
        }
        else {
            double num = 0;
            if(getDoubleNumber(token, &num)) {
                if(num < 0) {//todo
                    auto elem = new UnMinus();
                    elem->setNumber(countOP);
                    tokens.push_back(elem);
                    num = -num;
                    countOP++;
                }
                auto elem = new Var(num);
                elem->setNumber(countOP++);
                tokens.push_back(elem);
            }
        }
        token = strtok(nullptr, " ");
    }

    //for debug
//    std::vector<Token*>::iterator it;
//    for (it = std::begin(tokens); it != std::end(tokens) ; ++it) {
//        if(dynamic_cast<UnMinus*>(*it) != nullptr) {
//            std::cout << "unMinus\n";
//        } else if (dynamic_cast<Pair*>(*it) != nullptr) {
//            std::cout << "Pair(" << dynamic_cast<Pair*>(*it)->getType() << ")\n";
//        } else if (dynamic_cast<Add*>(*it) != nullptr) {
//            std::cout << "add\n";
//        } else if (dynamic_cast<Sub*>(*it) != nullptr) {
//            std::cout << "sub\n";
//        } else if (dynamic_cast<Mul*>(*it) != nullptr) {
//            std::cout << "mul\n";
//        } else if (dynamic_cast<Div*>(*it) != nullptr) {
//            std::cout << "div\n";
//        } else if (dynamic_cast<Var*>(*it) != nullptr) {
//            std::cout << "var(" << dynamic_cast<Var*>(*it)->getValue() << ")\n";
//        }
//    }
    return tokens;
}


Node* buildTree(std::vector<Token*>* tokens) {
    assert(tokens);
    std::stack<Token*> stTokens;
    std::stack<Node*>  stNodes;

    std::vector<Token*>::iterator it;
    for (it = std::begin(*tokens); it != std::end(*tokens) ; ++it) {
        if(dynamic_cast<UnMinus*>(*it) != nullptr) {
            stTokens.push(*it);
            continue;
        } else if (dynamic_cast<Pair*>(*it) != nullptr) {
            auto item = dynamic_cast<Pair*>(*it);
            if(item->getType() == OPEN) {
                stTokens.push(item);
                continue;
            } else {  // pair == CLOSE
                bool isOk = true;
                while (isOk) {
                    if(stTokens.empty()) {
                        std::cout << "stack is empty\n";
                        break;
                    }
                    auto token = stTokens.top();
                    if(dynamic_cast<Pair*>(token) != nullptr) {
                        auto pair = dynamic_cast<Pair*>(token);
                        if(pair->getType() == OPEN) {
                            stTokens.pop();
                            isOk = false;
                            continue;
                        }
                    }
                    Node* node =  new Node();
                    if (dynamic_cast<Add*>(token) != nullptr) {
                        node->type = ADD;
                    } else if (dynamic_cast<Sub*>(token) != nullptr) {
                        node->type = SUB;
                    } else if (dynamic_cast<Mul*>(token) != nullptr) {
                        node->type = MUL;
                    } else if (dynamic_cast<Div*>(token) != nullptr) {
                        node->type = DIV;
                    } else if (dynamic_cast<UnMinus*>(token) != nullptr) {
                        node->type = SUB;

                        Node* lChild =  new Node();
                        lChild->type = NUM;
                        lChild->value = 0;
                        node->leftChild = lChild;

                        auto* numForSub = stNodes.top();
                        stNodes.pop();
                        node->rightChild = numForSub;
                        node->index = token->getNumber();
                        stNodes.push(node);

                        stTokens.pop();//delete top

                        continue;
                    }

                    node->rightChild = stNodes.top();
                    stNodes.pop();
                    node->index = token->getNumber();

                    node->leftChild = stNodes.top();
                    stNodes.pop();

                    stNodes.push(node);

                    stTokens.pop();//delete top

                }
                continue;
            }
        } else if(dynamic_cast<Var*>(*it) != nullptr) {
            Node* nodeVar;
            nodeVar =  new Node();
            nodeVar->type = NUM;
            auto elem = dynamic_cast<Var*>(*it);
            nodeVar->value =  elem->getValue();
            nodeVar->leftChild = nullptr;
            nodeVar->rightChild = nullptr;
            nodeVar->index = elem->getNumber();
            stNodes.push(nodeVar);
        }

        else {
           if(!stTokens.empty()) {
               auto op2 = stTokens.top();
               bool isOk = true;
               while(isOk) {
                   if(op2->getPriority() >= (*it)->getPriority()) {
                       stTokens.pop();
                       Node* node =  new Node();
                       if(dynamic_cast<UnMinus*>(op2) != nullptr) {
                           node->type = SUB;

                           Node* lChild =  new Node();
                           lChild->type = NUM;
                           lChild->value = 0;
                           node->leftChild = lChild;

                           auto* numForSub = stNodes.top();
                           stNodes.pop();
                           node->rightChild = numForSub;
                           node->index = op2->getNumber();
                           stNodes.push(node);

                           if(!stTokens.empty()) {
                               op2 = stTokens.top();
                           } else isOk = false;
                           continue;
                       }
                       if (dynamic_cast<Add*>(op2) != nullptr) {
                           node->type = ADD;
                       } else if (dynamic_cast<Sub*>(op2) != nullptr) {
                           node->type = SUB;
                       } else if (dynamic_cast<Mul*>(op2) != nullptr) {
                           node->type = MUL;
                       } else if (dynamic_cast<Div*>(op2) != nullptr) {
                           node->type = DIV;
                       }

                       node->rightChild = stNodes.top();
                       stNodes.pop();

                       node->leftChild = stNodes.top();
                       stNodes.pop();
                       node->index = op2->getNumber();

                       stNodes.push(node);

                       if(!stTokens.empty()) {
                           op2 = stTokens.top();
                       } else isOk = false;

                   } else {
                       isOk = false;
                   }
               }
           }
            stTokens.push(*it);
            continue;
        }
    }

    while (!stTokens.empty()) {
        auto token = stTokens.top();
        Node* node = new Node();
        if (dynamic_cast<Add*>(token) != nullptr) {
            node->type = ADD;
        } else if (dynamic_cast<Sub*>(token) != nullptr) {
            node->type = SUB;
        } else if (dynamic_cast<Mul*>(token) != nullptr) {
            node->type = MUL;
        } else if (dynamic_cast<Div*>(token) != nullptr) {
            node->type = DIV;
        } else if (dynamic_cast<UnMinus*>(token) != nullptr) {
            node->type = SUB;

            Node* lChild =  new Node();
            lChild->type = NUM;
            lChild->value = 0;
            node->leftChild = lChild;

            auto* numForSub = stNodes.top();
            stNodes.pop();
            node->rightChild = numForSub;
            node->index = token->getNumber();

            stNodes.push(node);

            stTokens.pop();//delete top
            continue;
        }

        node->rightChild = stNodes.top();
        stNodes.pop();

        node->index = token->getNumber();

        node->leftChild = stNodes.top();
        stNodes.pop();

        stNodes.push(node);

        stTokens.pop();//delete top
    }

    for (auto & token : *tokens) {
        delete token;
    }

    auto tree = stNodes.top();
    stNodes.pop();
    return tree;
}


void gravizDump(char* outPath, Node* tree) {
    assert(outPath);
    assert(tree);
    int index = 0;
    std::ofstream myfile;
    myfile.open (outPath);
    myfile << "graph astdump {\n";
    myfile << "res [shape=doubleoctagon]" << "[label=\"result = "<< calculate(tree)<<"\"]\n";
    gravizDeepWriting(myfile, tree, &index);
    myfile << "}\n";

    myfile.close();

}
int calculate(Node* tree) {
    if(tree == nullptr)
        return 0;
    if(tree->type != NUM) {
        int a = calculate(tree->leftChild);
        int b = calculate(tree->rightChild);
        switch (tree->type) {
            case ADD:
                return a + b;
            case MUL:
                return a * b;
            case SUB:
                return a - b;
            case DIV:
                return a / b;
            case NUM:
                break;
        }
    }
    return tree->value;
}



void gravizDeepWriting(std::ofstream& myfile, Node* tree, int *index) {
    if(tree == nullptr)
        return;
    if(tree->type != NUM) {
        myfile << "_" << tree->index << "_" << tree->type << "[shape=box, color=blue]\n";
        myfile << "_" << tree->index << "_" << tree->type << "[label=\"" << getNameOfOp(tree->type) << "\"]\n";
    } else {
        return;
    }
    myfile << "_" << tree->index << "_" << tree->type << " -- ";
    if(tree->leftChild->type == NUM) {
        myfile << "_" << tree ->leftChild->index << "V" ;
        char temp[100] ="";
        if(isZero(tree->leftChild->value)) {
            sprintf(temp,"_%d0",(*index)++);
        }
        myfile << temp << "\n";
        myfile << "_" << tree->leftChild->index << "V" << temp << " [label=\"" << tree->leftChild->value << "\"]\n";

    }
    else {
        myfile << "_" << tree->leftChild->index << "_" << tree->leftChild->type << ";\n";
    }

    myfile << "_" << tree->index << "_";
    myfile << tree->type << " -- ";
    if(tree->rightChild->type == NUM) {
        myfile << "_" << tree ->rightChild->index << "V" << tree ->rightChild->value << ";\n";
        myfile << "_" << tree ->rightChild->index << "V" << tree ->rightChild->value << " [label=\"" << tree ->rightChild->value << "\"]\n";
    }
    else
        myfile << "_" << tree->rightChild->index << "_" << tree->rightChild->type << ";\n";

    gravizDeepWriting(myfile, tree->leftChild, index);
    gravizDeepWriting(myfile, tree->rightChild, index);
}




void latexDump(char* outPath, Node* tree) {
    assert(outPath);
    assert(tree);
    std::ofstream myfile;
    myfile.open (outPath);
    myfile << "\\documentclass{article}\n";
    myfile << "\\begin{document}\n";
    myfile << "$$";
    latexDeepWriting(myfile, tree);
    myfile << "$$";
    myfile << "\n";
    myfile << "\\end{document}\n";
    myfile.close();
}

void latexDeepWriting(std::ofstream& myfile, Node* root) {
    if(root == nullptr)
        return;
    if(root->type != NUM) {
        if (root->type == DIV) {
            myfile << "\\frac{";
            latexDeepWriting(myfile, root->leftChild);
            myfile << "}{";
            latexDeepWriting(myfile, root->rightChild);
            myfile << "}";
            return;
        }
        myfile << "(";
        latexDeepWriting(myfile, root->leftChild);
        myfile << getNameOfOp(root->type);
        latexDeepWriting(myfile, root->rightChild);
        myfile << ")";
    } else {
        myfile << root->value;
    }
}


