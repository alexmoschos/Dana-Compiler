#ifndef ast_h
#define ast_h
#include <string>
using namespace std;
enum stmt_type {
    TSKIP,
    TPC,
    TIF,
    TFDEF,
    TFDECL,
    TEXIT,
    TRET,
    TFCALL,
    TCONTM,
    TCONT,
    TBREAKM,
    TBREAK,
    TDECL,
    TASSIGN,
    TLOOP
};
class ASTNode{
    int he;
    //virtual void run();
};
class ASTExpr:ASTNode{
public:
        char op;
        string identifier;
        int constant_val;
        ASTExpr* left;
        ASTExpr* right;
        ASTExpr(char,string,int,ASTExpr*,ASTExpr*);
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
