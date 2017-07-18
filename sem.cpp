extern "C"{
 #include "symbol.h"
 #include "error.h"
}
#include "sem.h"
#include "ast.h"
#include <stdio.h>
//TODO: petrou an kati eiani int[1][2][3] tha prepei na mporei na mpei se sunarthsh me type int[][2][3]

void printSymbolTable ()
{
    Scope       * scp;
    SymbolEntry * e;
    SymbolEntry * args;

    scp = currentScope;
    if (scp == NULL)
        printf("no scope\n");
    else
        while (scp != NULL) {
            printf("scope: ");
            e = scp->entries;
            while (e != NULL) {
                if (e->entryType == ENTRY_TEMPORARY)
                    printf("$%d", e->u.eTemporary.number);
                else
                    printf("%s", e->id);
                switch (e->entryType) {
                    case ENTRY_FUNCTION:
                        printf("(");
                        args = e->u.eFunction.firstArgument;
                        while (args != NULL) {
                            printMode(args->u.eParameter.mode);
                            printf("%s : ", args->id);
                            printType(args->u.eParameter.type);
                            args = args->u.eParameter.next;
                            if (args != NULL)
                                printf("; ");
                        }
                        printf(") : ");
                        printType(e->u.eFunction.resultType);
                        break;
#ifdef SHOW_OFFSETS
                    case ENTRY_VARIABLE:
                        printf("[%d]", e->u.eVariable.offset);
                        break;
                    case ENTRY_PARAMETER:
                        printf("[%d]", e->u.eParameter.offset);
                        break;
                    case ENTRY_TEMPORARY:
                        printf("[%d]", e->u.eTemporary.offset);
                        break;
#endif
                }
                e = e->nextInScope;
                if (e != NULL)
                    printf(", ");
            }
            scp = scp->parent;
            printf("\n");
        }
    printf("----------------------------------------\n");
}


void sem_check_fdef(ASTfdef* func){
        if(func==NULL) exit(-1);
        cout << "I just started a function definition check" << endl;
        ASTheader *header = func->header;
        SymbolEntry *f = newFunction(header->identifier.c_str());
        openScope();
        cout << "the new function is " << header->identifier << endl;;
        ASTparam *iter = header->paramlist;
        while(iter != NULL){
                for(auto st : *iter->identifiers){
                        if(iter->byref)
                                newParameter(st.c_str(),iter->p,PASS_BY_REFERENCE,f);
                        else
                                newParameter(st.c_str(),iter->p,PASS_BY_VALUE,f); //Can arrays be passed by value?? NO parser doesnt allow it
                }
                iter = iter->next;
        }

        endFunctionHeader(f,header->type);

        sem_check_stmt(func->body);
        cout << "HERE" << endl;
        closeScope();
}


