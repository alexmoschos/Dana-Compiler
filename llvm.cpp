#include "ast.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>


using namespace llvm;
static LLVMContext context;
static IRBuilder<> Builder(context);
static Module* module;

Value* CompileExpression(ASTExpr* expr){
    if(expr == NULL){
        return NULL;
    }
    Value *left = CompileExpression(expr->left);
    Value *right = CompileExpression(expr->right);
    switch(expr->op){
        case '+':
            return Builder.CreateBinOp(Instruction::Add,left,right,"");
            break;
        case '-':
            return Builder.CreateBinOp(Instruction::Sub,left,right,"");
            break;
        case '*':
            return Builder.CreateBinOp(Instruction::Mul,left,right,"");
            break;
        case '/':
            return Builder.CreateBinOp(Instruction::SDiv,left,right,"");
            break;
        case '&':
            return Builder.CreateAnd(left,right,"");
            break;
        case '|':
            return Builder.CreateOr(left,right,"");
            break;
        case '!':
            return Builder.CreateNot(right,"");
            break;
        case 'c':
            return ConstantInt::get(llvm::Type::getInt32Ty(context),expr->constant_val,true);
            break;
        case 'x':
            break;
        case 'b':
            break;
        case '>':
            break;
        case '<':
            break;
        case 'l':
            break;
        case 's':
            break;
        case 'e':
            break;
        case 'd':
            break;
        case 'a':
            break;
        case 'o':
            break;
        case 'n':
            break;
        case 'f':
            break;

        case 'i':
            break;
        default: exit(1);


    }
    return NULL;
}

Value* CompileStatements(ASTstmt* stmt){
    switch(stmt->type){
        case TSKIP:
            break;
        case TPC:
            break;
        case TFCALL:
            break;
        case TIF:
            break;
        case TFDEF:
            break;
        case TFDECL:
            break;
        case TEXIT:
            break;
        case TRET:
            break;
        case TCONTM:
            break;
        case TCONT:
            break;
        case TBREAKM:
            break;
        case TBREAK:
            break;
        case TDECL:
            break;
        case TLOOP:
            break;
        case TASSIGN:
            break;
    }
    return NULL;
}

int main(int argc, char const *argv[]) {
    vector<const llvm::Type*> argTypes;
    FunctionType* ftype = FunctionType::get(llvm::Type::getVoidTy(context),false);
    Function* mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);

    BasicBlock* bl = BasicBlock::Create(context,"entry",mainFunction,0);
    //ASTExpr* b = new ASTExpr('c',NULL,42,NULL,NULL);

    auto b = new ASTExpr('c',NULL,42,NULL,NULL);
    auto c = new ASTExpr('*',NULL,0,b,b);
    Value* a = CompileExpression(c);
    a->print(errs());
    //PassManager pm;
    //pm.add(createPrintModulePass(a);
//    if(a != NULL) exit(1);
    return 0;
}
