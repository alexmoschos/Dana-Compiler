extern "C"
#include "symbol.h"
#include "error.h"
#include "sem.h"
#include "ast.h"
#include <stdio.h>

void sem_check_fdef(ASTfdef* func){
        if(func==NULL) exit(-1);
        openScope();
        ASTheader *header = func->header;
        SymbolEntry *f = newFunction(header->identifier.c_str());
        ASTparam *iter = header->paramlist;
        while(iter != NULL){
                for(auto st : *iter->identifiers){
                        if(iter->byref)
                                newParameter(st.c_str(),iter->p,PASS_BY_REFERENCE,f);
                        else
                                newParameter(st.c_str(),iter->p,PASS_BY_VALUE,f);
                }
                iter = iter->next;
        }

        endFunctionHeader(f,header->type);
        sem_check_stmt(func->body);
        closeScope();
}


void sem_check_stmt(ASTstmt* stmt){
        if(stmt==NULL) return;

        switch(stmt->type){
                case TSKIP: break;

                case TPC:
                        { 
                          SymbolEntry *s = lookupEntry(stmt->expr->f->identifier.c_str(),LOOKUP_ALL_SCOPES,true);
  
                          if(s->entryType!=ENTRY_FUNCTION) error("identifier is not a function");
                          if(!equalType(s->u.eFunction.resultType,typeVoid)) error("non-void function cannot be called as a command"); 

                          SymbolEntry par = s -> u.eFunction.firstArument;

                          vector<ASTExpr*> par_vector = *(stmt->expr->f->parameters); 
                          int counter  = 0, size = par_vector.size();
                    
                          while(par->next){

                              if(counter >= size) {error("fewer parameter provided in function"); exit(1);}
                              if(!equalType(par->u.eParameter.Type,typeVoid),sem_check_expr(par_vector[counter])) 
                                    {error("Type mismatch in real and typical parameters"); exit(1);}
                               counter++;
                               par = par -> next;
                           }

                          if(counter < size) {error("more parameters provided in function than required"); exit(1);}

                          if(stmt->tail) sem_check_stmt(tail);
                          break; 
                        }

                case TFCALL: 
                        { 
                          SymbolEntry *s = lookupEntry(stmt->expr->f->identifier.c_str(),LOOKUP_ALL_SCOPES,true);

                          if(s->entryType!=ENTRY_FUNCTION) {error("identifier is not a function"); exit(1);}
                          if(!equalType(s->u.eFunction.resultType,typeVoid)) error("non-void function cannot be called as a command"); 

                          SymbolEntry par = s -> u.eFunction.firstArument;

                          vector<ASTExpr*> par_vector = *(stmt->expr->f->parameters); 
                          int counter  = 0, size = par_vector.size();
                     
                          while(par -> next){

                             if(counter >= size) {error("fewer parameter provided in function"); exit(1);}
                             if(!equalType(par->u.eParameter.Type,typeVoid),sem_check_expr(par_vector[counter])) 
                                  {error("Type mismatch in real and typical parameters"); exit(1);}
                             counter++;
                             par = par -> next;
                          }

                          if(counter < size) {error("more parameters provided in function than required"); exit(1);}

                          if(stmt->tail) sem_check_stmt(tail);
                          break;
                        }
 
                case TIF:
                        { 
                          Type cond_type = sem_check_expr(stmt->ifnode->condition)
                          if(!equalType(cond_type,typeBoolean) && !equalType(cond_type,typeChar) && !equalType(cond_type,typeInteger))
                             {error("wrong type in if-condition"); exit(1);}

                          sem_check_stmt(stmt->ifnode->body);

                          ASTif* new_ifnode = stmt->ifnode->tail;
                          while(new_ifnode){

                             sem_check_stmt(new_ifnode->body)
                             if(new_ifnode->condition){
                               
                                Type cond_type2 = sem_check_expr(new_ifnode->condition)
                                if(!equalType(cond_type2,typeBoolean) && !equalType(cond_type2,typeChar) && !equalType(cond_type2,typeInteger))
                                     {error("wrong type in if-condition"); exit(1);}
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

                case TEXIT: break;

                case TRET: 
                        { 
                           Type _TRET_TYPE = sem_check_expr(stmt->expr);
                          
                           if(stmt->def == NULL) {error("should have a ASTdef to check function type with return type") exit(1);}      

                           SymbolEntry *s = lookupEntry(stmt->def->header->identifier.c_str(),LOOKUP_ALL_SCOPES,true);
                           if (s->entryType != ENTRY_FUNCTION)
                               internal("NOT a Function");   //redundant all astdef in stack are functions...

                           if(!equalType(_TRET_TYPE,s->u.eFunction.resultType)) {error("Function type and return type different"); exit(1);}
                           break; 
                         }  

                case TCONTM: 
                         {
                           SymbolEntry *s = lookupEntry(stmt->label.c_str(),LOOKUP_ALL_SCOPES,true);
                           if (s->entryType != ENTRY_CONSTANT) internal("NOT a label");     
                           if(!equalType(s->u.eConstant.resultType,typeVoid)) {error("Function type and return type different"); exit(1);}
                           break;
                          }
 
                case TCONT: break;
                case TBREAKM:
                          { 
                            SymbolEntry *s = lookupEntry(stmt->label.c_str(),LOOKUP_ALL_SCOPES,true);
                            if (s->entryType != ENTRY_CONSTANT)  internal("NOT a label");     
                            if(!equalType(s->u.eConstant.resultType,typeVoid)) {error("Function type and return type different"); exit(1);}
                            break;
                          }

                case TBREAK: break;

                case TDECL: 
                          {
                             for(auto st : *stmt->identifiers) newVariable(st.c_str(),stmt->t);
                             break;
                          }

                case TLOOP: 
                         {
                            if((stmt->label).compare("")!=0) newConstant(stmt->label.c_str(),typeVoid);
                            if(stmt->tail) sem_check_stmt(tail);
                            break;
                         }     
      
                case TASSIGN: // to aristera kai to dexia na exoun idio type
                        //sem_check_expr(stmt->expr);
                        break;



        }   
}








//TODO: Rwta gia default type cast apo int se char
Type sem_check_expr(ASTExpr* expr){
        //return typeVoid;
        //return typeVoid;
        if(expr==NULL){
                return typeVoid;
        }
        SymbolEntry *a;
        Type left = sem_check_expr(expr->left);
        Type right = sem_check_expr(expr->right);
        switch(expr->op){
                case '+':
                        //FIXME: UNARYPLUS
                        if(!equalType(left,right)){
                                error("Typemismatch");
                        }

                        return left;
                case '-':
                        //FIXME: UNARYMINUS
                        if(!equalType(left,right)){
                                exit(-1);
                        }

                        return left;
                case '*':
                        if(!equalType(left,right)){
                                exit(-1);
                        }

                        return left;
                case '/':
                        if(!equalType(left,right)){
                                exit(-1);
                        }

                        return left;
                case '&':
                        if(!equalType(left,typeInteger) || !equalType(right,typeInteger)){
                                exit(-1);
                        }
                        return typeInteger;
                case '|':
                        if(!equalType(left,typeInteger) || !equalType(right,typeInteger)){
                                exit(-1);
                        }
                        return typeInteger;
                case '!':
                        if(!equalType(right,typeInteger)){
                                exit(-1);
                        }
                        return typeInteger;
                case 'i':
                        //ASTlval* lv = expr->operand;
                        //SymbolEntry* s = lookupEntry(lv->identifier,LOOKUP_ALL_SCOPES,true);
                        //if(s->entryType == )
                        return typeVoid;
                case 'c': return typeInteger;
                case 'f':
                        //a = lookupEntry(expr->f->identifier.c_str(),LOOKUP_ALL_SCOPES,true);
                        //printType(a->u.eFunction.resultType);
                case 'x': return typeChar;
                case '>':
                        if(!equalType(left,right)){
                                error("Cant compare operands of different types");
                                exit(-1);
                        }
                        return typeBoolean;
                case '<':
                        if(!equalType(left,right)){
                                error("Cant compare operands of different types");
                                exit(-1);
                        }
                        return typeBoolean;
                case 'l':
                        if(!equalType(left,right)){
                                error("Cant compare operands of different types");
                                exit(-1);
                        }
                        return typeBoolean;
                case 's':
                        if(!equalType(left,right)){
                                error("Cant compare operands of different types");
                                exit(-1);
                        }
                        return typeBoolean;
                case 'e':
                        if(!equalType(left,right)){
                                error("Cant compare operands of different types");
                                exit(-1);
                        }
                        return typeBoolean;
                case 'd':
                        if(!equalType(left,right)){
                                error("Cant compare operands of different types");
                                exit(-1);
                        }
                        return typeBoolean;
                case 'b': return typeBoolean;
                case 'a':
                        if(!equalType(left,typeBoolean) || !equalType(right,typeBoolean)){
                                //handle case where
                                //cout << "WHYYYYYYYYYYYYY";
                                //exit(-1);
                        }
                        return typeBoolean;
                case 'o':
                        if(!equalType(left,typeBoolean) || !equalType(right,typeBoolean)){
                                //handle case where
                                //cout << "WHYYYYYYYYYYYYY";
                                //exit(-1);
                        }
                        return typeBoolean;
                case 'n':
                        if(!equalType(right,typeBoolean)){
                                //cout << "WHYYYYYYYYYYYYY";
                                //exit(-1);
                        }
                        return typeBoolean;
        }
        return typeChar;
}
