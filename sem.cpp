extern "C" {
#include "error.h"
#include "symbol.h"
}
#include "ast.h"
#include "sem.h"
#include "string.h"
#include <algorithm>
#include <stdio.h>

void sem_check_fdef(ASTfdef *func) {
    if (func == NULL)
        exit(-1);
    ASTheader *header = func->header;
    SymbolEntry *f = newFunction(header->identifier.c_str());
    openScope();
    ASTparam *iter = header->paramlist;
    while (iter != NULL) {
        for (auto st : *iter->identifiers) {
            if (iter->byref)
                newParameter(st.c_str(), iter->p, PASS_BY_REFERENCE, f);
            else
                newParameter(st.c_str(), iter->p, PASS_BY_VALUE, f);
        }
        iter = iter->next;
    }
    endFunctionHeader(f, header->type);
    sem_check_stmt(func->body);
    closeScope();
}

void sem_check_fdecl(ASTfdef *func) {
    if (func == NULL)
        exit(-1);
    ASTheader *header = func->header;
    SymbolEntry *f = newFunction(header->identifier.c_str());
    forwardFunction(f);
    openScope();
    ASTparam *iter = header->paramlist;
    while (iter != NULL) {
        for (auto st : *iter->identifiers) {
            if (iter->byref)
                newParameter(st.c_str(), iter->p, PASS_BY_REFERENCE, f);
            else
                newParameter(st.c_str(), iter->p, PASS_BY_VALUE, f);
        }
        iter = iter->next;
    }
    endFunctionHeader(f, header->type);
    // sem_check_stmt(func->body);
    closeScope();
}

