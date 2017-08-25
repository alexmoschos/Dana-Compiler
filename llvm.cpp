extern "C"{
    #include "symbol.h"
}
#include "llvm.h"
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
#include <vector>

llvm::Value* CompileStatements(ASTstmt* stmt);
llvm::Value* CompileFunction(ASTfdef*);
llvm::Value* CompileExpression(ASTExpr*);
llvm::AllocaInst* currentAlloca;
static llvm::LLVMContext context;
static llvm::IRBuilder<> Builder(context);
static llvm::Module *mod;
llvm::StructType *currentFrame;
llvm::Type* translate(Type a){
    if(equalType(a,typeInteger))
        return llvm::Type::getInt16Ty(context);
    if(equalType(a,typeChar))
        return llvm::Type::getInt8Ty(context);
    if(equalType(a,typeVoid))
        return llvm::Type::getVoidTy(context);
    //gennaia prospatheia gia arraytype,polla douleuoun out of the box
    //return llvm::ArrayType::get(llvm::Type::getInt16Ty(context), 10);
    //we need to do arraytype
    //iteratively create the type via the indices
    //just a pointer for IArray
    cout << "Type that i dont know yet.Probably array or byref" << endl; 
    printType(a);
    return llvm::Type::getVoidTy(context);
    //exit(-1);


}

using namespace llvm;
ASTstmt* push_decl(ASTstmt *decl, std::vector<llvm::Type*>& fields){
    if(decl == NULL) return NULL;
    switch(decl->type){
    case TFDEF:
        CompileFunction(decl->def);
        break;
    case TFDECL:
        break;
    case TDECL:
        for(auto i : *decl->identifiers){
            fields.push_back(translate(decl->t));
        }
        break;
    default:
        return decl;
    }
    return push_decl(decl->tail,fields);

}
Value* CompileFunction(ASTfdef *func){
    StructType *old = currentFrame;
    if(func == NULL) return NULL;
    StructType *frame = mod->getTypeByName("struct."+func->header->identifier);
    if (!frame) {
        frame = StructType::create(mod->getContext(), "struct."+func->header->identifier);
    }
    std::vector<llvm::Type*>frame_fields;
    frame_fields.push_back(PointerType::get(currentFrame, 0));
    //frame_fields.push_back(IntegerType::get(mod->getContext(), 16));
    std::vector<llvm::Type*> params;
    PointerType* frame_ptr = PointerType::get(currentFrame, 0);
    params.push_back(frame_ptr);
    currentFrame = frame;
    //do the parameters!
 
    for(auto p = func->header->paramlist; p != NULL; p = p->next){
        int numparams = p->identifiers->size();
        for(int j = 0; j < numparams; ++j){
            frame_fields.push_back(translate(p->p));
            params.push_back(translate(p->p));
        }
    } 
    //do the local variables
    ASTstmt* first = push_decl(func->body,frame_fields);
    if (frame->isOpaque()) {
        frame->setBody(frame_fields, /*isPacked=*/false);
    }

    //done with definitions start 
    vector<const llvm::Type*> argTypes;
    FunctionType* ftype = FunctionType::get(translate(func->header->type),params,false);
    Function* function = Function::Create(
     /*Type=*/ftype,
     /*Linkage=*/GlobalValue::ExternalLinkage,
     /*Name=*/func->header->identifier, mod);
    
    Function::arg_iterator arg_it = function->arg_begin();
    arg_it->setName("link");
    arg_it++;

    for(auto p = func->header->paramlist; p != NULL; p = p->next){
        for(auto id : *p->identifiers){
            arg_it->setName(id);
            arg_it++;
        }
    } 
    BasicBlock* bl = BasicBlock::Create(context,"",function,0);
    Builder.SetInsertPoint(bl);
    currentAlloca = Builder.CreateAlloca(frame, 0, "");
    unsigned int j = 0;
    arg_it = function->arg_begin();
    Value* current = &(*arg_it);
    std::vector<llvm::Value *> values;
    llvm::APInt zero(32, 0);
    values.push_back(llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(context), zero));
    values.push_back(llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(context), zero));
    auto gep = Builder.CreateGEP(currentAlloca,values,"");
    current = &(*arg_it);
    Builder.CreateStore(current,gep);
    arg_it++;
    std::cout << params.size() << std::endl;

    for(j=1; j < params.size();++j,++arg_it){
        std::vector<llvm::Value *> values;
        llvm::APInt zero(32, 0);
        llvm::APInt offset(32,j);
        values.push_back(llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(context), zero));
        values.push_back(llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(context), offset));
        auto gep = Builder.CreateGEP(currentAlloca,values,"");
        current = &(*arg_it);
        Builder.CreateStore(current,gep);
    }



    CompileStatements(first);
    currentFrame = old;
    return NULL;
    
}
Value* CompileExpression(ASTExpr* expr){
    //std::cout << "I AM DOING AN EXPRESSION" << std::endl;
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
            return ConstantInt::get(llvm::Type::getInt16Ty(context),expr->constant_val,true);
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
            //implement a function just doing calls, to use everywhere else
            break;

        case 'i':{
	    //this is the case where nested_diff == 0
            //to generalize add #nested_diff geps traversing the 
            //first element of the struct 0 offset
            std::vector<llvm::Value *> values;
            llvm::APInt zero(32, 0);
            llvm::APInt offset(32, expr->operand->offset+1);
            values.push_back(
            llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(context), zero));
            // This is the field offset
            values.push_back(
            llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(context), offset));
            auto gep = Builder.CreateGEP(currentAlloca,values,"");
            return Builder.CreateLoad(gep, "");
            //return Builder.CreateGEP(currentAlloca,indexList,"");
            //return Builder.CreateStructGEP(currentFrame,currentAlloca,4, "");

        }
        default: exit(1);


    }
    return NULL;
}