void sem_check_stmt(ASTstmt* stmt){
        if(stmt==NULL) return;
        cout << " i just started a stmt check" << endl;
        switch(stmt->type){
                case TSKIP:
                        {
                           break;
                        }

                case TPC:
                        {
                          SymbolEntry *s = lookupEntry(stmt->expr->f->identifier.c_str(),LOOKUP_ALL_SCOPES,true);
                           printf("%s",s ->id);
                          if(s->entryType!=ENTRY_FUNCTION){
                               error("\ridentifier is not a function");
                               exit(1);
                          }
                          if(!equalType(s->u.eFunction.resultType,typeVoid)) {
                               error("\rnon-void function cannot be called as a command");
                               exit(1);
                          }

                          SymbolEntry *par = s -> u.eFunction.firstArgument;

                          vector<ASTExpr*> par_vector = *(stmt->expr->f->parameters);
                          std::reverse(par_vector.begin(),par_vector.end());
                          int counter  = 0, size = par_vector.size();

                          printf("\n" );

                          while(par){
                               printf("%s \n",par->id);
                              if(counter >= size) {error("\rfewer parameter provided in function"); exit(1);}
                              Type par_type = sem_check_expr(par_vector[counter]);
                              if(!equalType(par->u.eParameter.type,par_type)) {
                                   
                                    if(par_type->kind == 5 && par->u.eParameter.type->kind == 5){
                                      if(par->u.eParameter.type->size>0){
                                        error("\rType mismatch in real and typical parameters");
                                        exit(1);
                                      }
                                    }
                                   // if(par_vector[counter]->f!=NULL)
                                     // printf("%s\n", par_vector[counter]->f->identifier.c_str());

                                    //printf("%d\n",counter );
                                    //printSymbolTable();
                                  //  printf("\n");
                                   // printType(sem_check_expr(par_vector[counter]));
                                    //printf("\n");
                                    //printType( par->u.eParameter.type);
                                    //printf("%d\n", par->u.eParameter.type->size);
                                    //printf("\n");
                               }

                               counter++;
                               par = par->u.eParameter.next;
                           }

                          if(counter < size) {
                               error("\rmore parameters provided in function than required");
                               exit(1);
                          }

                          break;
                        }

                case TFCALL:
                        {
                          printSymbolTable();
                          cout <<"PRIN" << endl;
                          SymbolEntry *s = lookupEntry(stmt->expr->f->identifier.c_str(),LOOKUP_ALL_SCOPES,true);
                          cout << "WHAT";
                          if(s->entryType!=ENTRY_FUNCTION) {
                               error("\ridentifier is not a function");
                               exit(1);
                          }
                          if(!equalType(s->u.eFunction.resultType,typeVoid)) {
                               error("\rnon-void function cannot be called as a command");
                               exit(1);
                          }

                          SymbolEntry *par = s -> u.eFunction.firstArgument;

                          vector<ASTExpr*> par_vector = *(stmt->expr->f->parameters);
                          int counter  = 0, size = par_vector.size();

                          while(par->u.eParameter.next){
                             if(counter >= size) {
                                 error("\rfewer parameter provided in function");
                                 exit(1);
                              }
                             if(!equalType(par->u.eParameter.type,sem_check_expr(par_vector[counter]))){
                                 error("\rType mismatch in real and typical parameters");
                                 exit(1);
                              }
                             counter++;
                             par = par->u.eParameter.next;
                          }

                          if(counter < size) {
                              error("\rmore parameters provided in function than required");
                              exit(1);
                          }
                          break;
                        }

                case TIF:
                        {
                          printf("TIF\n");
                          Type cond_type = sem_check_expr(stmt->ifnode->condition);
                          if(!equalType(cond_type,typeBoolean) && !equalType(cond_type,typeChar) && !equalType(cond_type,typeInteger)){
                              error("\rwrong type in if-condition");
                              exit(1);
                          }
                          sem_check_stmt(stmt->ifnode->body);

                          ASTif* new_ifnode = stmt->ifnode->tail;
                          while(new_ifnode){

                             sem_check_stmt(new_ifnode->body);
                             if(new_ifnode->condition){

                                Type cond_type2 = sem_check_expr(new_ifnode->condition);
                                if(!equalType(cond_type2,typeBoolean) && !equalType(cond_type2,typeChar) && !equalType(cond_type2,typeInteger)){
                                      error("\rwrong type in if-condition");
                                      exit(1);
                                }
                             }
                             new_ifnode = new_ifnode->tail;
                          }
                          break;
                        }

                case TFDEF:
                        {
                          sem_check_fdef(stmt->def);
                          SymbolEntry *s = lookupEntry(stmt->def->header->identifier.c_str(),LOOKUP_ALL_SCOPES,true);
                          forwardFunction (s);
                          break;
                        }

                case TFDECL:
                          sem_check_fdef(stmt->def);
                          break;

                case TEXIT:
                           break;

                case TRET:
                        {
                           Type _TRET_TYPE = sem_check_expr(stmt->expr);

                           if(stmt->def == NULL) {
                                 error("\rshould have a ASTdef to check function type with return type");
                                 exit(1);
                           }

                           SymbolEntry *s = lookupEntry(stmt->def->header->identifier.c_str(),LOOKUP_ALL_SCOPES,true);
                           if (s->entryType != ENTRY_FUNCTION)
                               internal("NOT a Function");   //redundant all astdef in stack are functions...

                           if(!equalType(_TRET_TYPE,s->u.eFunction.resultType)) {
                                 error("\rFunction type and return type different");
                                 exit(1);
                           }
                           break;
                        }

                case TCONTM:
                         {
                           SymbolEntry *s = lookupEntry(stmt->label.c_str(),LOOKUP_ALL_SCOPES,true);
                           if (s->entryType != ENTRY_CONSTANT) internal("NOT a label");
                           if(!equalType(s->u.eConstant.type,typeVoid)) {
                                  error("\rFunction type and return type different");
                                  exit(1);
                           }
                           break;
                         }

                case TCONT:
                         {
                           break;
                         }
                case TBREAKM:
                         {
                            SymbolEntry *s = lookupEntry(stmt->label.c_str(),LOOKUP_ALL_SCOPES,true);
                            if (s->entryType != ENTRY_CONSTANT)  internal("NOT a label");
                            if(!equalType(s->u.eConstant.type,typeVoid)) {
                                   error("\rFunction type and return type different");
                                   exit(1);
                            }
                            break;
                         }

                case TBREAK:
                         {
                            break;
                         }

                case TDECL:
                          {
                             for(auto st : *stmt->identifiers) newVariable(st.c_str(),stmt->t);
                             break;
                          }

                case TLOOP:
                         {
                            if((stmt->label).compare("")!=0) newConstant(stmt->label.c_str(),typeVoid);
                            printf("TLOOP\n");
                            sem_check_stmt(stmt->body);
                            break;
                         }

                case TASSIGN:
                {
                         printf("TASSIGN\n");
                        Type rval_type = sem_check_expr(stmt->expr);
                        SymbolEntry *s = lookupEntry(stmt->lvalue->identifier.c_str(),LOOKUP_ALL_SCOPES,true);
                        
                        if(s==NULL) {
                                cout << "Hello";
                                exit(1);
                        }
                        Type lval_type;
                        if(s->entryType == ENTRY_PARAMETER)
                                lval_type = s->u.eParameter.type;
                        else if(s->entryType == ENTRY_VARIABLE)
                                lval_type = s->u.eVariable.type;
                        else{
                                error("\rNot an lvalue !!");
                                exit(1);
                        }
                        printf("%s",s->id);


                        if((lval_type->kind == 5 && ((*stmt->lvalue->indices)).size()>0) || lval_type->kind == 6){
                            int size = (*stmt->lvalue->indices).size();
                            while(lval_type->refType != NULL && size > 0 ){
                              lval_type = lval_type->refType;
                              size --;
                            }
                            printType(lval_type);
                          }
                       
                       if(equalType(lval_type,typeChar) && equalType(rval_type,typeInteger)) break;
                         
                       if(!equalType(lval_type,rval_type)){

                                error("\rlvalue and rvalue Typemismatch");

                                exit(1);
                        }

                            break;
              }
        }
        sem_check_stmt(stmt->tail);

}