void sem_check_stmt(ASTstmt *stmt) {
    if (stmt == NULL)
        return;
    switch (stmt->type) {
    case TSKIP:
        break;
    case TPC: {
        SymbolEntry *s = lookupEntry(stmt->expr->f->identifier.c_str(),
                                     LOOKUP_ALL_SCOPES, true);
        if (s->entryType != ENTRY_FUNCTION) {
            error("\ridentifier is not a function");
            exit(1);
        }
        if (!equalType(s->u.eFunction.resultType, typeVoid)) {
            error("\rnon-void function cannot be called as a command");
            exit(1);
        }

        SymbolEntry *par = s->u.eFunction.firstArgument;
        stmt->expr->f->nesting_diff =
            currentScope->nestingLevel - s->nestingLevel;
        vector<ASTExpr *> *par_vector_ptr = stmt->expr->f->parameters;
        if (par_vector_ptr == NULL) {
            if (par == NULL)
                break;
            else {
                printf("%s:\n", s->id);
                error("\rfewer parameter provided in function");
                exit(1);
            }
        }

        vector<ASTExpr *> par_vector = *(par_vector_ptr);
        // std::reverse(par_vector.begin(), par_vector.end());
        int counter = 0, size = par_vector.size();

        while (par) {

            if (counter >= size) {
                error("\rfewer parameter provided in function");
                exit(1);
            }
            Type par_type = sem_check_expr(par_vector[counter]);
            // printType(par_type);
            printf("\n");
            if (!equalType(par->u.eParameter.type, par_type)) {
                if (par_type->kind == 5 && par->u.eParameter.type->kind == 5) {
                    if (par->u.eParameter.type->size > 0) {
                        printType(par_type);
                        printf("\n");
                        printType(par->u.eParameter.type);
                        error("\rType mismatch in real and typical parameters");
                        exit(1);
                    }
                }
            }
            counter++;
            par = par->u.eParameter.next;
        }

        if (counter < size) {
            printf("%s:\n", s->id);
            error("\rmore parameters provided in function than required");
            exit(1);
        }

        stmt->expr->f->nesting_diff =
            currentScope->nestingLevel - s->nestingLevel;
        // printf("%s  %d\n\n", s->id, stmt->expr->f->nesting_diff);

        break;
    }
    case TFCALL: {
        SymbolEntry *s = lookupEntry(stmt->expr->f->identifier.c_str(),
                                     LOOKUP_ALL_SCOPES, true);
        if (s->entryType != ENTRY_FUNCTION) {
            error("\ridentifier is not a function");
            exit(1);
        }
        if (!equalType(s->u.eFunction.resultType, typeVoid)) {
            error("\rnon-void function cannot be called as a command");
            exit(1);
        }
        stmt->expr->f->nesting_diff =
            currentScope->nestingLevel - s->nestingLevel;
        SymbolEntry *par = s->u.eFunction.firstArgument;

        vector<ASTExpr *> *par_vector_ptr = stmt->expr->f->parameters;
        if (par_vector_ptr == NULL) {
            if (par == NULL)
                break;
            else {
                printf("%s:\n", s->id);
                error("\rfewer parameter provided in function");
                exit(1);
            }
        }

        vector<ASTExpr *> par_vector = *(par_vector_ptr);
        // std::reverse(par_vector.begin(), par_vector.end());
        int counter = 0, size = par_vector.size();

        while (par) {

            if (counter >= size) {
                error("\rfewer parameter provided in function");
                exit(1);
            }
            Type par_type = sem_check_expr(par_vector[counter]);
            if (!equalType(par->u.eParameter.type, par_type)) {

                if (par_type->kind == 5 && par->u.eParameter.type->kind == 5) {
                    if (par->u.eParameter.type->size > 0) {
                        printType(par_type);
                        printf("\n");
                        printType(par->u.eParameter.type);
                        printf("%d \n", counter);
                        error("\rType mismatch in real and typical parameters");
                        exit(1);
                    }
                }
            }

            counter++;
            par = par->u.eParameter.next;
        }

        if (counter < size) {
            printf("%s:\n", s->id);
            error("\rmore parameters provided in function than required");
            exit(1);
        }

        stmt->expr->f->nesting_diff =
            currentScope->nestingLevel - s->nestingLevel;
        printf("%s  %d\n\n", s->id, stmt->expr->f->nesting_diff);

        break;
    }
    case TIF: {
        Type cond_type = sem_check_expr(stmt->ifnode->condition);
        if (!equalType(cond_type, typeBoolean) &&
            !equalType(cond_type, typeChar) &&
            !equalType(cond_type, typeInteger)) {
            error("\rwrong type in if-condition");
            exit(1);
        }
        sem_check_stmt(stmt->ifnode->body);

        ASTif *new_ifnode = stmt->ifnode->tail;
        while (new_ifnode) {
            sem_check_stmt(new_ifnode->body);
            if (new_ifnode->condition) {
                Type cond_type2 = sem_check_expr(new_ifnode->condition);
                if (!equalType(cond_type2, typeBoolean) &&
                    !equalType(cond_type2, typeChar) &&
                    !equalType(cond_type2, typeInteger)) {
                    error("\rWrong type in if-condition");
                    exit(1);
                }
            }
            new_ifnode = new_ifnode->tail;
        }
        break;
    }

    case TFDEF: {
        sem_check_fdef(stmt->def);
        // SymbolEntry *s = lookupEntry(stmt->def->header->identifier.c_str(),
        //                             LOOKUP_ALL_SCOPES, true);
        // forwardFunction(s);
        break;
    }
    case TFDECL:
        sem_check_fdecl(stmt->def);
        break;
    case TEXIT:{

        if (stmt->def == NULL) {
            error("\rshould have a ASTdef to check function type");
            exit(1);
        }

        SymbolEntry *s = lookupEntry(stmt->def->header->identifier.c_str(),
                                     LOOKUP_ALL_SCOPES, true);

         if (!equalType(typeVoid, s->u.eFunction.resultType)) {

            printf("%s:\n", s->id);
            error("\rCan't exit from non void function ");
            exit(1);
        }


        break;
     }
    case TRET: {
        Type _TRET_TYPE = sem_check_expr(stmt->expr);

        if (stmt->def == NULL) {
            error("\rshould have a ASTdef to check function type with return type");
            exit(1);
        }

        SymbolEntry *s = lookupEntry(stmt->def->header->identifier.c_str(),
                                     LOOKUP_ALL_SCOPES, true);
        if (s->entryType != ENTRY_FUNCTION)
            internal("NOT a Function"); // redundant all astdef in stack are
                                        // functions...

        if (!equalType(_TRET_TYPE, s->u.eFunction.resultType) &&
            !(equalType(_TRET_TYPE, typeInteger) &&
              equalType(s->u.eFunction.resultType, typeChar)) &&
            !(equalType(_TRET_TYPE, typeChar) &&
              equalType(s->u.eFunction.resultType, typeInteger))) {

            printf("%s:\n", s->id);
            error("\rFunction type and return type different ");
            exit(1);
        }
        break;
    }

    case TCONTM: {
        SymbolEntry *s =
            lookupEntry(stmt->label.c_str(), LOOKUP_ALL_SCOPES, true);
        if (s->entryType != ENTRY_VARIABLE)
            error("Loop label error");
        // internal("NOT a label");
        if (!equalType(s->u.eVariable.type, typeVoid)) {
            error("\rYou can only use continue on a loop label");
            exit(1);
        }
        break;
    }
    case TCONT: {
        break;
    }
    case TBREAKM: {
        SymbolEntry *s =
            lookupEntry(stmt->label.c_str(), LOOKUP_ALL_SCOPES, true);
        if (s->entryType != ENTRY_VARIABLE)
            error("Loop label error");
        if (!equalType(s->u.eVariable.type, typeVoid)) {
            error("\rYou can only use break on a constant");
            exit(1);
        }
        break;
    }
    case TBREAK: {
        break;
    }

    case TDECL: {
        for (auto st : *stmt->identifiers)
            newVariable(st.c_str(), stmt->t);
        break;
    }
    case TLOOP: {
        if ((stmt->label).compare("") != 0)
            newVariable(stmt->label.c_str(), typeVoid);
        sem_check_stmt(stmt->body);

        break;
    }
    case TASSIGN: {
        Type rval_type = sem_check_expr(stmt->expr);
        //printType(rval_type);
        SymbolEntry *s = lookupEntry(stmt->lvalue->identifier.c_str(),
                                     LOOKUP_ALL_SCOPES, true);
        stmt->lvalue->nesting_diff =
            currentScope->nestingLevel - s->nestingLevel;
        if (s == NULL) {
            // cout << "Hello";
            exit(1);
        }
        Type lval_type;
        if (s->entryType == ENTRY_PARAMETER) {
            if (s->u.eParameter.mode == PASS_BY_REFERENCE)
                stmt->lvalue->byRef = 1;
            lval_type = s->u.eParameter.type;
            stmt->lvalue->offset = s->u.eParameter.offset;
        } else if (s->entryType == ENTRY_VARIABLE) {
            lval_type = s->u.eVariable.type;
            stmt->lvalue->offset = s->u.eVariable.offset;
        } else {
            error("\rNot an lvalue !!");
            exit(1);
        }
        for (auto i : *stmt->lvalue->indices) {
            if (!equalType(sem_check_expr(i), typeInteger)) {
                error("\rNeed an integer inside an index");
            }
        }
        if ((lval_type->kind == 5 && ((*stmt->lvalue->indices)).size() > 0) ||
            lval_type->kind == 6) {
            int size = (*stmt->lvalue->indices).size();
            while (lval_type->refType != NULL && size > 0) {
                lval_type = lval_type->refType;
                size--;
            }
        }

        if (equalType(lval_type, typeChar) && equalType(rval_type, typeInteger))
            break;
        if (!equalType(lval_type, rval_type)) {
            printType(lval_type);
            printf("\n");
            printType(rval_type);
            printf("\n");
            error("\rlvalue and rvalue Typemismatch");
            exit(1);
        }

        stmt->lvalue->nesting_diff =
            currentScope->nestingLevel - s->nestingLevel;
        // printf("%s\t%d\t%d\n\n", s->id, stmt->lvalue->nesting_diff,
        //       stmt->lvalue->offset);

        break;
    }
    }
    sem_check_stmt(stmt->tail);
}

