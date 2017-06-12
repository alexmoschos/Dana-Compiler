#include "ast.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Pass.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/raw_ostream.h>
#include <iostream>

using namespace llvm;
static LLVMContext context;
static IRBuilder<> Builder(context);
static Module* mod;

Value* CompileExpression(ASTExpr* expr){
    if(expr == NULL){
        return NULL;
    }
    Value *left = CompileExpression(expr->left);
    Value *right = CompileExpression(expr->right);
    switch(expr->op){
        case '+':
            return Builder.CreateBinOp(Instruction::Add,left,right,"");
        case '-':
            return Builder.CreateBinOp(Instruction::Sub,left,right,"");

        case '*':
            return Builder.CreateBinOp(Instruction::Mul,left,right,"");
        case '/':
            return Builder.CreateBinOp(Instruction::SDiv,left,right,"");
        case '&':
            return Builder.CreateAnd(left,right,"");
        case '|':
            return Builder.CreateOr(left,right,"");
        case '!':
            return Builder.CreateNot(right,"");
        case 'c':
            return ConstantInt::get(llvm::Type::getInt32Ty(context),expr->constant_val,true);
        case 'x':
            return ConstantInt::get(llvm::Type::getInt8Ty(context),expr->constant_val,true);
        case 'b':
            return ConstantInt::get(llvm::Type::getInt8Ty(context),expr->constant_val,true);
        case '>':
            return Builder.CreateICmpSLT(left,right,"");
        case '<':
            return Builder.CreateICmpSGT(left,right,"");
        case 'l':
            return Builder.CreateICmpSGE(left,right,"");
        case 's':
            return Builder.CreateICmpSLE(left,right,"");
        case 'e':
            return Builder.CreateICmpEQ(left,right,"");
        case 'd':
            return Builder.CreateICmpNE(left,right,"");
        case 'a':
            return Builder.CreateAnd(left,right,"");
        case 'o':
            return Builder.CreateOr(left,right,"");
        case 'n':
            return Builder.CreateNot(right,"");
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
    mod = new Module("arx.ll", getGlobalContext());
    mod->setDataLayout("");
    mod->setTargetTriple("x86_64-pc-linux-gnu");

    vector<const llvm::Type*> argTypes;
    FunctionType* ftype = FunctionType::get(llvm::Type::getVoidTy(context),false);
    //Function* mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);
    Function* mainFunction = Function::Create(
     /*Type=*/ftype,
     /*Linkage=*/GlobalValue::ExternalLinkage,
     /*Name=*/"main", mod);
    //mainFunction->setCallingConv(CallingConv::C);
    BasicBlock* bl = BasicBlock::Create(context,"entry",mainFunction,0);
    //ASTExpr* b = new ASTExpr('c',NULL,42,NULL,NULL);
    //Builder.CreateBr(bl);
    Builder.SetInsertPoint(bl);
    auto b = new ASTExpr('c',NULL,42,NULL,NULL);
    auto d = new ASTExpr('x',NULL,12,NULL,NULL);
    //d = new ASTExpr('e',NULL,0,b,d);
    auto c = new ASTExpr('*',NULL,0,b,d);
    Value* a = CompileExpression(c);
    Builder.CreateRet(a);
    //a->print(errs());
    legacy::PassManager pm;
    pm.add(createPrintModulePass(outs()));
    pm.run(*mod);
    std::cout << "Hello world";
//    if(a != NULL) exit(1);
    return 0;
}
