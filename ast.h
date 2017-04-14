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
enum ptype{
    RINT,
    RBYTE,
    INTEG,
    BYT,
    INTA,
    BYTA
};
class ASTlval;
class ASTstmt;
class ASTExpr;
class ASTparam;
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

class ASTheader:ASTNode{
public:
    ASTheader(int,ASTparam*);
    ASTparam *paramlist;
    int type;
};

class ASTfdef:ASTNode{
public:
    ASTfdef(ASTheader*,ASTstmt*);
    ASTheader *header;
    ASTstmt *body;
};
class ASTstmt:ASTNode{
public:
    ASTstmt(stmt_type,ASTstmt*);
    stmt_type type;
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
class ASTparam:ASTNode{
public:
    ASTparam(string,ptype,ASTparam*);
    string identifier;
    ptype p;
    std::vector<int> *indices;
    ASTparam *next;
};
#endif
