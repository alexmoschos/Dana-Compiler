#ifndef ast_h
#define ast_h
#include <string>
using namespace std;
class ASTNode{
    virtual void run();
};
class ASTExpr:ASTNode{
public:
        char op;
        string identifier;
        int constant_val;
        ASTExpr* left;
        ASTExpr* right;
        ASTExpr(char,ASTExpr*,ASTExpr*);
};

class ASTCond:ASTNode{
public:
        char op;
        ASTCond* left;
        ASTCond* right;
        ASTCond(char,ASTExpr*,ASTExpr*);
};
class ASTfdecl:ASTNode{

};
class ASTfdef:ASTfdecl{

};
class ASTstmt:ASTNode{
public:
    ASTstmt *tail;
};

#endif
