extern "C" {
#include "symbol.h"
}
#include "ast.h"
#include "llvm.h"
#include "opt.h"
#include <iostream>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <map>
#include <vector>

llvm::Value *CompileStatements(ASTstmt *stmt);
llvm::Value *CompileFunction(ASTfdef *);
llvm::Value *CompileCondition(ASTExpr *cond);
llvm::Value *CompileExpression(ASTExpr *);
llvm::Value *CompileDeclaration(ASTfdef *func);
llvm::AllocaInst *currentAlloca;
llvm::AllocaInst *retAlloca;
llvm::BasicBlock *RetCont;
llvm::BasicBlock *LoopCont;
llvm::BasicBlock *LoopBody;
map<string, llvm::BasicBlock *> LoopMap;
map<string, llvm::BasicBlock *> LoopMap2;
int i = 0;

static llvm::LLVMContext context;
static llvm::Type *i8 = llvm::IntegerType::get(context, 8);
static llvm::Type *i16 = llvm::IntegerType::get(context, 16);
static llvm::Type *i32 = llvm::IntegerType::get(context, 32);
static llvm::Type *i64 = llvm::IntegerType::get(context, 64);
static llvm::IRBuilder<> Builder(context);
static llvm::Module *mod;
llvm::StructType *currentFrame;
llvm::Type *translate(myType a) {
    if (equalType(a, typeInteger))
        return llvm::Type::getInt16Ty(context);
    if (equalType(a, typeChar))
        return llvm::Type::getInt8Ty(context);
    if (equalType(a, typeVoid))
        return llvm::Type::getVoidTy(context);
    if (isIArrayType(a)) {
        myType b = a->refType;
        llvm::Type *translatedRef = translate(b);
        return llvm::PointerType::get(translatedRef, 0);
    }
    if (isArrayType(a)) {
        myType b = a->refType;
        // llvm::Type *translatedRef = translate(b);
        vector<int> sizes;
        sizes.push_back(a->size);
        while (isArrayType(b)) {
            sizes.push_back(b->size);
            b = b->refType;
        }
        llvm::Type *result = translate(b);
        for (auto i : sizes) {
            // cout << i << endl;
            result = llvm::ArrayType::get(result, i);
        }
        // return nullptr;
        return result;
    }
    cout << "Type that i dont know yet.Probably array or byref" << endl;
    printType(a);
    return llvm::Type::getVoidTy(context);
    // exit(-1);
}
string transform(string a) { return a.substr(1, a.size() - 2); }