Value* CompileStatements(ASTstmt* stmt){
    if(stmt == NULL) return NULL;
    std::cout << "I AM DOING A STATEMENT" << std::endl;
    switch(stmt->type){
        case TSKIP:
            break;
        case TPC:
            break;
        case TFCALL:
            //CompileFCall(stmt->);
            break;
        case TIF:
            break;
        case TEXIT:
            Builder.CreateRet(nullptr);
            break;
        case TRET:
            Builder.CreateRet(CompileExpression(stmt->expr));
            break;
        case TCONTM:
            break;
        case TCONT:
            break;
        case TBREAKM:
            break;
        case TBREAK:
            break;
        case TLOOP:
            break;
        case TASSIGN:
        {
            //again gep loop
            std::vector<llvm::Value *> values;
            llvm::APInt zero(32, 0);
            llvm::APInt offset(32, stmt->lvalue->offset+1);
            values.push_back(
            llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(context), zero));
            // This is the field offset
            values.push_back(
            llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(context), offset));
            Value* gep = Builder.CreateGEP(currentAlloca,values,"");
            Value* val = CompileExpression(stmt->expr);
            Builder.CreateStore(val,gep);
            break;
        }
        default:
            std::cout << "Unexpected statement type" << std::endl;
    }
    return CompileStatements(stmt->tail);
}

int Compile(ASTfdef* main) {
    mod = new Module("arx.ll", getGlobalContext());
    mod->setDataLayout("");
    mod->setTargetTriple("x86_64-pc-linux-gnu");


    currentFrame = StructType::create(mod->getContext(), "struct.dummy");
    CompileFunction(main);

    //Builder.SetInsertPoint(bl);
    //Builder.CreateRet(a);
    //a->print(errs());
    legacy::PassManager pm;
    pm.add(createPrintModulePass(outs()));
    pm.run(*mod);
    //add certain passes to perform optimizations on the intermediate representation
    //also add passes to create x86-64 code and then perform optimizations on it
    return 0;
}
/*
int main(){
    return 0;
}
*/
