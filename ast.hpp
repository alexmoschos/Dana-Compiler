#ifndef ast_h
#define ast_h
#include <string>
using namespace std;
typedef enum {
  AST_empty,
  AST_seq,
  AST_print,
  AST_let,
  AST_for,
  AST_if,
  AST_num,
  AST_id,
  AST_op
} ast_enum;

class astNode{
public:
	ast_enum type;
	astNode *left, *right;
	string name;
	int number;
	astNode(ast_enum type,astNode *left,astNode *right,string name,int number){
		this->type = type;
		this->left = left;
		this->right = right;
		this->name = name;
		this->number = number;
	}
};


#endif