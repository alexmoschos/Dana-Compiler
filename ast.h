#ifndef ast_h
#define ast_h
#include <string>
#include <vector>
#include <iostream>
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
class ASTlval;
class ASTNode{
    int he;
    //virtual void run();
};
class ASTExpr:ASTNode{
public:
        char op;
        ASTlval* operand;
        int constant_val;
        ASTExpr* left;
        ASTExpr* right;
        ASTExpr(char,ASTlval*,int,ASTExpr*,ASTExpr*);
};


class ASTfdecl:ASTNode{

};
class ASTfdef:ASTfdecl{

};
class ASTstmt:ASTNode{
public:
    ASTstmt *tail;
};
class ASTlval:ASTNode{
public:
    ASTlval(bool,string);
    string identifier;
    bool constant;
    std::vector<ASTExpr*> *indices;
    void print(){
        cout << "Printing bracket lists" << endl;
        for(auto i : *indices){
            cout << i->constant_val << " ";
        }
        cout << endl;
    }
};
#endif