Type sem_check_expr(ASTExpr *expr) {
    if (expr == NULL) {
        return typeVoid;
    }
    // SymbolEntry *a;
    Type left = sem_check_expr(expr->left);
    Type right = sem_check_expr(expr->right);
    switch (expr->op) {
    case '+':
        if ((!equalType(typeInteger, right) && !equalType(typeChar, right)) ||
            (!equalType(typeInteger, left) && !equalType(typeVoid, left) &&
             !equalType(typeChar, left)) ||
            (!equalType(typeVoid, left) && !equalType(right, left))) {
            error("\rTypemismatch in add");
            exit(1);
        }
        return right;

    case '-':
        if ((!equalType(typeInteger, right) && !equalType(typeChar, right)) ||
            (!equalType(typeInteger, left) && !equalType(typeVoid, left) &&
             !equalType(typeChar, left)) ||
            (!equalType(typeVoid, left) && !equalType(right, left))) {
            error("\rTypemismatch in minus");
            exit(1);
        }
        return right;

    case '*':
        if (!(equalType(typeInteger, right) && equalType(typeInteger, left)) &&
            !(equalType(typeChar, right) && equalType(typeChar, left))) {
            error("\rTypemismatch in multiplication");
            exit(1);
        }
        return right;
    case '/':
        if (!(equalType(typeInteger, right) && equalType(typeInteger, left)) &&
            !(equalType(typeChar, right) && equalType(typeChar, left))) {
            error("\rTypemismatch in div");
            exit(1);
        }
        return right;

    case '%':
        if (!(equalType(typeInteger, right) && equalType(typeInteger, left)) &&
            !(equalType(typeChar, right) && equalType(typeChar, left))) {
            error("\rTypemismatch in mod");
            exit(1);
        }
        return right;

    case '&':
        if (!equalType(left, typeChar) || !equalType(right, typeChar)) {
            error("\rTypemismatch in AND");
            exit(-1);
        }
        return right;
    case '|':
        if (!equalType(left, typeChar) || !equalType(right, typeChar)) {
            error("\rTypemismatch in OR");
            exit(-1);
        }
        return right;
    case '!':
        if (!equalType(right, typeChar)) {
            error("\rTypemismatch in NOT");
            exit(-1);
        }
        return right;

    case 'c':
        return typeInteger;

    case 'x': {
        return typeChar;
    }

    case 'b':
        return typeChar;

    case '>':
        if (!equalType(left, right)) {
            error("\rCan't compare operands of different types");
            exit(1);
        } else if (!equalType(left, typeChar) &&
                   !equalType(left, typeInteger)) {
            error("\roperator > can be used only with int or byte");
            exit(1);
        }
        return typeBoolean;
    case '<':
        if (!equalType(left, right)) {
            error("\rCant compare operands of different types");
            exit(1);
        } else if (!equalType(left, typeChar) &&
                   !equalType(left, typeInteger)) {
            error("\roperator < can be used only with int or byte");
            exit(1);
        }
        return typeBoolean;

    case 'l':
        if (!equalType(left, right)) {
            error("\rCan't compare operands of different types");
            exit(1);
        } else if (!equalType(left, typeChar) &&
                   !equalType(left, typeInteger)) {
            error("\roperator >= can be used only with int or byte");
            exit(1);
        }
        return typeBoolean;

    case 's':
        if (!equalType(left, right)) {
            error("\rCan't compare operands of different types");
            exit(1);
        } else if (!equalType(left, typeChar) &&
                   !equalType(left, typeInteger)) {
            error("\roperator <= can be used only with int or byte");
            exit(1);
        }
        return typeBoolean;

    case 'e':
        if (!equalType(left, right)) {
            error("\rCan't compare operands of different types");
            exit(1);
        } else if (!equalType(left, typeChar) &&
                   !equalType(left, typeInteger)) {
            error("\roperator = can be used only with int or byte");
            exit(1);
        }
        return typeBoolean;

    case 'd':
        if (!equalType(left, right)) {
            error("\rCan't compare operands of different types");
            exit(1);
        } else if (!equalType(left, typeChar) &&
                   !equalType(left, typeInteger)) {
            error("\roperator <> can be used only with int or byte");
            exit(1);
        }
        return typeBoolean;

    case 'a':

        if ((!equalType(left, typeChar) && !equalType(left, typeInteger) && !equalType(left, typeBoolean)) ||
            (!equalType(right, typeChar) && !equalType(right, typeInteger) && !equalType(right, typeBoolean))) {
            error("\rand operator requires boolean condition");
            exit(-1);
        }
        return right;

    case 'o':

        if ((!equalType(left, typeChar) && !equalType(left, typeInteger) && !equalType(left, typeBoolean)) ||
            (!equalType(right, typeChar) && !equalType(right, typeInteger) && !equalType(right, typeBoolean))) {
            error("\ror operator requires boolean condition");
            exit(-1);
        }
        return right;

    case 'n':

        if ((!equalType(right, typeChar) && !equalType(right, typeInteger) && !equalType(right, typeBoolean))) {
            error("\rnot operator requires boolean condition");
            exit(1);
        }
        return right;

    case 'f': {
        // printf("hey\n");
        SymbolEntry *a =
            lookupEntry(expr->f->identifier.c_str(), LOOKUP_ALL_SCOPES, true);
        if (a->entryType != ENTRY_FUNCTION) {
            error("\rExpression not a function");
            exit(1);
        }
        expr->f->nesting_diff = currentScope->nestingLevel - a->nestingLevel;

        SymbolEntry *par = a->u.eFunction.firstArgument;

        vector<ASTExpr *> *par_vector_ptr = expr->f->parameters;
        if (par_vector_ptr == NULL) {
            if (par == NULL)
                return a->u.eFunction.resultType;
            else {
                printf("%s:\n", a->id);
                error("\rfewer parameter provided in function");
                exit(1);
            }
        }
        // printf("hey2\n");

        vector<ASTExpr *> par_vector = *(par_vector_ptr);
        // std::reverse(par_vector.begin(), par_vector.end());
        int counter = 0, size = par_vector.size();

        while (par) {

            if (counter >= size) {
                error("\rfewer parameter provided in function");
                exit(1);
            }
            Type par_type = sem_check_expr(par_vector[counter]);
            if (!equalType(par->u.eParameter.type, par_type)) {

                if (par_type->kind == 5 && par->u.eParameter.type->kind == 5) {
                    if (par->u.eParameter.type->size > 0) {
                        printType(par_type);
                        printf("\n");
                        printType(par->u.eParameter.type);
                        printf("%d \n", counter);

                        error("\rType mismatch in real and typical parameters");
                        exit(1);
                    }
                }
            }

            counter++;
            par = par->u.eParameter.next;
        }

        if (counter < size) {
            error("\rmore parameters provided in function than required");
            exit(1);
        }

        if (equalType(a->u.eFunction.resultType, typeVoid))
            error("\rvoid function in expression!");

        expr->f->nesting_diff = currentScope->nestingLevel - a->nestingLevel;
        // printf("function call %s  %d\n\n", expr->f->identifier.c_str(),
        //       expr->f->nesting_diff);

        return a->u.eFunction.resultType;
    }

    case 'i': {
        ASTlval *lv = expr->operand;

        // for stringliteral
        if (lv->constant == true) {
            printf("**\n");
            if (lv->indices->size() == 1)
                return typeChar;
            if (lv->indices->size() == 0)
                return typeIArray(typeChar);
            //    return typeArray(strlen(lv->identifier.c_str()) + 1,
            //    typeChar);
            else {
                error("Too many indices on string literal");
            }
        }

        SymbolEntry *s =
            lookupEntry(lv->identifier.c_str(), LOOKUP_ALL_SCOPES, true);
        Type lval_type;
        if (s->entryType == ENTRY_PARAMETER) {
            if (s->u.eParameter.mode == PASS_BY_REFERENCE)
                lv->byRef = 1;
            lval_type = s->u.eParameter.type;
            lv->offset = s->u.eParameter.offset;
        } else if (s->entryType == ENTRY_VARIABLE) {
            lval_type = s->u.eVariable.type;
            lv->offset = s->u.eVariable.offset;
        } else {
            error("\rNot an lvalue !!");
            exit(1);
        }

        lv->nesting_diff = currentScope->nestingLevel - s->nestingLevel;
        // printf("%s\t%d\t%d\n\n", s->id, lv->nesting_diff, lv->offset);
        for (auto i : *lv->indices) {
            if (!equalType(sem_check_expr(i), typeInteger)) {
                error("\rNeed an integer inside an index");
            }
        }

        if ( (lval_type->kind == 5 || lval_type->kind == 6) && ((*lv->indices)).size() > 0) {

            while (lval_type->refType != NULL) {
                lval_type = lval_type->refType;
            }
        }
       // printType(lval_type);
        //printf("\n");
        return lval_type;
    }
    }
    return typeVoid;
}