using namespace llvm;
bool isLibFunction(ASTfcall *f) {
    string id = f->identifier;
    return id == "writeInteger" or id == "writeByte" or id == "writeChar" or
           id == "writeString" or id == "readInteger" or id == "readByte" or
           id == "readChar" or id == "readString" or id == "extend" or
           id == "shrink" or id == "strlen" or id == "strcat" or
           id == "strcmp" or id == "strcpy";
}
ASTstmt *push_decl(ASTstmt *decl, std::vector<llvm::Type *> &fields) {
    if (decl == NULL)
        return NULL;
    // return NULL;
    switch (decl->type) {
    case TFDEF:
        // CompileFunction(decl->def);
        break;
    case TFDECL:
        break;
    case TDECL:
        for (auto i : *decl->identifiers) {
            fields.push_back(translate(decl->t));
        }
        break;
    default:
        return decl;
    }
    return push_decl(decl->tail, fields);
}
void InnerFunc(ASTstmt *decl) {
    if (decl == NULL)
        return;
    switch (decl->type) {
    case TFDEF:
        CompileFunction(decl->def);
        break;
    case TFDECL:
        CompileDeclaration(decl->def);
        break;
    case TDECL:
        break;
    default:
        return;
    }
    return InnerFunc(decl->tail);
}
Value *CompileLoop(ASTstmt *loop) {
    BasicBlock *OldLoop, *OldCont;
    OldLoop = LoopBody;
    OldCont = LoopCont;
    Function *TheFunction = Builder.GetInsertBlock()->getParent();
    LoopBody = BasicBlock::Create(getGlobalContext(), "", TheFunction);
    LoopCont = BasicBlock::Create(getGlobalContext(), "", TheFunction);
    Builder.CreateBr(LoopBody);
    Builder.SetInsertPoint(LoopBody);
    if (loop->label != "") {
        LoopMap[loop->label] = LoopBody;
        LoopMap2[loop->label] = LoopCont;
    }
    Value *body = CompileStatements(loop->body);
    Builder.CreateBr(LoopBody);
    // TheFunction->getBasicBlockList().push_back(LoopCont);
    Builder.SetInsertPoint(LoopCont);
    LoopBody = OldLoop;
    LoopCont = OldCont;
    if (loop->label != "") {
        // cerr << "HERE";
        auto x = LoopMap.find(loop->label);
        LoopMap.erase(x);
        x = LoopMap2.find(loop->label);
        LoopMap2.erase(x);
        // auto x = LoopMap->find(loop->label);
    }
    return body;
}
Value *CompileRefGEP(ASTExpr *expr) {
    if (expr->op == 'i') {
        int nesting_diff = expr->operand->nesting_diff;
        llvm::APInt zero(32, 0);
        Value *jj = currentAlloca;

        while (nesting_diff > 0) {
            std::vector<llvm::Value *> *nestlink =
                new std::vector<llvm::Value *>();
            nestlink->push_back(Constant::getIntegerValue(
                llvm::Type::getInt32Ty(context), zero));
            nestlink->push_back(Constant::getIntegerValue(
                llvm::Type::getInt32Ty(context), zero));
            auto gep2 = Builder.CreateGEP(jj, *nestlink, "");
            jj = Builder.CreateLoad(gep2, "");
            --nesting_diff;
            delete nestlink;
        }
        std::vector<llvm::Value *> values;
        llvm::APInt offset(32, expr->operand->offset + 1);
        values.push_back(llvm::Constant::getIntegerValue(
            llvm::Type::getInt32Ty(context), zero));
        // This is the field offset
        values.push_back(llvm::Constant::getIntegerValue(
            llvm::Type::getInt32Ty(context), offset));
        Value *gep;
        gep = Builder.CreateGEP(jj, values, "");
        for (auto i : *expr->operand->indices) {
            std::vector<llvm::Value *> index;
            // i->operand->offset = 2;
            Value *x = CompileExpression(i);

            Value *ext = Builder.CreateSExt(x, i64, "");

            llvm::APInt offset2(64, 3);
            llvm::APInt zero64(64, 0);
            if (gep->getType()->isPointerTy()) {

                auto a = gep->getType()->getPointerElementType();
                if (a->isArrayTy()) {
                    index.push_back(
                        llvm::Constant::getIntegerValue(i64, zero64));
                    index.push_back(ext);
                    gep = Builder.CreateGEP(gep, index, "");
                } else {
                    // cerr << "OMG" << endl;
                    index.push_back(ext);
                    gep = Builder.CreateLoad(gep);
                    gep = Builder.CreateInBoundsGEP(gep, index);
                    // gep = Builder.CreateLoad(gep);
                }
            }
        }
        return gep;
    } else {
        cerr << "You can only call by reference on an lval";
        exit(1);
    }
}
Value *CompileArrayGEP(ASTExpr *expr) {
    // cerr << "what is happening ";
    switch (expr->op) {
    case 'f':
        cerr << "error: You need to provide a local variable, for saving "
                "the "
                "array result";
        exit(1);
    case 'i':
        int nesting_diff = expr->operand->nesting_diff;
        llvm::APInt zero(32, 0);
        Value *jj = currentAlloca;

        while (nesting_diff > 0) {
            std::vector<llvm::Value *> *nestlink =
                new std::vector<llvm::Value *>();
            nestlink->push_back(Constant::getIntegerValue(
                llvm::Type::getInt32Ty(context), zero));
            nestlink->push_back(Constant::getIntegerValue(
                llvm::Type::getInt32Ty(context), zero));
            auto gep2 = Builder.CreateGEP(jj, *nestlink, "");
            jj = Builder.CreateLoad(gep2, "");
            --nesting_diff;
            delete nestlink;
        }
        std::vector<llvm::Value *> values;
        llvm::APInt offset(32, expr->operand->offset + 1);
        values.push_back(llvm::Constant::getIntegerValue(
            llvm::Type::getInt32Ty(context), zero));
        // This is the field offset
        values.push_back(llvm::Constant::getIntegerValue(
            llvm::Type::getInt32Ty(context), offset));
        Value *gep;
        gep = Builder.CreateGEP(jj, values, "");
        for (auto i : *expr->operand->indices) {
            std::vector<llvm::Value *> index;
            // i->operand->offset = 2;
            Value *x = CompileExpression(i);

            Value *ext = Builder.CreateSExt(x, i64, "");

            llvm::APInt offset2(64, 3);
            llvm::APInt zero64(64, 0);
            if (gep->getType()->isPointerTy()) {
                // cerr << "hello" << endl;
                auto a = gep->getType()->getPointerElementType();
                if (a->isArrayTy()) {
                    index.push_back(
                        llvm::Constant::getIntegerValue(i64, zero64));
                    index.push_back(ext);
                    gep = Builder.CreateGEP(gep, index, "");
                } else {
                    index.push_back(ext);
                    gep = Builder.CreateGEP(gep, index);
                    gep = Builder.CreateLoad(gep);
                }
            }
        }
        return gep;
    }
    cerr << "error: array needed as a parameter";
    exit(1);
}
Value *CompileIf(ASTif *ifnode) {
    Value *CondV = CompileCondition(ifnode->condition);
    if (!CondV)
        return nullptr;
    Function *TheFunction = Builder.GetInsertBlock()->getParent();
    // Create blocks for the then and else cases.  Insert the 'then' block
    // at
    // the
    // end of the function.
    BasicBlock *ThenBB =
        BasicBlock::Create(getGlobalContext(), "", TheFunction);
    BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "");
    BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "");
    Builder.CreateCondBr(CondV, ThenBB, ElseBB);

    // Emit then value.
    Builder.SetInsertPoint(ThenBB);

    CompileStatements(ifnode->body);
    ThenBB = Builder.GetInsertBlock();

    Builder.CreateBr(MergeBB);
    TheFunction->getBasicBlockList().push_back(ElseBB);
    Builder.SetInsertPoint(ElseBB);
    for (auto it = ifnode->tail; it != NULL; it = it->tail) {
        Value *Cond;
        // Emit else block.
        if (it->condition != NULL) {
            Cond = CompileCondition(it->condition);
            BasicBlock *Then =
                BasicBlock::Create(getGlobalContext(), "", TheFunction);
            BasicBlock *Elif = BasicBlock::Create(getGlobalContext(), "");
            Builder.CreateCondBr(Cond, Then, Elif);
            Builder.SetInsertPoint(Then);
            CompileStatements(it->body);
            Builder.CreateBr(MergeBB);
            TheFunction->getBasicBlockList().push_back(Elif);
            Builder.SetInsertPoint(Elif);
        } else {
            CompileStatements(it->body);
        }
    }
    Builder.CreateBr(MergeBB);

    // Emit merge block.
    TheFunction->getBasicBlockList().push_back(MergeBB);
    Builder.SetInsertPoint(MergeBB);
    return NULL;
}
Value *FuncCall(ASTfcall *f) {
    if (isLibFunction(f)) {
        // std::cout << "I have found a lib function" << std::endl;
        Function *CalleeF = mod->getFunction(f->identifier);
        Function::arg_iterator arg_it = CalleeF->arg_begin();
        if (CalleeF == NULL) {
            cerr << f->identifier << "No such function";
            exit(1);
        }
        std::vector<Value *> params;
        if (f->parameters != NULL) {
            reverse(f->parameters->begin(), f->parameters->end());
            for (auto i : *f->parameters) {
                Value *p = CompileExpression(i);
                // p = Builder.CreateSExt(p, i64, "");
                Value *curr_arg = &(*arg_it);
                if (curr_arg->getType()->isPointerTy() &&
                    p->getType()->isArrayTy()) {

                    std::vector<llvm::Value *> index;
                    llvm::APInt zero64(64, 0);
                    index.push_back(
                        llvm::Constant::getIntegerValue(i64, zero64));
                    index.push_back(
                        llvm::Constant::getIntegerValue(i64, zero64));
                    p = CompileArrayGEP(i);
                    p = Builder.CreateGEP(p, index);
                }
                if (curr_arg->getType()->isPointerTy() &&
                    p->getType()->isIntegerTy()) {
                    curr_arg->getType()->print(errs());
                    // cerr << endl;
                    // p->getType()->print(errs());
                    // cerr << "HEREEEEEEEEE";
                    p = CompileRefGEP(i);
                }

                params.push_back(p);
                arg_it++;
            }
        }
        return Builder.CreateCall(CalleeF, params, "");
    }
    int nd = f->nesting_diff;
    std::vector<Value *> params;
    llvm::APInt zero(32, 0);
    Function *CalleeF = mod->getFunction(f->identifier);
    Value *j = currentAlloca;
    while (nd > 0) {
        std::vector<llvm::Value *> *nestlink = new std::vector<llvm::Value *>();
        nestlink->push_back(
            Constant::getIntegerValue(llvm::Type::getInt32Ty(context), zero));
        nestlink->push_back(
            Constant::getIntegerValue(llvm::Type::getInt32Ty(context), zero));
        auto gep = Builder.CreateGEP(j, *nestlink, "");
        j = Builder.CreateLoad(gep, "");
        nd--;
        delete nestlink;
    }
    params.push_back(j);
    Function::arg_iterator arg_it = CalleeF->arg_begin();
    arg_it++;
    if (f->parameters != NULL) {
        reverse(f->parameters->begin(), f->parameters->end());
        for (auto i : *f->parameters) {
            Value *p = CompileExpression(i);
            Value *curr_arg = &(*arg_it);
            if (curr_arg->getType()->isPointerTy() &&
                p->getType()->isArrayTy()) {
                std::vector<llvm::Value *> index;
                llvm::APInt zero64(64, 0);
                index.push_back(llvm::Constant::getIntegerValue(i64, zero64));
                index.push_back(llvm::Constant::getIntegerValue(i64, zero64));
                p = CompileArrayGEP(i);
                p = Builder.CreateGEP(p, index);
            }
            if (curr_arg->getType()->isPointerTy() &&
                p->getType()->isIntegerTy()) {
                // cerr << "HEREEEEEEEEE";
                p = CompileRefGEP(i);
            }
            params.push_back(p);
            arg_it++;
        }
    }
    return Builder.CreateCall(CalleeF, params, "");
}
Value *CompileDeclaration(ASTfdef *func) {
    if (func == NULL)
        return NULL;

    std::vector<llvm::Type *> params;
    PointerType *frame_ptr = PointerType::get(currentFrame, 0);
    params.push_back(frame_ptr);
    // currentFrame = frame;
    // do the parameters!

    for (auto p = func->header->paramlist; p != NULL; p = p->next) {
        int numparams = p->identifiers->size();
        for (int j = 0; j < numparams; ++j) {
            auto newType = translate(p->p);
            if (p->byref)
                newType = PointerType::get(newType, 0);
            params.push_back(newType);
        }
    }

    FunctionType *ftype =
        FunctionType::get(translate(func->header->type), params, false);
    Function *function = Function::Create(
        /*Type=*/ftype,
        /*Linkage=*/GlobalValue::ExternalLinkage,
        /*Name=*/func->header->identifier, mod);
    Function::arg_iterator arg_it = function->arg_begin();
    arg_it->setName("link");
    arg_it++;

    for (auto p = func->header->paramlist; p != NULL; p = p->next) {
        for (auto id : *p->identifiers) {
            arg_it->setName(id);
            arg_it++;
        }
    }
    return function;
}
Value *CompileFunction(ASTfdef *func) {
    StructType *old = currentFrame;
    if (func == NULL)
        return NULL;
    StructType *frame =
        mod->getTypeByName("struct." + func->header->identifier);
    if (!frame) {
        frame = StructType::create(mod->getContext(),
                                   "struct." + func->header->identifier);
    }
    std::vector<llvm::Type *> frame_fields;
    frame_fields.push_back(PointerType::get(currentFrame, 0));
    std::vector<llvm::Type *> params;
    PointerType *frame_ptr = PointerType::get(currentFrame, 0);
    params.push_back(frame_ptr);
    currentFrame = frame;
    // do the parameters!

    for (auto p = func->header->paramlist; p != NULL; p = p->next) {
        int numparams = p->identifiers->size();
        for (int j = 0; j < numparams; ++j) {
            auto newType = translate(p->p);
            if (p->byref)
                newType = PointerType::get(newType, 0);
            frame_fields.push_back(newType);
            params.push_back(newType);
        }
    }
    ASTstmt *first = push_decl(func->body, frame_fields);

    if (frame->isOpaque()) {
        frame->setBody(frame_fields, /*isPacked=*/false);
    }
    // done with definitions start
    FunctionType *ftype =
        FunctionType::get(translate(func->header->type), params, false);
    Function *function = mod->getFunction(func->header->identifier);
    if (function == NULL)
        function = Function::Create(
            /*Type=*/ftype,
            /*Linkage=*/GlobalValue::ExternalLinkage,
            /*Name=*/func->header->identifier, mod);
    Function::arg_iterator arg_it = function->arg_begin();
    arg_it->setName("link");
    arg_it++;

    for (auto p = func->header->paramlist; p != NULL; p = p->next) {
        for (auto id : *p->identifiers) {
            arg_it->setName(id);
            arg_it++;
        }
    }
    InnerFunc(func->body);
    BasicBlock *bl = BasicBlock::Create(context, "", function, 0);
    RetCont = BasicBlock::Create(getGlobalContext(), "", function);
    Builder.SetInsertPoint(bl);
    currentAlloca = Builder.CreateAlloca(frame, 0, "");
    if (!equalType(func->header->type, typeVoid)) {
        retAlloca = Builder.CreateAlloca(translate(func->header->type), 0, "");
    }
    // result = Builder.CreateRet(CompileExpression(stmt->expr));
    unsigned int j = 0;
    arg_it = function->arg_begin();
    Value *current; //= &(*arg_it);
    std::vector<llvm::Value *> values;
    llvm::APInt zero(32, 0);
    values.push_back(
        llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(context), zero));
    values.push_back(
        llvm::Constant::getIntegerValue(llvm::Type::getInt32Ty(context), zero));
    auto gep = Builder.CreateGEP(currentAlloca, values, "");
    current = &(*arg_it);
    Builder.CreateStore(current, gep);
    arg_it++;
    // std::cout << params.size() << std::endl;

    for (j = 1; j < params.size(); ++j, ++arg_it) {
        std::vector<llvm::Value *> values;
        llvm::APInt zero(32, 0);
        llvm::APInt offset(32, j);
        values.push_back(llvm::Constant::getIntegerValue(
            llvm::Type::getInt32Ty(context), zero));
        values.push_back(llvm::Constant::getIntegerValue(
            llvm::Type::getInt32Ty(context), offset));
        auto gep = Builder.CreateGEP(currentAlloca, values, "");
        current = &(*arg_it);
        Builder.CreateStore(current, gep);
    }

    CompileStatements(first);
    Builder.CreateBr(RetCont);
    // function->getBasicBlockList().push_back(RetCont);
    Builder.SetInsertPoint(RetCont);
    if (!equalType(func->header->type, typeVoid)) {
        Value *retval = Builder.CreateLoad(retAlloca, "");
        Builder.CreateRet(retval);
    } else {
        Builder.CreateRet(nullptr);
    }

    currentFrame = old;
    return NULL;
}
Value *CompileCondition(ASTExpr *cond) {
    Value *result = CompileExpression(cond);
    if (result->getType() == llvm::Type::getInt16Ty(context)) {
        result = Builder.CreateICmpNE(
            result, ConstantInt::get(llvm::Type::getInt16Ty(context), 0, true),
            "");
    } else if (result->getType() == llvm::Type::getInt8Ty(context)) {
        result = Builder.CreateICmpNE(
            result, ConstantInt::get(llvm::Type::getInt8Ty(context), 0, true),
            "");
    }
    return result;
}
Value *CompileBoolean(Value *result) {
    // cerr << "Compiling Boolean" << endl;
    if (result->getType() == llvm::Type::getInt16Ty(context)) {
        result = Builder.CreateICmpNE(
            result, ConstantInt::get(llvm::Type::getInt16Ty(context), 0, true),
            "");
    } else if (result->getType() == llvm::Type::getInt8Ty(context)) {
        result = Builder.CreateICmpNE(
            result, ConstantInt::get(llvm::Type::getInt8Ty(context), 0, true),
            "");
    }
    return result;
}