Type sem_check_expr(ASTExpr* expr){

        if(expr==NULL){
                return typeVoid;
        }

        SymbolEntry *a;
        Type left = sem_check_expr(expr->left);
        Type right = sem_check_expr(expr->right);
        switch(expr->op){
                case '+':
                        if(!equalType(typeInteger,right) || (!equalType(typeInteger,left) && !equalType(typeVoid,left))){
                                error("\rTypemismatch in add");
                                exit(1);
                        }
                        return right;

                case '-':
                        if(!equalType(typeInteger,right) ||  (!equalType(typeInteger,left) && !equalType(typeVoid,left))){
                                error("\rTypemismatch in minus");
                                exit(1);
                        }
                        return right;

                case '*':
                       if(!(equalType(typeInteger,right) && equalType(typeInteger,left)) &&
                           !(equalType(typeChar,right) && equalType(typeChar,left)) ) {
                                error("\rTypemismatch in multiplication");
                                exit(1);
                        }
                        return right;
                case '/':
                         if(!(equalType(typeInteger,right) && equalType(typeInteger,left)) &&
                            !(equalType(typeChar,right) && equalType(typeChar,left)) ) {
                                error("\rTypemismatch in div");
                                exit(1);
                        }
                        return right;

                case '%':
                         if(!(equalType(typeInteger,right) && equalType(typeInteger,left)) &&
                            !(equalType(typeChar,right) && equalType(typeChar,left)) ) {
                                error("\rTypemismatch in mod");
                                exit(1);
                        }
                        return right;        

                case '&':
                        if(!equalType(left,typeChar) || !equalType(right,typeChar)){
                                error("\rTypemismatch in AND");
                                exit(-1);
                        }
                        return right;
                case '|':
                        if(!equalType(left,typeChar) || !equalType(right,typeChar)){
                                error("\rTypemismatch in OR");
                                exit(-1);
                        }
                        return right;
                case '!':
                        if(!equalType(right,typeChar)){
                                error("\rTypemismatch in NOT");
                                exit(-1);
                        }
                        return right;

                case 'c': return typeInteger;

                case 'x': {return typeChar;}

                case 'b': return typeChar;

                case '>':
                        if(!equalType(left,right)){
                                error("\rCan't compare operands of different types");
                                exit(1);
                        }
                        else if(!equalType(left,typeChar) && !equalType(left,typeInteger)){
                                error("\roperator > can be used only with int or byte");
                                exit(1);
                        }
                        return typeBoolean;
                case '<':
                        if(!equalType(left,right)){
                                error("\rCant compare operands of different types");
                                exit(1);
                        }
                        else if(!equalType(left,typeChar) && !equalType(left,typeInteger)){
                                error("\roperator < can be used only with int or byte");
                                exit(1);
                        }
                        return typeBoolean;

                case 'l':
                         if(!equalType(left,right)){
                                error("\rCan't compare operands of different types");
                                exit(1);
                        }
                        else if(!equalType(left,typeChar) && !equalType(left,typeInteger)){
                                error("\roperator >= can be used only with int or byte");
                                exit(1);
                        }
                        return typeBoolean;

                case 's':
                         if(!equalType(left,right)){
                                error("\rCan't compare operands of different types");
                                exit(1);
                        }
                        else if(!equalType(left,typeChar) && !equalType(left,typeInteger)){
                                error("\roperator <= can be used only with int or byte");
                                exit(1);
                        }
                        return typeBoolean;

                case 'e':
                        if(!equalType(left,right)){
                                error("\rCan't compare operands of different types");
                                exit(1);
                        }
                        else if(!equalType(left,typeChar) && !equalType(left,typeInteger)){
                                error("\roperator = can be used only with int or byte");
                                exit(1);
                        }
                        return typeBoolean;

                case 'd':
                        if(!equalType(left,right)){
                                error("\rCan't compare operands of different types");
                                exit(1);
                        }
                        else if(!equalType(left,typeChar) && !equalType(left,typeInteger)){
                                error("\roperator <> can be used only with int or byte");
                                exit(1);
                        }
                        return typeBoolean;

                case 'a':
                        if(!equalType(left,typeBoolean) || !equalType(right,typeBoolean)){
                                error("\rand operator requires boolean condition");
                                exit(1);
                        }
                        return right;
                case 'o':
                        if(!equalType(left,typeBoolean) || !equalType(right,typeBoolean)){
                                error("\ror operator requires boolean condition");
                                exit(1);
                        }
                        return right;
                case 'n':
                        if(!equalType(right,typeBoolean) && !equalType(right,typeChar)){
                                error("\rnot operator requires boolean condition");
                                exit(1);
                        }
                        return right;

                case 'f':
                        {
                          SymbolEntry *a = lookupEntry(expr->f->identifier.c_str(),LOOKUP_ALL_SCOPES,true);
                          if(a->entryType != ENTRY_FUNCTION) {
                             error("\rExpression not a function");
                             exit(1);
                           }  
                          

                          SymbolEntry *par = a -> u.eFunction.firstArgument;

                          vector<ASTExpr*> par_vector = *(expr->f->parameters);
                          std::reverse(par_vector.begin(),par_vector.end());
                          int counter  = 0, size = par_vector.size();

                          
                          while(par){
                               
                              if(counter >= size) {error("\rfewer parameter provided in function"); exit(1);}
                              Type par_type = sem_check_expr(par_vector[counter]);
                              if(!equalType(par->u.eParameter.type,par_type)) {
                                   
                                    if(par_type->kind == 5 && par->u.eParameter.type->kind == 5){
                                      if(par->u.eParameter.type->size>0){
                                        error("\rType mismatch in real and typical parameters");
                                        exit(1);
                                      }
                                    }
                                    
                               }
                          
                               counter++;
                               par = par->u.eParameter.next;
                           }

                          if(counter < size) {
                               error("\rmore parameters provided in function than required");
                               exit(1);
                          }

                          if(equalType(a->u.eFunction.resultType,typeVoid)) error("\rvoid function in expression!");
                          return a->u.eFunction.resultType;
                     

                        }

                 case 'i':
                        {
                          ASTlval* lv = expr->operand;

                          //for stringliteral
                          if(lv->constant == true) {
                            return typeArray(strlen(lv->identifier.c_str())+1, typeChar);
                          }


                          SymbolEntry* s = lookupEntry(lv->identifier.c_str(),LOOKUP_ALL_SCOPES,true);
                          //TODO: CONSIDER a[0][1] if a is 2d array a[0][1] is int
                          Type lval_type;

                          if(s->entryType == ENTRY_PARAMETER)
                                    lval_type = s->u.eParameter.type;
                          else if(s->entryType == ENTRY_VARIABLE)
                                    lval_type = s->u.eVariable.type;
                          else{
                                 error("\rNot an lvalue !!");
                                 exit(1);
                          }           
                                                    
                          if((lval_type->kind == 5 && ((*lv->indices)).size()>0) || lval_type->kind == 6){
                            while(lval_type->refType != NULL){
                              lval_type = lval_type->refType;
                            }
                          }

                          return lval_type;
                        }

        }
        return typeVoid;
}