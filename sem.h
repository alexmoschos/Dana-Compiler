#ifndef __sem_h
#define __sem_h
#include "symbol.h"
#include "error.h"
#include "ast.h"
void sem_check_fdef(ASTfdef* func);
void sem_check_stmt(ASTstmt* stmt);
Type sem_check_expr(ASTExpr* expr);
#endif