// ######################################################
// ----------------- LIBRARY FUNCTIONS ------------------
// ######################################################

void add_lib_Functions() {
    vector<string> *param_identifiers;
    ASTparam *second_Param, *first_Param;
    //|------------------------OUTPUT LIBRARY FUNCTIONS-----------------------|

    param_identifiers = new vector<string>();
    param_identifiers->push_back("n");
    first_Param = new ASTparam(param_identifiers, typeInteger, NULL);

    ASTheader *writeInteger_header =
        new ASTheader(typeVoid, first_Param, "writeInteger");
    ASTfdef *writeInteger = new ASTfdef(writeInteger_header, NULL);

    sem_check_fdef(writeInteger);

    /*******************************************************************/

    param_identifiers = new vector<string>();
    param_identifiers->push_back("b");
    first_Param = new ASTparam(param_identifiers, typeChar, NULL);

    ASTheader *writeByte_header =
        new ASTheader(typeVoid, first_Param, "writeByte");
    ASTfdef *writeByte = new ASTfdef(writeByte_header, NULL);

    sem_check_fdef(writeByte);

    /*******************************************************************/

    param_identifiers = new vector<string>();
    param_identifiers->push_back("b");
    first_Param = new ASTparam(param_identifiers, typeChar, NULL);

    ASTheader *writeChar_header =
        new ASTheader(typeVoid, first_Param, "writeChar");
    ASTfdef *writeChar = new ASTfdef(writeChar_header, NULL);

    sem_check_fdef(writeChar);

    /*******************************************************************/

    param_identifiers = new vector<string>();
    param_identifiers->push_back("s");
    first_Param = new ASTparam(param_identifiers, typeIArray(typeChar), NULL);

    ASTheader *writeString_header =
        new ASTheader(typeVoid, first_Param, "writeString");
    ASTfdef *writeString = new ASTfdef(writeString_header, NULL);

    sem_check_fdef(writeString);

    //|-------------------------INPUT LIBRARY FUNCTIONS----------------------|

    ASTheader *readInteger_header =
        new ASTheader(typeInteger, NULL, "readInteger");
    ASTfdef *readInteger = new ASTfdef(readInteger_header, NULL);
    sem_check_fdef(readInteger);

    /*******************************************************************/

    ASTheader *readByte_header = new ASTheader(typeChar, NULL, "readByte");
    ASTfdef *readByte = new ASTfdef(readByte_header, NULL);
    sem_check_fdef(readByte);

    /*******************************************************************/

    ASTheader *readChar_header = new ASTheader(typeChar, NULL, "readChar");
    ASTfdef *readChar = new ASTfdef(readChar_header, NULL);
    sem_check_fdef(readChar);

    /*******************************************************************/

    param_identifiers = new vector<string>();
    param_identifiers->push_back("s");
    second_Param =
        new ASTparam(param_identifiers, typeIArray(typeChar),
                     NULL); //////WHY typeIArray ???????den xereis to megethos

    param_identifiers = new vector<string>();
    param_identifiers->push_back("n");
    first_Param = new ASTparam(param_identifiers, typeInteger, second_Param);

    ASTheader *readString_header =
        new ASTheader(typeVoid, first_Param, "readString");
    ASTfdef *readString = new ASTfdef(readString_header, NULL);

    sem_check_fdef(readString);

    /*
    -----------------------------------------------------------------------------
    |---------------------------CONVERT LIBRARY
    FUNCTIONS-----------------------|
    -----------------------------------------------------------------------------
       */

    param_identifiers = new vector<string>();
    param_identifiers->push_back("b");
    first_Param = new ASTparam(param_identifiers, typeChar, NULL);

    ASTheader *extend_header =
        new ASTheader(typeInteger, first_Param, "extend");
    ASTfdef *extend = new ASTfdef(extend_header, NULL);

    sem_check_fdef(extend);

    /*******************************************************************/

    param_identifiers = new vector<string>();
    param_identifiers->push_back("i");
    first_Param = new ASTparam(param_identifiers, typeInteger, NULL);

    ASTheader *shrink_header = new ASTheader(typeChar, first_Param, "shrink");
    ASTfdef *shrink = new ASTfdef(shrink_header, NULL);

    sem_check_fdef(shrink);

    /*-----------------------------------------------------------------------------
      |----------------------------STRING LIBRARY
      FUNCTIONS-----------------------|
      -----------------------------------------------------------------------------
    */

    param_identifiers = new vector<string>();
    param_identifiers->push_back("s");
    first_Param = new ASTparam(param_identifiers, typeIArray(typeChar), NULL);

    ASTheader *strlen_header =
        new ASTheader(typeInteger, first_Param, "strlen");
    ASTfdef *strlen = new ASTfdef(strlen_header, NULL);

    sem_check_fdef(strlen);

    /*******************************************************************/

    param_identifiers = new vector<string>();
    param_identifiers->push_back("s2");
    second_Param = new ASTparam(param_identifiers, typeIArray(typeChar), NULL);

    param_identifiers = new vector<string>();
    param_identifiers->push_back("s1");
    first_Param =
        new ASTparam(param_identifiers, typeIArray(typeChar), second_Param);

    ASTheader *strcmp_header =
        new ASTheader(typeInteger, first_Param, "strcmp");
    ASTfdef *strcmp = new ASTfdef(strcmp_header, NULL);

    sem_check_fdef(strcmp);

    /*******************************************************************/

    param_identifiers = new vector<string>();
    param_identifiers->push_back("trg");
    param_identifiers->push_back("src");
    first_Param = new ASTparam(param_identifiers, typeIArray(typeChar), NULL);

    ASTheader *strcpy_header = new ASTheader(typeVoid, first_Param, "strcpy");
    ASTfdef *strcpy = new ASTfdef(strcpy_header, NULL);

    sem_check_fdef(strcpy);

    /*******************************************************************/

    param_identifiers = new vector<string>();
    param_identifiers->push_back("trg");
    param_identifiers->push_back("src");
    first_Param = new ASTparam(param_identifiers, typeIArray(typeChar), NULL);

    ASTheader *strcat_header = new ASTheader(typeVoid, first_Param, "strcat");
    ASTfdef *strcat = new ASTfdef(strcat_header, NULL);

    sem_check_fdef(strcat);
}