Value *CompileExpression(ASTExpr *expr) {

    // std::cout << "I AM DOING AN EXPRESSION" << std::endl;
    if (expr == NULL) {
        return NULL;
    }
    // cerr << expr->op << endl;
    Value *left, *right;
    switch (expr->op) {
    case '+':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        if (left != NULL)
            return Builder.CreateBinOp(Instruction::Add, left, right, "");
        else
            return right;
    case '-':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        if (left != NULL)
            return Builder.CreateBinOp(Instruction::Sub, left, right, "");
        else
            return Builder.CreateNeg(right, "");
    case '*':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateBinOp(Instruction::Mul, left, right, "");
    case '/':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateBinOp(Instruction::SDiv, left, right, "");
    case '%':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateURem(left, right, "");
    // mhpws SRem?
    case '&':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateAnd(left, right, "");
    case '|':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateOr(left, right, "");
    case '!':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateNot(right, "");
    case 'c':
        return ConstantInt::get(llvm::Type::getInt16Ty(context),
                                expr->constant_val, true);
    case 'x':
        return ConstantInt::get(llvm::Type::getInt8Ty(context),
                                expr->constant_val, true);
    case 'b':
        return ConstantInt::get(llvm::Type::getInt8Ty(context),
                                expr->constant_val, true);
    case '>':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateICmpSGT(left, right, "");
    case '<':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateICmpSLT(left, right, "");
    case 'l':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateICmpSGE(left, right, "");
    case 's':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateICmpSLE(left, right, "");
    case 'e':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateICmpEQ(left, right, "");
    case 'd':
        left = CompileExpression(expr->left);
        right = CompileExpression(expr->right);
        return Builder.CreateICmpNE(left, right, "");
    case 'a': {
        left = CompileExpression(expr->left);
        left = CompileBoolean(left);
        // right = CompileExpression(expr->right);
        // right = CompileBoolean(right);
        Function *TheFunction = Builder.GetInsertBlock()->getParent();
        Value *CondV = Builder.CreateICmpEQ(
            left, ConstantInt::get(llvm::Type::getInt1Ty(context), 0, true));

        BasicBlock *ThenBB =
            BasicBlock::Create(getGlobalContext(), "", TheFunction);
        BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "");
        BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "");
        Builder.CreateCondBr(CondV, ThenBB, ElseBB);
        // Builder.CreateCondBr(CondV, ThenBB, ElseBB);

        // Emit then value.
        Builder.SetInsertPoint(ThenBB);

        Value *ThenV =
            ConstantInt::get(llvm::Type::getInt1Ty(context), 0, true);

        Builder.CreateBr(MergeBB);

        ThenBB = Builder.GetInsertBlock();

        // Emit else block.
        TheFunction->getBasicBlockList().push_back(ElseBB);
        Builder.SetInsertPoint(ElseBB);

        Value *ElseV = CompileExpression(expr->right);
        ElseV = CompileBoolean(ElseV);

        Builder.CreateBr(MergeBB);
        // Codegen of 'Else' can change the current block, update ElseBB for the
        // PHI.
        ElseBB = Builder.GetInsertBlock();

        // Emit merge block.
        TheFunction->getBasicBlockList().push_back(MergeBB);
        Builder.SetInsertPoint(MergeBB);
        PHINode *PN = Builder.CreatePHI(llvm::Type::getInt1Ty(context), 2);

        PN->addIncoming(ThenV, ThenBB);
        PN->addIncoming(ElseV, ElseBB);
        return PN;

        // return Builder.CreateAnd(left, right, "");
    }
    case 'o': {
        left = CompileExpression(expr->left);
        left = CompileBoolean(left);
        // right = CompileExpression(expr->right);
        // right = CompileBoolean(right);
        Function *TheFunction = Builder.GetInsertBlock()->getParent();
        Value *CondV = Builder.CreateICmpEQ(
            left, ConstantInt::get(llvm::Type::getInt1Ty(context), 1, true));

        BasicBlock *ThenBB =
            BasicBlock::Create(getGlobalContext(), "", TheFunction);
        BasicBlock *MergeBB = BasicBlock::Create(getGlobalContext(), "");
        BasicBlock *ElseBB = BasicBlock::Create(getGlobalContext(), "");
        Builder.CreateCondBr(CondV, ThenBB, ElseBB);
        // Builder.CreateCondBr(CondV, ThenBB, ElseBB);

        // Emit then value.
        Builder.SetInsertPoint(ThenBB);

        Value *ThenV =
            ConstantInt::get(llvm::Type::getInt1Ty(context), 1, true);

        Builder.CreateBr(MergeBB);

        ThenBB = Builder.GetInsertBlock();

        // Emit else block.
        TheFunction->getBasicBlockList().push_back(ElseBB);
        Builder.SetInsertPoint(ElseBB);

        Value *ElseV = CompileExpression(expr->right);
        ElseV = CompileBoolean(ElseV);

        Builder.CreateBr(MergeBB);
        // Codegen of 'Else' can change the current block, update ElseBB for the
        // PHI.
        ElseBB = Builder.GetInsertBlock();

        // Emit merge block.
        TheFunction->getBasicBlockList().push_back(MergeBB);
        Builder.SetInsertPoint(MergeBB);
        PHINode *PN = Builder.CreatePHI(llvm::Type::getInt1Ty(context), 2);

        PN->addIncoming(ThenV, ThenBB);
        PN->addIncoming(ElseV, ElseBB);
        return PN;
        // left = CompileExpression(expr->left);
        // right = CompileExpression(expr->right);
        // left = CompileBoolean(left);
        // right = CompileBoolean(right);
        // return Builder.CreateOr(left, right, "");
    }

    case 'n':
        right = CompileExpression(expr->right);
        right = CompileBoolean(right);
        return Builder.CreateNot(right, "");
    case 'f':
        return FuncCall(expr->f);
    case 'i': {
        // cerr << "i entered" << endl;
        if (expr->operand->byRef == 1) {

            // cerr << "I HAVE FOUND A REF" << endl;
            // cerr << expr->operand->identifier << endl;

            int nesting_diff = expr->operand->nesting_diff;
            llvm::APInt zero(32, 0);
            Value *jj = currentAlloca;

            while (nesting_diff > 0) {
                std::vector<llvm::Value *> *nestlink =
                    new std::vector<llvm::Value *>();
                nestlink->push_back(Constant::getIntegerValue(
                    llvm::Type::getInt32Ty(context), zero));
                nestlink->push_back(Constant::getIntegerValue(
                    llvm::Type::getInt32Ty(context), zero));
                auto gep2 = Builder.CreateGEP(jj, *nestlink, "");
                jj = Builder.CreateLoad(gep2, "");
                --nesting_diff;
                delete nestlink;
            }
            std::vector<llvm::Value *> values;
            llvm::APInt offset(32, expr->operand->offset + 1);
            values.push_back(llvm::Constant::getIntegerValue(
                llvm::Type::getInt32Ty(context), zero));
            // This is the field offset
            values.push_back(llvm::Constant::getIntegerValue(
                llvm::Type::getInt32Ty(context), offset));
            Value *gep;
            gep = Builder.CreateGEP(jj, values, "");
            // gep->getType()->print(errs());
            gep = Builder.CreateLoad(gep);
            return Builder.CreateLoad(gep);
        } else {
            if (expr->operand->constant) {

                Value *str = Builder.CreateGlobalStringPtr(
                    transform(expr->operand->identifier));

                if (expr->operand->indices->size() > 0) {
                    for (auto i : *expr->operand->indices) {
                        std::vector<llvm::Value *> index;
                        Value *x = CompileExpression(i);
                        Value *ext = Builder.CreateSExt(x, i64, "");
                        llvm::APInt zero64(64, 0);
                        // index.push_back(
                        //    llvm::Constant::getIntegerValue(i64, zero64));
                        index.push_back(ext);
                        str = Builder.CreateGEP(str, index);
                    }
                    return Builder.CreateLoad(str);
                } else
                    return str;
            } else {
                int nesting_diff = expr->operand->nesting_diff;
                llvm::APInt zero(32, 0);
                Value *jj = currentAlloca;

                while (nesting_diff > 0) {
                    std::vector<llvm::Value *> *nestlink =
                        new std::vector<llvm::Value *>();
                    nestlink->push_back(Constant::getIntegerValue(
                        llvm::Type::getInt32Ty(context), zero));
                    nestlink->push_back(Constant::getIntegerValue(
                        llvm::Type::getInt32Ty(context), zero));
                    auto gep2 = Builder.CreateGEP(jj, *nestlink, "");
                    jj = Builder.CreateLoad(gep2, "");
                    --nesting_diff;
                    delete nestlink;
                }
                std::vector<llvm::Value *> values;
                llvm::APInt offset(32, expr->operand->offset + 1);
                values.push_back(llvm::Constant::getIntegerValue(
                    llvm::Type::getInt32Ty(context), zero));
                // This is the field offset
                values.push_back(llvm::Constant::getIntegerValue(
                    llvm::Type::getInt32Ty(context), offset));
                Value *gep;
                gep = Builder.CreateGEP(jj, values, "");
                for (auto i : *expr->operand->indices) {
                    std::vector<llvm::Value *> index;
                    Value *x = CompileExpression(i);

                    Value *ext = Builder.CreateSExt(x, i64, "");

                    llvm::APInt offset2(64, 3);
                    llvm::APInt zero64(64, 0);
                    if (gep->getType()->isPointerTy()) {

                        auto a = gep->getType()->getPointerElementType();
                        if (a->isArrayTy()) {
                            index.push_back(
                                llvm::Constant::getIntegerValue(i64, zero64));
                            index.push_back(ext);
                            gep = Builder.CreateGEP(gep, index, "");
                        } else {
                            index.push_back(ext);
                            gep = Builder.CreateLoad(gep);
                            gep = Builder.CreateInBoundsGEP(gep, index);
                            // gep = Builder.CreateLoad(gep);
                        }
                    }
                }
                return Builder.CreateLoad(gep);
            }
        }
    }
    default:
        cerr << "A message would be nice";
    }
    return NULL;
}

