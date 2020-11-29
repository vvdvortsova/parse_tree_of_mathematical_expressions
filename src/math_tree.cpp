#include "math_tree.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stack>
#include <cctype>
#include <cmath>
#include <iostream>

#define  EQUATION_EPS 1e-9

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
std::vector<Token*> getTokens(char* expr, int size) {
    assert(expr != nullptr);

    std::vector<Token*> tokens;
    char* token = strtok(expr, " ");
    while( token != nullptr ) {
        printf( " %s\n", token );

        if(strcmp(token,"(") == 0) tokens.push_back(new Pair(OPEN));
        else if(strcmp(token,"+") == 0) tokens.push_back(new Add());
        else if(strcmp(token,"-") == 0) tokens.push_back(new Sub());
        else if(strcmp(token,"/") == 0) tokens.push_back(new Div());
        else if(strcmp(token,"*") == 0) tokens.push_back(new Mul());
        else if(strcmp(token,")") == 0) tokens.push_back(new Pair(CLOSE));
        else {
            double num = 0;
            if(getDoubleNumber(token, &num)) {
                if(num < 0) {//todo
                    tokens.push_back((Token*)new UnMinus());
                    num = -num;
                }
                tokens.push_back((Token*)new Var(num));
            }
        }

        token = strtok(nullptr, " ");
    }

    std::vector<Token*>::iterator it;
    for (it = std::begin(tokens); it != std::end(tokens) ; ++it) {
        if(dynamic_cast<UnMinus*>(*it) != nullptr) {
            std::cout << "unMinus\n";
        } else if (dynamic_cast<Pair*>(*it) != nullptr) {
            std::cout << "Pair(" << dynamic_cast<Pair*>(*it)->getType() << ")\n";
        } else if (dynamic_cast<Add*>(*it) != nullptr) {
            std::cout << "add\n";
        } else if (dynamic_cast<Sub*>(*it) != nullptr) {
            std::cout << "sub\n";
        } else if (dynamic_cast<Mul*>(*it) != nullptr) {
            std::cout << "mul\n";
        } else if (dynamic_cast<Div*>(*it) != nullptr) {
            std::cout << "div\n";
        } else if (dynamic_cast<Var*>(*it) != nullptr) {
            std::cout << "var(" << dynamic_cast<Var*>(*it)->getValue() << ")\n";
        }
    }
    return tokens;
}

/**
 * Shunting-Yard
 * @param tokens
 * @return
 */
Node* buildTree(std::vector<Token*> tokens) {
    std::stack<Token*> stTokens;
    std::stack<Node*>  stNodes;

    std::vector<Token*>::iterator it;
    for (it = std::begin(tokens); it != std::end(tokens) ; ++it) {
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
                    if(stTokens.empty()){
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
                    //toDo
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
                        stNodes.push(node);

                        stTokens.pop();//delete top

                        continue;
                    }

                    node->rightChild = stNodes.top();
                    stNodes.pop();

                    node->leftChild = stNodes.top();
                    stNodes.pop();

                    stNodes.push(node);

                    stTokens.pop();//delete top

                }
                continue;
            }
        } else if (dynamic_cast<Add*>(*it) != nullptr || dynamic_cast<Sub*>(*it) != nullptr) {
           if(!stTokens.empty()){
               auto op2 = stTokens.top();
               if (dynamic_cast<Sub*>(op2) != nullptr || dynamic_cast<Add*>(op2) != nullptr
                   || dynamic_cast<Mul*>(op2) != nullptr || dynamic_cast<Div*>(op2) != nullptr
                   || dynamic_cast<UnMinus*>(op2) != nullptr) {
                   stTokens.pop();
                   Node* node =  new Node();

                   //todo check is correct?
                   if(dynamic_cast<UnMinus*>(op2) != nullptr) {
                       node->type = SUB;

                       Node* lChild =  new Node();
                       lChild->type = NUM;
                       lChild->value = 0;
                       node->leftChild = lChild;

                       auto* numForSub = stNodes.top();
                       stNodes.pop();
                       node->rightChild = numForSub;
                       stNodes.push(node);

                       //push op1
                       stTokens.push(*it);
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

                   stNodes.push(node);

                   //push op1
                   stTokens.push(*it);
                   continue;
               }
           }

            stTokens.push(*it);
            continue;

        } else if (dynamic_cast<Mul*>(*it) != nullptr || dynamic_cast<Div*>(*it) != nullptr) {
            if(!stTokens.empty()) {
                auto op2 = dynamic_cast<Token*>(stTokens.top());
                if (dynamic_cast<Mul*>(op2) != nullptr || dynamic_cast<Div*>(op2) != nullptr) {
                    stTokens.pop();
                    Node* node =  new Node();
                    if (dynamic_cast<Mul*>(op2) != nullptr) {
                        node->type = MUL;
                    } else if (dynamic_cast<Div*>(op2) != nullptr) {
                        node->type = DIV;
                    }

                    node->rightChild = stNodes.top();
                    stNodes.pop();
                    node->leftChild  = stNodes.top();
                    stNodes.pop();

                    stNodes.push(node);
                    stTokens.push(*it);
                    continue;
                }
                stTokens.push(*it);
                continue;
            }
             else {
                stTokens.push(*it);
                continue;
            }

        }
        else if (dynamic_cast<Var*>(*it) != nullptr) {
            Node* nodeVar;
            nodeVar =  new Node();

            nodeVar->type = NUM;
            auto elem = dynamic_cast<Var*>(*it);
            nodeVar->value =  elem->getValue();
            nodeVar->leftChild = nullptr;
            nodeVar->rightChild = nullptr;
            stNodes.push(nodeVar);
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
            stNodes.push(node);

            stTokens.pop();//delete top

            token = stTokens.top();//get next
            continue;
        }

        node->rightChild = stNodes.top();
        stNodes.pop();

        node->leftChild = stNodes.top();
        stNodes.pop();

        stNodes.push(node);

        stTokens.pop();//delete top

    }


    //toDo free tokens
    auto tree = stNodes.top();
    stNodes.pop();
    return tree;

}


/**
* @brief      Compare a double variable with zero
* @param[in]  a  double variable
* @return     0 if a == 0 else 1
*/
static bool isZero(double a){
    return fabs(a) < EQUATION_EPS;
}