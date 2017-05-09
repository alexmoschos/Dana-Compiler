#ifndef __sem_h
#define __sem_h
#include "symbol.h"
#include "error.h"
#include "ast.h"
int sem_check_fdef(ASTfdef* func);
int sem_check_stmt(ASTstmt* stmt);
int sem_check_expr(ASTExpr* expr);
#endif
