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

typedef struct Node {
    OP_TYPE type;
    double value;
    double number;
    Node* leftChild;
    Node* rightChild;
}Node;

class Token {//make it abstract??
    int num;
public:
    Token()= default;
    virtual ~Token()= default;
    void setNumber(int val){ this->num = val;}
    int getNumber() const { return num;}
};

class Var:     public Token {
    double value;
public:
    explicit Var(double val): value(val) {}
    double getValue() const { return value;}
};
class UnMinus: public Token {};
class Mul:     public Token {
    int priority = 2;
public:
    int getPriority() const { return priority;};
};
class Add:     public Token {
    int priority = 1;
public:
    int getPriority() const { return priority;};
};
class Sub:     public Token {
    int priority = 1;
public:
    int getPriority() const { return priority;};
};
class Div:     public Token {
    int priority = 1;
public:
    int getPriority() const { return priority;};
};
class Pair:    public Token {
    OP_PAIR pType;
public:
    Pair(OP_PAIR _type): pType(_type){}
    OP_PAIR getType() const { return pType;}
};


char* getExpressionFromFile(char* fName, int* size);
void gravizDeepWriting(std::ofstream& myfile, Node* tree, int* index);

std::vector<Token*> getTokens(char* expr, int size);

Node* buildTree(std::vector<Token*>* tokens);

void gravizDump(char* outPath, Node* tree);

void latexDump(char* outPath, Node* tree);

void latexDeepWriting(std::ofstream& myfile, Node* root);

const char* getNameOfOp(OP_TYPE type);

int calculate(Node* tree);

#endif //PARSE_TREE_OF_MATHEMATICAL_EXPRESSIONS_MATH_TREE_H
