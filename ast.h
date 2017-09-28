#ifndef ast_h
#define ast_h
#include "symbol.h"
#include <iostream>
#include <string>
#include <vector>
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
class ASTstmt;
class ASTif;
class ASTExpr;
class ASTparam;
class ASTfcall;

class ASTNode {
  public:
    int he;
    // ASTNode(int a) { he = a ;}
    // virtual void run() = 0;
};
class ASTExpr : ASTNode {
  public:
    char op;
    ASTlval *operand;
    int constant_val;
    ASTExpr *left;
    ASTExpr *right;
    ASTfcall *f = NULL;
    ASTExpr(char, ASTlval *, int, ASTExpr *, ASTExpr *);
};

class ASTheader : ASTNode {
  public:
    ASTheader(Type, ASTparam *, string);
    ASTparam *paramlist;
    string identifier;
    Type type;
};

class ASTfdef : ASTNode {
  public:
    ASTfdef(ASTheader *, ASTstmt *);
    ASTheader *header;
    ASTstmt *body;
};

class ASTstmt : ASTNode {
  public:
    ASTstmt(stmt_type, ASTstmt *, ASTstmt *, string);

    stmt_type type;
    string label;
    ASTlval *lvalue;
    ASTExpr *expr;
    ASTif *ifnode;
    vector<string> *identifiers;
    ASTstmt *body;
    ASTstmt *tail;
    ASTfdef *def;
    Type t;
};

class ASTlval : ASTNode {
  public:
    int byRef = 0;
    ASTlval(bool, string);
    string identifier;
    bool constant;
    int nesting_diff = 0; // these are the starting values and may change
    int offset = 0;
    std::vector<ASTExpr *> *indices;
    void print() {
        cout << "Printing bracket lists" << endl;
        for (auto i : *indices) {
            cout << i->constant_val << " ";
        }
        cout << endl;
    }
};

class ASTparam : ASTNode {
  public:
    ASTparam(vector<string> *, Type, ASTparam *);
    vector<string> *identifiers;
    Type p;
    bool byref;
    ASTparam *next;
};

class ASTfcall : ASTNode {
  public:
    ASTfcall(string);
    string identifier;
    vector<ASTExpr *> *parameters;
    int nesting_diff; // these are the starting values and may change
};

class ASTif {
  public:
    ASTif(ASTExpr *, ASTstmt *);
    ASTExpr *condition;
    ASTstmt *body;
    ASTif *tail;
};
#endif
