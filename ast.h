#ifndef ast_h
#define ast_h
#include <string>
using namespace std;
class ASTNode{
    virtual void run();
};
class ASTExpr:ASTNode{
        char op;
        ASTExpr* left;
        ASTExpr* right;
        ASTExpr(char,ASTExpr*,ASTExpr*);
};

class ASTCond:ASTNode{
        char op;
};

#endif