Value *CompileStatements(ASTstmt *sstmt) {
    if (sstmt == NULL)
        return NULL;
    Value *result = NULL;
    for (auto stmt = sstmt; stmt != NULL; stmt = stmt->tail) {
        // cerr << i++ << endl;
        switch (stmt->type) {
        case TSKIP:
            break;
        case TPC:
            result = FuncCall(stmt->expr->f);
            break;
        case TFCALL:
            result = FuncCall(stmt->expr->f);
            break;
        case TIF:
            result = CompileIf(stmt->ifnode);
            break;
        case TEXIT: {
            Function *function = Builder.GetInsertBlock()->getParent();
            auto c = BasicBlock::Create(getGlobalContext(), "", function);
            Builder.CreateBr(RetCont);
            Builder.SetInsertPoint(c);
            break;
        }
        case TRET: {
            result =
                Builder.CreateStore(CompileExpression(stmt->expr), retAlloca);
            Function *function = Builder.GetInsertBlock()->getParent();
            auto c = BasicBlock::Create(getGlobalContext(), "", function);
            Builder.CreateBr(RetCont);
            Builder.SetInsertPoint(c);

            break;
        }
        case TCONTM: {
            Function *function = Builder.GetInsertBlock()->getParent();
            // auto body = BasicBlock::
            auto LoopB = LoopMap[stmt->label];
            if (LoopB == NULL) {
                cerr << "Loop label not found" << endl;
                exit(1);
            }
            result = Builder.CreateBr(LoopB);
            // Builder.CreateBr(RetCont);
            auto c = BasicBlock::Create(getGlobalContext(), "", function);
            Builder.SetInsertPoint(c);
            break;
        }
        case TCONT: {
            Function *function = Builder.GetInsertBlock()->getParent();
            result = Builder.CreateBr(LoopBody);
            // Builder.CreateBr(RetCont);
            auto c = BasicBlock::Create(getGlobalContext(), "", function);
            Builder.SetInsertPoint(c);
            break;
        }
        case TBREAKM: {
            Function *function = Builder.GetInsertBlock()->getParent();
            auto LoopB = LoopMap2[stmt->label];
            if (LoopB == NULL) {
                cerr << "Loop label not found" << endl;
                exit(1);
            }
            result = Builder.CreateBr(LoopB);
            // Builder.CreateBr(RetCont);
            auto c = BasicBlock::Create(getGlobalContext(), "", function);
            Builder.SetInsertPoint(c);
            break;
        }
        case TBREAK: {
            Function *function = Builder.GetInsertBlock()->getParent();
            result = Builder.CreateBr(LoopCont);
            // Builder.CreateBr(RetCont);
            auto newblock =
                BasicBlock::Create(getGlobalContext(), "", function);
            Builder.SetInsertPoint(newblock);
            break;
        }
        case TLOOP:
            result = CompileLoop(stmt);
            break;
        case TASSIGN: {
            if (stmt->lvalue->byRef) {
                std::vector<llvm::Value *> values;
                llvm::APInt zero(32, 0);
                Value *jj = currentAlloca;
                int nesting_diff = stmt->lvalue->nesting_diff;
                while (nesting_diff > 0) {
                    std::vector<llvm::Value *> *nestlink =
                        new std::vector<llvm::Value *>();
                    nestlink->push_back(Constant::getIntegerValue(
                        llvm::Type::getInt32Ty(context), zero));
                    nestlink->push_back(Constant::getIntegerValue(
                        llvm::Type::getInt32Ty(context), zero));
                    auto gep2 = Builder.CreateGEP(jj, *nestlink, "");
                    jj = Builder.CreateLoad(gep2, "");
                    --nesting_diff;
                    delete nestlink;
                }
                llvm::APInt offset(32, stmt->lvalue->offset + 1);
                values.push_back(llvm::Constant::getIntegerValue(
                    llvm::Type::getInt32Ty(context), zero));
                // This is the field offset
                values.push_back(llvm::Constant::getIntegerValue(
                    llvm::Type::getInt32Ty(context), offset));
                Value *gep = Builder.CreateGEP(jj, values, "");

                gep = Builder.CreateLoad(gep);
                Value *val = CompileExpression(stmt->expr);
                result = Builder.CreateStore(val, gep);
                break;
            } else {
                std::vector<llvm::Value *> values;
                llvm::APInt zero(32, 0);
                Value *jj = currentAlloca;
                int nesting_diff = stmt->lvalue->nesting_diff;
                while (nesting_diff > 0) {
                    std::vector<llvm::Value *> *nestlink =
                        new std::vector<llvm::Value *>();
                    nestlink->push_back(Constant::getIntegerValue(
                        llvm::Type::getInt32Ty(context), zero));
                    nestlink->push_back(Constant::getIntegerValue(
                        llvm::Type::getInt32Ty(context), zero));
                    auto gep2 = Builder.CreateGEP(jj, *nestlink, "");
                    jj = Builder.CreateLoad(gep2, "");
                    --nesting_diff;
                    delete nestlink;
                }
                llvm::APInt offset(32, stmt->lvalue->offset + 1);
                values.push_back(llvm::Constant::getIntegerValue(
                    llvm::Type::getInt32Ty(context), zero));
                // This is the field offset
                values.push_back(llvm::Constant::getIntegerValue(
                    llvm::Type::getInt32Ty(context), offset));
                Value *gep = Builder.CreateGEP(jj, values, "");
                for (auto i : *stmt->lvalue->indices) {
                    std::vector<llvm::Value *> index;
                    Value *x = CompileExpression(i);
                    Value *ext = Builder.CreateSExt(x, i64, "");
                    llvm::APInt offset2(64, 3);
                    llvm::APInt zero64(64, 0);
                    if (gep->getType()->isPointerTy()) {
                        // cerr << "hello" << endl;
                        auto a = gep->getType()->getPointerElementType();
                        if (a->isArrayTy()) {
                            index.push_back(
                                llvm::Constant::getIntegerValue(i64, zero64));
                            index.push_back(ext);
                            gep = Builder.CreateGEP(gep, index, "");
                        } else {
                            index.push_back(ext);
                            gep = Builder.CreateLoad(gep);
                            gep = Builder.CreateInBoundsGEP(gep, index);
                            // gep = Builder.CreateLoad(gep);
                        }
                    }
                }

                Value *val = CompileExpression(stmt->expr);
                result = Builder.CreateStore(val, gep);
                break;
            }
        }
        default:
            std::cout << "Unexpected statement type" << std::endl;
        }
    }
    return result;
}

