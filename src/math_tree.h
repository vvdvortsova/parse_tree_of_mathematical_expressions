/**
* @file         math_tree.h
* @author       Dvortsova Varvara BSE182 HSE
*/
#ifndef PARSE_TREE_OF_MATHEMATICAL_EXPRESSIONS_MATH_TREE_H
#define PARSE_TREE_OF_MATHEMATICAL_EXPRESSIONS_MATH_TREE_H

#include <vector>
#include <fstream>

enum OP_TYPE {
    ADD = 0,
    SUB = 1,
    DIV = 2,
    MUL = 3,
    NUM = 4
};


enum OP_PAIR {
    OPEN,
    CLOSE
};

/**
 * struct is describing of the node of the ast tree
 */
typedef struct Node {
    OP_TYPE type;
    double value;
    double index;
    Node* leftChild;
    Node* rightChild;
}Node;

/**
 * Super class for all tokens
 */
class Token {
    int num{};
    int priority = 0;
public:
    explicit Token(int _priority):priority(_priority){};
    virtual ~Token()= default;
    void setNumber(int val){ this->num = val;}
    int getNumber() const { return num;}
    int getPriority() const { return priority;};
};

class Var:     public Token {
    double value;
public:
    explicit Var(double val): Token(0), value(val) {}
    double getValue() const { return value;}
};
class UnMinus: public Token {
public:
    explicit UnMinus(): Token(2){};
};
class Mul:     public Token {
public:
    explicit Mul(): Token(2){};
};
class Add:     public Token {
public:
    explicit Add(): Token(1){};
};
class Sub:     public Token {
public:
    explicit Sub(): Token(1){};
};
class Div:     public Token {
public:
    explicit Div(): Token(2){}
};
class Pair:    public Token {
    OP_PAIR pType;
public:
    Pair(OP_PAIR _type): Token(0), pType(_type) {}
    OP_PAIR getType() const { return pType;}
};


char* getExpressionFromFile(char* fName, int* size);


/**
 * Method splits line into tokens.
 * It is important to use space when you write brackets.
 * Example: ( 1 + 2 ) not (1+2) or (1 + 2)
 * Do'not worry I'l fixed it as soon as possible.
 * @param expr
 * @param size
 * @return
 */
std::vector<Token*> getTokens(char* expr);

/**
 * I use the Shunting-Yard algorithm and build tree parallel
 * @param tokens
 * @return
 */
Node* buildTree(std::vector<Token*>* tokens);

/**
 * Dumps tree into graviz format
 * @param outPath
 * @param tree
 */
void gravizDump(char* outPath, Node* tree);

/**
 * Helping method to walk the tree when gravizDump is doing
 * @param myfile
 * @param tree
 * @param index
 */
void gravizDeepWriting(std::ofstream& myfile, Node* tree, int* index);

/**
 * Dumps tree into latex format
 * @param outPath
 * @param tree
 */
void latexDump(char* outPath, Node* tree);

/**
 * Helping method to walk the tree when latexDump is doing
 * @param myfile
 * @param root
 */
void latexDeepWriting(std::ofstream& myfile, Node* root);

const char* getNameOfOp(OP_TYPE type);

/**
 * Calculate the value of expression using tree
 * @param tree
 * @return
 */
int calculate(Node* tree);

#endif //PARSE_TREE_OF_MATHEMATICAL_EXPRESSIONS_MATH_TREE_H
