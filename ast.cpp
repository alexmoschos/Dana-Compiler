#include <string>
#include <iostream>
using namespace std;
class astNode{

};
class exprNode: astNode{
public:
	char op;
	exprNode *left,*right;
	exprNode(char op,exprNode *left, exprNode *right){
		this->op = op;
		this->left = left;
		this->right = right;
	}
};

int main(int argc, char const *argv[])
{
	cout << "hello world" << endl;
	return 0;
}