int Compile(ASTfdef *main) {
    mod = new Module("1.ll", getGlobalContext());
    mod->setTargetTriple("x86_64-pc-linux-gnu");
    // mod->setDataLayout("e-m:e-i64:64-f80:128-n8:16:32:64-S128");
    FunctionType *writeChar_type = FunctionType::get(
        llvm::Type::getVoidTy(context), std::vector<llvm::Type *>{i8}, false);
    Function::Create(writeChar_type, Function::ExternalLinkage, "writeChar",
                     mod);

    FunctionType *writeByte_type = FunctionType::get(
        llvm::Type::getVoidTy(context), std::vector<llvm::Type *>{i8}, false);
    Function::Create(writeByte_type, Function::ExternalLinkage, "writeByte",
                     mod);

    FunctionType *writeInteger_type = FunctionType::get(
        llvm::Type::getVoidTy(context), std::vector<llvm::Type *>{i16}, false);
    Function::Create(writeInteger_type, Function::ExternalLinkage,
                     "writeInteger", mod);

    FunctionType *writeString_type = FunctionType::get(
        llvm::Type::getVoidTy(context),
        std::vector<llvm::Type *>{PointerType::get(i8, 0)}, false);
    Function::Create(writeString_type, Function::ExternalLinkage, "writeString",
                     mod);

    FunctionType *strlen_type = FunctionType::get(
        i16, std::vector<llvm::Type *>{PointerType::get(i8, 0)}, false);
    Function::Create(strlen_type, Function::ExternalLinkage, "strlen", mod);

    FunctionType *strcmp_type = FunctionType::get(
        i16, std::vector<llvm::Type *>{PointerType::get(i8, 0),
                                       PointerType::get(i8, 0)},
        false);
    Function::Create(strcmp_type, Function::ExternalLinkage, "strcmp", mod);

    FunctionType *strcpy_type = FunctionType::get(
        i16, std::vector<llvm::Type *>{PointerType::get(i8, 0),
                                       PointerType::get(i8, 0)},
        false);
    Function::Create(strcpy_type, Function::ExternalLinkage, "strcpy", mod);

    FunctionType *strcat_type =
        FunctionType::get(llvm::Type::getVoidTy(context),
                          std::vector<llvm::Type *>{PointerType::get(i8, 0),
                                                    PointerType::get(i8, 0)},
                          false);
    Function::Create(strcat_type, Function::ExternalLinkage, "strcat", mod);

    FunctionType *readInteger_type =
        FunctionType::get(i16, std::vector<llvm::Type *>{}, false);
    Function::Create(readInteger_type, Function::ExternalLinkage, "readInteger",
                     mod);

    FunctionType *readByte_type =
        FunctionType::get(i8, std::vector<llvm::Type *>{}, false);
    Function::Create(readByte_type, Function::ExternalLinkage, "readByte", mod);

    FunctionType *readChar_type =
        FunctionType::get(i8, std::vector<llvm::Type *>{}, false);
    Function::Create(readChar_type, Function::ExternalLinkage, "readChar", mod);

    FunctionType *readString_type = FunctionType::get(
        llvm::Type::getVoidTy(context),
        std::vector<llvm::Type *>{i16, PointerType::get(i8, 0)}, false);
    Function::Create(readString_type, Function::ExternalLinkage, "readString",
                     mod);

    FunctionType *extend_type =
        FunctionType::get(i16, std::vector<llvm::Type *>{i8}, false);
    Function *extend =
        Function::Create(extend_type, Function::ExternalLinkage, "extend", mod);
    Function::arg_iterator args = extend->arg_begin();
    Value *int8_c = &(*args);
    BasicBlock *bl = BasicBlock::Create(context, "", extend, 0);
    Builder.SetInsertPoint(bl);
    Value *ret = Builder.CreateSExt(int8_c, i16);
    Builder.CreateRet(ret);

    FunctionType *shrink_type =
        FunctionType::get(i8, std::vector<llvm::Type *>{i16}, false);
    Function *shrink =
        Function::Create(shrink_type, Function::ExternalLinkage, "shrink", mod);
    args = shrink->arg_begin();
    Value *int16_c = &(*args);
    BasicBlock *bl2 = BasicBlock::Create(context, "", shrink, 0);
    Builder.SetInsertPoint(bl2);
    ret = Builder.CreateTrunc(int16_c, i8);
    Builder.CreateRet(ret);

    currentFrame = StructType::create(mod->getContext(), "struct.dummy");
    CompileFunction(main);
    auto m = mod->getFunction(main->header->identifier);
    m->setName("main");
    // Builder.SetInsertPoint(bl);
    // Builder.CreateRet(a);
    // a->print(errs());
    legacy::PassManager pm; //= Optimize();
    pm.add(createPrintModulePass(outs()));
    // pm.add(createVerifierPass());

    pm.run(*mod);
    return 0;
}
