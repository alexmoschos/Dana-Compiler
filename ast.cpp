#include "ast.h"
#include <iostream>
#include <string>
using namespace std;
ASTExpr::ASTExpr(char op,string id,int constval,ASTExpr *left,ASTExpr *right){
    cout << "hello world" << endl;
    this->op = op;
    this->identifier=id;
    this->constant_val=constval;
    this->left = left;
    this->right = right;

}
