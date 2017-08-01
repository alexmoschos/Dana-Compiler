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

static llvm::LLVMContext context;
static llvm::IRBuilder<> Builder(context);
static llvm::Module *mod;
llvm::StructType *currentFrame;
llvm::Type* translate(Type a){
    if(equalType(a,typeInteger))
        return llvm::Type::getInt16Ty(context);
    if(equalType(a,typeChar))
        return llvm::Type::getInt8Ty(context);

    cout << "Type that i dont know yet.Probably array or byref"; 
    exit(-1);

}

using namespace llvm;
void push_decl(ASTstmt *decl, std::vector<llvm::Type*>& fields){
    //std::cout << "hello" << std::endl;
    if(decl == NULL) return;
    //std::cout << "hello" << std::endl;
    switch(decl->type){
    case TFDEF:
        CompileFunction(decl->def);

        break;
    case TFDECL:
        break;
    case TDECL:
        //std::cout << "beginf" << std::endl;
        for(auto i : *decl->identifiers){
            //std::cout << "loopf" << std::endl;
            fields.push_back(translate(decl->t));
        }
        break;
    default:
        return;
    }
    push_decl(decl->tail,fields);

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


    currentFrame = frame;
    push_decl(func->body,frame_fields);
    if (frame->isOpaque()) {
        frame->setBody(frame_fields, /*isPacked=*/false);
    }
    //CompileStatements(func->body);
    currentFrame = old;
    //done with definitions start 
    vector<const llvm::Type*> argTypes;
    FunctionType* ftype = FunctionType::get(translate(func->header->type),false);
    Function* function = Function::Create(
     /*Type=*/ftype,
     /*Linkage=*/GlobalValue::ExternalLinkage,
     /*Name=*/func->header->identifier, mod);
    BasicBlock* bl = BasicBlock::Create(context,"",function,0);
    Builder.SetInsertPoint(bl);

    Builder.CreateAlloca(frame, 0, "my_frame");
    return Builder.CreateRet(ConstantInt::get(llvm::Type::getInt16Ty(context),42,true));

    
}
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
            break;

        case 'i':

            break;
        default: exit(1);


    }
    return NULL;
}

Value* CompileStatements(ASTstmt* stmt){
    if(stmt == NULL) return NULL;
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

int Compile(ASTfdef* main) {
    mod = new Module("arx.ll", getGlobalContext());
    mod->setDataLayout("");
    mod->setTargetTriple("x86_64-pc-linux-gnu");
    /*
    vector<const llvm::Type*> argTypes;
    FunctionType* ftype = FunctionType::get(llvm::Type::getInt16Ty(context),false);
    Function* mainFunction = Function::Create(
    // /*Type=*///ftype,
    // /*Linkage=*/GlobalValue::ExternalLinkage,
    // /*Name=*/"main", mod);
    //mainFunction->setCallingConv(CallingConv::C);
    //BasicBlock* bl = BasicBlock::Create(context,"entry",mainFunction,0);
    

    currentFrame = StructType::create(mod->getContext(), "struct.dummy");
    //auto second_func = new ASTstmt(TFDEF,NULL,NULL,"");
    //second_func->def =new ASTfdef(new ASTheader(typeInteger,NULL,"asthenoforo"),NULL);

    //auto dec = new ASTstmt(TDECL,NULL,NULL,"");
    //second_func->def->body = dec;
    //std::vector<std::string> *id = new std::vector<std::string>();
    //id->push_back("hello");
    //id->push_back("world");
    //dec->identifiers = id;
    //dec->t = typeInteger; 
    //CompileFunction(new ASTfdef(new ASTheader(typeChar,NULL,"kremmudi"), second_func));
    CompileFunction(main);

    //Builder.SetInsertPoint(bl);
    //Builder.CreateRet(a);
    //a->print(errs());
    legacy::PassManager pm;
    pm.add(createPrintModulePass(outs()));
    pm.run(*mod);
//    if(a != NULL) exit(1);
    return 0;
}
