#include "ast.h"
#include <iostream>
#include <string>
using namespace std;
ASTstmt::ASTstmt(stmt_type type, ASTstmt *tail){
    this->type = type;
    this->tail = tail;
}
ASTExpr::ASTExpr(char op,ASTlval* operand,int constval,ASTExpr *left,ASTExpr *right){
    //cout << "hello world" << endl;
    this->op = op;
    this->operand=operand;
    this->constant_val=constval;
    this->left = left;
    this->right = right;

}

ASTlval::ASTlval(bool constant,string identifier){
    this->constant = constant;
    this->identifier = identifier;
    this->indices = new vector<ASTExpr*>();
}
